#ifndef THOSEMAGNIFICENTMEN_INPUT_H
#define THOSEMAGNIFICENTMEN_INPUT_H

#include <map>

namespace ThoseMagnificentMen
{
    enum Key;

    class Input
    {
    public:
        bool GetKeyDown(Key key) { return keyStates_[key]; }
        void RegisterKeyDown(int keyCode) { keyStates_[GetKeyMapping(keyCode)] = true; }
        void RegisterKeyUp(int keyCode) { keyStates_[GetKeyMapping(keyCode)] = false; }
        Key GetKeyMapping(int keyCode);

    private:
        std::map<Key, bool> keyStates_;
    };

	enum Key
	{
		KeyBackQuote,		// Top top row
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
		Key0,
		KeyMinus,
		KeyPlus,
		KeyBackspace,
		
		KeyTab,				// Top row
		KeyQ,
		KeyW,
		KeyE,
		KeyR,
		KeyT,
		KeyY,
		KeyU,
		KeyI,
		KeyO,
		KeyP,
		KeyLeftBrace,
		KeyRightBrace,
		KeyEnter,
		
		KeyCapsLock,		// Middle row
		KeyA,
		KeyS,
		KeyD,
		KeyF,
		KeyG,
		KeyH,
		KeyJ,
		KeyK,
		KeyL,
		KeySemiColon,
		KeySingleQuote,
		KeyHash,
		
		KeyLeftShift,		// Bottom row
		KeyBackSlash,
		KeyZ,
		KeyX,
		KeyC,
		KeyV,
		KeyB,
		KeyN,
		KeyM,
		KeyLeftAngle,
		KeyRightAngle,
		KeyForwardSlash,
		KeyRightShift,
		
		KeyLeftCtrl,	// Bottom bottom row
		KeyLeftWin,
		KeyAlt,
		KeySpace,
		KeyAltGr,
		KeyRightWin,
		KeyCopyPaste,
		KeyRightCtrl,
		
		KeyUp,			// Arrows
		KeyLeft,
		KeyDown,
		KeyRight,
		
		KeyNumPadNumLock,		// Number pad
		KeyNumPadDivide,
		KeyNumPadMultiply,
		KeyNumPadMinus,
		KeyNumPad7,
		KeyNumPad8,
		KeyNumPad9,
		KeyNumPadPlus,
		KeyNumPad4,
		KeyNumPad5,
		KeyNumPad6,
		KeyNumPad1,
		KeyNumPad2,
		KeyNumPad3,
		KeyNumPad0,
		KeyNumPadDot,
		
		KeyEscape,		// Function keys
		KeyF1,
		KeyF2,
		KeyF3,
		KeyF4,
		KeyF5,
		KeyF6,
		KeyF7,
		KeyF8,
		KeyF9,
		KeyF10,
		KeyF11,
		KeyF12,
		
		KeyInsert,		// Other crap
		KeyHome,
		KeyPageUp,
		KeyDelete,
		KeyEnd,
		KeyPageDown,

		KeyPrintScreen,
		KeyScrollLock,
		KeyPause,
		
		KeyBuggeredIfIKnow
	};
}

#endif // THOSEMAGNIFICENTMEN_INPUT_H
