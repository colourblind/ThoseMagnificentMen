#include "Window.h"

#include <tchar.h>

#include "Constants.h"
#include "OpenGL.h"

using namespace ThoseMagnificentMen;

Input *Window::input_ = NULL;

Window::Window()
{

}

Window::~Window()
{
	if (renderContext_)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(renderContext_);
	}
	if (window_ && deviceContext_)
		ReleaseDC(window_, deviceContext_);
}

bool Window::Create(HINSTANCE instance)
{
    DWORD exStyle;	
    DWORD style;
	WNDCLASS windowClass;

    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = (WNDPROC)MessageHandler;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = instance;
    windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);	
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = NULL;			
    windowClass.lpszMenuName = NULL;		
    windowClass.lpszClassName = _T("ThoseMagnificentMenWindow");
	
    if (!::RegisterClass(&windowClass))
		return false;			

    // Fullscreen?
    if (false)
    {
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth	= WIDTH;
		screenSettings.dmPelsHeight	= HEIGHT;
		screenSettings.dmBitsPerPel	= 32;
		screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			return false;

		exStyle = WS_EX_APPWINDOW;
		style = WS_POPUP;
		ShowCursor(false);
    }
    else
    {
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
		style = WS_OVERLAPPEDWINDOW;	
    }
	
    window_ = CreateWindowEx(exStyle,
				_T("ThoseMagnificentMenWindow"),
				_T("ThoseMagnificentMen"),
				style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				0,
				0,
				WIDTH,
				HEIGHT,
				NULL,
				NULL,
				windowClass.hInstance,
				NULL);
				
	if (!window_)
    {
        DWORD fail = ::GetLastError();
		return false;
    }

	return true;
}

bool Window::Init(Input *input)
{
	if (!window_)
		return false;

    input_ = input;

    deviceContext_ = GetDC(window_);
    if (!deviceContext_)
		return false;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int pixelFormat = ChoosePixelFormat(deviceContext_, &pfd);
	if (pixelFormat == 0)
		return false;
	
	if (!SetPixelFormat(deviceContext_, pixelFormat, &pfd))
		return false;
	
    renderContext_ = wglCreateContext(deviceContext_); 
    if (!renderContext_)
        return false;
    ::wglMakeCurrent(deviceContext_, renderContext_);

    ::ShowWindow(window_, SW_SHOWDEFAULT);
    ::UpdateWindow(window_);

	return true;
}

void Window::SwapBuffers()
{
	::SwapBuffers(deviceContext_);
}

LRESULT CALLBACK Window::MessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	long result = 0;
	switch (message)
	{
		case WM_PAINT:
            hdc = ::BeginPaint(window, &ps);
            EndPaint(window, &ps);
			break;
		case WM_KEYDOWN:
			input_->RegisterKeyDown(wParam);
			break;
		case WM_KEYUP:
			input_->RegisterKeyUp(wParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
            // This seems to die if we wait to return the result until the end callback
			return DefWindowProc(window, message, wParam, lParam);
	}

	if (input_->GetKeyDown(KeyEscape))
		::PostQuitMessage(0);

	return result;
}
