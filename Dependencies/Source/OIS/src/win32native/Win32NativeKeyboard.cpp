/*
The zlib/libpng License

Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that 
		you wrote the original software. If you use this software in a product, 
		an acknowledgment in the product documentation would be appreciated but is 
		not required.

    2. Altered source versions must be plainly marked as such, and must not be 
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
/*  Win32 native support added for OgreKit by Charlie C.
	http://gamekit.googlecode.com/
*/
#include "OISInputManager.h"
#include "win32native/Win32NativeKeyboard.h"

using namespace OIS;


//-------------------------------------------------------------//
Win32NativeKeyboard::Win32NativeKeyboard(InputManager *creator, bool buffered) :
	Keyboard(creator->inputSystemName(), buffered, 0, creator)
{
	memset(KeyBuffer, 0, 256);
	mTextMode = Ascii;
}

//-------------------------------------------------------------//
Win32NativeKeyboard::~Win32NativeKeyboard()
{
}

//-------------------------------------------------------------//
bool Win32NativeKeyboard::isKeyDown( KeyCode key ) const
{
	return KeyBuffer[key] == 1 ? true : false;
}

//-------------------------------------------------------------//
const std::string& Win32NativeKeyboard::getAsString( KeyCode kc )
{
	static const std::string ret = "TODO";
	return ret;
}

//-------------------------------------------------------------//
void Win32NativeKeyboard::copyKeyStates( char keys[256] ) const
{
	memcpy(keys, KeyBuffer, 256);
}

//-------------------------------------------------------------//
void Win32NativeKeyboard::setBuffered(bool buffered)
{
	mBuffered = buffered;
}

//-------------------------------------------------------------//
void Win32NativeKeyboard::capture()
{
	if (mListener && mBuffered && !mEvents.empty())
	{
		size_t nr = mEvents.size();
		for (size_t i=0; i<nr; ++i)
		{
			WNKeyEvent &evt = mEvents.at(i);
			if (evt.type == 0)
			{
				KeyEvent e(this, (KeyCode)evt.scancode, evt.charcode);
				if (mListener->keyPressed(e) == false)
					break;
			}
			else
			{
				KeyEvent e(this, (KeyCode)evt.scancode, evt.charcode);
				if (mListener->keyReleased(e) == false)
					break;
			}
		}

		mEvents.clear();
	}
}

//-------------------------------------------------------------//
void Win32NativeKeyboard::handleKey( UINT msg, WPARAM wParam, LPARAM lParam )
{
	KeyMap::iterator it = mKeyMap.find(wParam);
	if ( it != mKeyMap.end() )
	{
		char curPress = ( msg == WM_SYSKEYDOWN || msg == WM_KEYDOWN ) ? 1 : 0;
		int code = it->second;

		KeyBuffer[code] = curPress;

		// save modifiers 
		if ( code == KC_LCONTROL || code == KC_RCONTROL )
		{
			if (curPress)
				mModifiers|= Ctrl;
			else
				mModifiers &= ~mModifiers;
		}
		if ( code == KC_LMENU || code == KC_RMENU )
		{
			if (curPress)
				mModifiers|= Alt;
			else
				mModifiers &= ~Alt;
		}
		if ( code == KC_LSHIFT || code == KC_RSHIFT )
		{
			if (curPress)
				mModifiers |= Shift;
			else
				mModifiers &= ~Shift;
		}

		WNKeyEvent evt;
		evt.scancode = it->second;
		evt.charcode = 0;
		if (mTextMode != Off)
		{
			// grab ascii code
			if (mTextMode == Ascii)
			{
				MSG _key;
				if ( ::PeekMessage( &_key, NULL, WM_CHAR, WM_SYSDEADCHAR, PM_REMOVE ) )
					evt.charcode = ( char )_key.wParam;
			}
			else if (mTextMode == Unicode)
			{
				MSG _key;
				if ( ::PeekMessage( &_key, NULL, 0x0109, WM_SYSDEADCHAR, PM_REMOVE ) )
					evt.charcode = ( wchar_t )_key.wParam;
			}
		}

		evt.type = curPress == 1 ? 0 : 1;
		if (mListener && mBuffered) mEvents.push_back(evt);
	}
}


