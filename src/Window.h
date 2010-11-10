#ifndef THOSEMAGNIFICENTMEN_WINDOW_H
#define THOSEMAGNIFICENTMEN_WINDOW_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Input.h"

namespace ThoseMagnificentMen
{
    class Window
    {
    public:
        Window();
        ~Window();

        bool Create(HINSTANCE instance);
        bool Init(Input *input);
        void SwapBuffers();

        static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        HWND window_;
        HDC deviceContext_;
        HGLRC renderContext_;
        static Input *input_;
    };
}

#endif // THOSEMAGNIFICENTMEN_WINDOW_H
