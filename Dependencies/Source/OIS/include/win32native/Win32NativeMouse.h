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
#ifndef _WIN32_NATIVE_MOUSE_H_
#define _WIN32_NATIVE_MOUSE_H_

#include "OISMouse.h"
#include "win32native/Win32NativePrereqs.h"

namespace OIS
{
	/**
		Native Win32 mouse backend
	*/
	class Win32NativeMouse : public Mouse
	{
	public:
		Win32NativeMouse(InputManager *creator, bool buffered, bool grab, bool hide);
		virtual ~Win32NativeMouse();

		/** @copydoc Object::setBuffered */
		 void setBuffered(bool buffered);

		/** @copydoc Object::capture */
		 void capture();

		/** @copydoc Object::queryInterface */
		 Interface* queryInterface(Interface::IType type) {return 0;}

		/** @copydoc Object::_initialize */
		 void _initialize();

		/** Notify that the mouse has gained/lost focus */
		 void changedFocus();

		 /** Process mouse events from the window system */
		 void handleMouse( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		 void _setHandle(HWND hWnd) {mHandle = hWnd;}


	private:
		int mLastX, mLastY;
		bool mMouseInit, mMouseMoved;
		bool mGrab, mDoGrab;
		bool mHide, mDoHide;

		HWND mHandle;
		typedef std::vector<WNMouseEvent> WNMouseEventVector;
		WNMouseEventVector mEvents;
	};
}

#endif//_WIN32_NATIVE_MOUSE_H_