//-------------------------------------------------------------//
void Win32NativeKeyboard::_initialize()
{
#define WNKeyAdd(win, ois) mKeyMap.insert( KeyMap::value_type(win, ois))

	WNKeyAdd(VK_KEY_0, KC_0);
	WNKeyAdd(VK_KEY_1, KC_1);
	WNKeyAdd(VK_KEY_2, KC_2);
	WNKeyAdd(VK_KEY_3, KC_3);
	WNKeyAdd(VK_KEY_4, KC_4);
	WNKeyAdd(VK_KEY_5, KC_5);
	WNKeyAdd(VK_KEY_6, KC_6);
	WNKeyAdd(VK_KEY_7, KC_7);
	WNKeyAdd(VK_KEY_8, KC_8);
	WNKeyAdd(VK_KEY_9, KC_9);
	WNKeyAdd(VK_KEY_A, KC_A);
	WNKeyAdd(VK_KEY_B, KC_B);
	WNKeyAdd(VK_KEY_C, KC_C);
	WNKeyAdd(VK_KEY_D, KC_D);
	WNKeyAdd(VK_KEY_E, KC_E);
	WNKeyAdd(VK_KEY_F, KC_F);
	WNKeyAdd(VK_KEY_G, KC_G);
	WNKeyAdd(VK_KEY_H, KC_H);
	WNKeyAdd(VK_KEY_I, KC_I);
	WNKeyAdd(VK_KEY_J, KC_J);
	WNKeyAdd(VK_KEY_K, KC_K);
	WNKeyAdd(VK_KEY_L, KC_L);
	WNKeyAdd(VK_KEY_M, KC_M);
	WNKeyAdd(VK_KEY_N, KC_N);
	WNKeyAdd(VK_KEY_O, KC_O);
	WNKeyAdd(VK_KEY_P, KC_P);
	WNKeyAdd(VK_KEY_Q, KC_Q);
	WNKeyAdd(VK_KEY_R, KC_R);
	WNKeyAdd(VK_KEY_S, KC_S);
	WNKeyAdd(VK_KEY_T, KC_T);
	WNKeyAdd(VK_KEY_U, KC_U);
	WNKeyAdd(VK_KEY_V, KC_V);
	WNKeyAdd(VK_KEY_W, KC_W);
	WNKeyAdd(VK_KEY_X, KC_X);
	WNKeyAdd(VK_KEY_Y, KC_Y);
	WNKeyAdd(VK_KEY_Z, KC_Z);
	WNKeyAdd(VK_NUMPAD0, KC_NUMPAD0);
	WNKeyAdd(VK_NUMPAD1, KC_NUMPAD1);
	WNKeyAdd(VK_NUMPAD2, KC_NUMPAD2);
	WNKeyAdd(VK_NUMPAD3, KC_NUMPAD3);
	WNKeyAdd(VK_NUMPAD4, KC_NUMPAD4);
	WNKeyAdd(VK_NUMPAD5, KC_NUMPAD5);
	WNKeyAdd(VK_NUMPAD6, KC_NUMPAD6);
	WNKeyAdd(VK_NUMPAD7, KC_NUMPAD7);
	WNKeyAdd(VK_NUMPAD8, KC_NUMPAD8);
	WNKeyAdd(VK_NUMPAD9, KC_NUMPAD9);
	WNKeyAdd(VK_F1, KC_F1);
	WNKeyAdd(VK_F2, KC_F2);
	WNKeyAdd(VK_F3, KC_F3);
	WNKeyAdd(VK_F4, KC_F4);
	WNKeyAdd(VK_F5, KC_F5);
	WNKeyAdd(VK_F6, KC_F6);
	WNKeyAdd(VK_F7, KC_F7);
	WNKeyAdd(VK_F8, KC_F8);
	WNKeyAdd(VK_F9, KC_F9);
	WNKeyAdd(VK_F10, KC_F10);
	WNKeyAdd(VK_F11, KC_F11);
	WNKeyAdd(VK_F12, KC_F12);
	WNKeyAdd(VK_F13, KC_F13);
	WNKeyAdd(VK_F14, KC_F14);
	WNKeyAdd(VK_F15, KC_F15);
	WNKeyAdd(VK_ESCAPE, KC_ESCAPE);
	WNKeyAdd(VK_BACK, KC_BACK);
	WNKeyAdd(VK_RETURN, KC_RETURN);
	WNKeyAdd(VK_DECIMAL, KC_DECIMAL);
	WNKeyAdd(VK_DIVIDE, KC_DIVIDE);
	WNKeyAdd(VK_MULTIPLY, KC_MULTIPLY);
	WNKeyAdd(VK_ADD, KC_ADD);
	WNKeyAdd(VK_SUBTRACT, KC_SUBTRACT);
	WNKeyAdd(VK_LEFT, KC_LEFT);
	WNKeyAdd(VK_RIGHT, KC_RIGHT);
	WNKeyAdd(VK_UP, KC_UP);
	WNKeyAdd(VK_DOWN, KC_DOWN);
	WNKeyAdd(VK_SPACE, KC_SPACE);
	WNKeyAdd(VK_TAB, KC_TAB);
	WNKeyAdd(VK_APPS, KC_APPS);
	WNKeyAdd(VK_CAPITAL, KC_CAPITAL);
	WNKeyAdd(VK_CONVERT, KC_CONVERT);
	WNKeyAdd(VK_DELETE, KC_DELETE);
	WNKeyAdd(VK_END, KC_END);
	WNKeyAdd(VK_HOME, KC_HOME);
	WNKeyAdd(VK_PRIOR, KC_PGUP);
	WNKeyAdd(VK_NEXT, KC_PGDOWN);

	WNKeyAdd(VK_OEM_1, KC_SEMICOLON);
	WNKeyAdd(VK_OEM_2, KC_SLASH );
	WNKeyAdd(VK_OEM_3, KC_GRAVE);
	WNKeyAdd(VK_OEM_4, KC_LBRACKET);
	WNKeyAdd(VK_OEM_5, KC_BACKSLASH);
	WNKeyAdd(VK_OEM_6, KC_RBRACKET);
	WNKeyAdd(VK_OEM_7, KC_APOSTROPHE);
	WNKeyAdd(VK_OEM_102, KC_OEM_102);
	
	WNKeyAdd(VK_INSERT, KC_INSERT);
	WNKeyAdd(VK_KANA, KC_KANA);
	WNKeyAdd(VK_KANJI, KC_KANJI);
	WNKeyAdd(VK_LWIN, KC_LWIN);
	WNKeyAdd(VK_NUMLOCK, KC_NUMLOCK);
	WNKeyAdd(VK_PAUSE, KC_PAUSE);

	WNKeyAdd(VK_RWIN, KC_RWIN);
	WNKeyAdd(VK_SCROLL, KC_SCROLL);
	WNKeyAdd(VK_SLEEP, KC_SLEEP);

	WNKeyAdd(VK_CONTROL, KC_LCONTROL);
	WNKeyAdd(VK_RCONTROL, KC_RCONTROL);
	WNKeyAdd(VK_LCONTROL, KC_LCONTROL);
	WNKeyAdd(VK_MENU,  KC_LMENU);
	WNKeyAdd(VK_RMENU, KC_RMENU);
	WNKeyAdd(VK_LMENU, KC_LMENU);
	WNKeyAdd(VK_SHIFT,  KC_LSHIFT);
	WNKeyAdd(VK_RSHIFT, KC_RSHIFT);
	WNKeyAdd(VK_LSHIFT, KC_LSHIFT);
}
