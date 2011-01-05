#include "Input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace ThoseMagnificentMen;

Key Input::GetKeyMapping(int keyCode)
{
	switch (keyCode)
	{
		case VK_OEM_8:		return KeyBackQuote;	// Top top row
		case 0x31:			return Key1;
		case 0x32:			return Key2;
		case 0x33:			return Key3;
		case 0x34:			return Key4;
		case 0x35:			return Key5;
		case 0x36:			return Key6;
		case 0x37:			return Key7;
		case 0x38:			return Key8;
		case 0x39:			return Key9;
		case 0x30:			return Key0;
		case VK_OEM_MINUS:	return KeyMinus;
		case VK_OEM_PLUS:	return KeyPlus;
		case VK_BACK:		return KeyBackspace;
		
		case VK_TAB:		return KeyTab;				// Top row
		case 0x51:			return KeyQ;
		case 0x57:			return KeyW;
		case 0x45:			return KeyE;
		case 0x52:			return KeyR;
		case 0x54:			return KeyT;
		case 0x59:			return KeyY;
		case 0x55:			return KeyU;
		case 0x49:			return KeyI;
		case 0x4F:			return KeyO;
		case 0x50:			return KeyP;
		case VK_OEM_4:		return KeyLeftBrace;
		case VK_OEM_6:		return KeyRightBrace;
		case VK_RETURN:		return KeyEnter;
		
		case VK_CAPITAL:	return KeyCapsLock;			// Middle row
		case 0x41:			return KeyA;
		case 0x53:			return KeyS;
		case 0x44:			return KeyD;
		case 0x46:			return KeyF;
		case 0x47:			return KeyG;
		case 0x48:			return KeyH;
		case 0x4A:			return KeyJ;
		case 0x4B:			return KeyK;
		case 0x4C:			return KeyL;
		case VK_OEM_1:		return KeySemiColon;
		case VK_OEM_3:		return KeySingleQuote;
		case VK_OEM_7:		return KeyHash;
		
		case VK_LSHIFT:		return KeyLeftShift;		// Bottom row
		case VK_OEM_5:		return KeyBackSlash;
		case 0x5A:			return KeyZ;
		case 0x58:			return KeyX;
		case 0x43:			return KeyC;
		case 0x56:			return KeyV;
		case 0x42:			return KeyB;
		case 0x4E:			return KeyN;
		case 0x4D:			return KeyM;
		case VK_OEM_COMMA:	return KeyLeftAngle;
		case VK_OEM_PERIOD:	return KeyRightAngle;
		case VK_OEM_2:		return KeyForwardSlash;
		case VK_RSHIFT:		return KeyRightShift;
		
		case VK_LCONTROL:	return KeyLeftCtrl;			// Bottom bottom row
		case VK_LWIN:		return KeyLeftWin;
		case VK_LMENU:		return KeyAlt;
		case VK_SPACE:		return KeySpace;
		case VK_RMENU:		return KeyAltGr;
		case VK_RWIN:		return KeyRightWin;
		case VK_APPS:		return KeyCopyPaste;
		case VK_RCONTROL:	return KeyRightCtrl;
		
		case VK_UP:			return KeyUp;				// Arrows
		case VK_LEFT: 		return KeyLeft;
		case VK_DOWN: 		return KeyDown;
		case VK_RIGHT:		return KeyRight;
		
		case VK_NUMLOCK:	return KeyNumPadNumLock;	// Number pad
		case VK_DIVIDE:		return KeyNumPadDivide;
		case VK_MULTIPLY:	return KeyNumPadMultiply;
		case VK_SUBTRACT:	return KeyNumPadMinus;
		case VK_NUMPAD7:	return KeyNumPad7;
		case VK_NUMPAD8:	return KeyNumPad8;
		case VK_NUMPAD9:	return KeyNumPad9;
		case VK_ADD:		return KeyNumPadPlus;
		case VK_NUMPAD4:	return KeyNumPad4;
		case VK_NUMPAD5:	return KeyNumPad5;
		case VK_NUMPAD6:	return KeyNumPad6;
		case VK_NUMPAD1:	return KeyNumPad1;
		case VK_NUMPAD2:	return KeyNumPad2;
		case VK_NUMPAD3:	return KeyNumPad3;
		case VK_NUMPAD0:	return KeyNumPad0;
		case VK_DECIMAL:	return KeyNumPadDot;
		
		case VK_ESCAPE:		return KeyEscape;		// Function keys
		case VK_F1:			return KeyF1;
		case VK_F2:			return KeyF2;
		case VK_F3:			return KeyF3;
		case VK_F4:			return KeyF4;
		case VK_F5:			return KeyF5;
		case VK_F6:			return KeyF6;
		case VK_F7:			return KeyF7;
		case VK_F8:			return KeyF8;
		case VK_F9:			return KeyF9;
		case VK_F10:		return KeyF10;
		case VK_F11:		return KeyF11;
		case VK_F12:		return KeyF12;
		
		case VK_INSERT:		return KeyInsert;		// Other crap
		case VK_HOME:		return KeyHome;
		case VK_PRIOR:		return KeyPageUp;
		case VK_DELETE:		return KeyDelete;
		case VK_END:		return KeyEnd;
		case VK_NEXT:		return KeyPageDown;

		case VK_SNAPSHOT:	return KeyPrintScreen;
		case VK_SCROLL:		return KeyScrollLock;
		case VK_CANCEL:
		case VK_PAUSE:		return KeyPause;

		case 0x10:
            short foo;
			foo = GetKeyState(VK_LSHIFT);
			if ((GetKeyState(VK_LSHIFT) & 0x8000) == 0x8000)
				return KeyLeftShift;
			else
				return KeyRightShift;
		case 0x11:
			if ((GetKeyState(VK_LCONTROL) & 0x8000) == 0x8000)
				return KeyLeftCtrl;
			else
				return KeyRightCtrl;
	}
	return KeyBuggeredIfIKnow;
}
