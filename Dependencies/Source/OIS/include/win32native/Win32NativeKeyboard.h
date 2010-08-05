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
#ifndef _WIN32_NATIVE_KEYBOARD_H_
#define _WIN32_NATIVE_KEYBOARD_H_

#include "OISKeyboard.h"
#include "win32native/Win32NativePrereqs.h"

namespace OIS
{
	/**
		Native Win32 keyboard backend
	*/
	class Win32NativeKeyboard : public Keyboard
	{
	public:
		Win32NativeKeyboard(InputManager *creator, bool buffered);
		virtual ~Win32NativeKeyboard();

		/** @copydoc Keyboard::isKeyDown */
		bool isKeyDown( KeyCode key ) const;

		/** @copydoc Keyboard::getAsString */
		const std::string& getAsString( KeyCode kc );

		/** @copydoc Keyboard::copyKeyStates */
		void copyKeyStates( char keys[256] ) const;

		/** @copydoc Object::setBuffered */
		void setBuffered(bool buffered);

		/** @copydoc Object::capture */
		void capture();

		/** @copydoc Object::queryInterface */
		Interface* queryInterface(Interface::IType type) {return 0;}

		/** @copydoc Object::_initialize */
		void _initialize();


		/** Process key events from the window system */
		void handleKey( UINT msg, WPARAM wParam, LPARAM lParam );

	protected:

		unsigned char KeyBuffer[256];
		typedef std::map<int, KeyCode> KeyMap;
		KeyMap mKeyMap;

		typedef std::vector<WNKeyEvent> WNKeyEventVector;
		WNKeyEventVector mEvents;
	};
}

#endif//_WIN32_NATIVE_KEYBOARD_H_
