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
/*  Win32 native support added for the Ogre GameKit port by Charlie C.
	http://gamekit.googlecode.com/
*/
#include "OISInputManager.h"
#include "win32native/Win32NativeMouse.h"

using namespace OIS;


//-------------------------------------------------------------//
Win32NativeMouse::Win32NativeMouse(InputManager *creator, bool buffered) :
	Mouse(creator->inputSystemName(), buffered, 0, creator),
	mLastX(0), mLastY(0), 
	mMouseInit(false), 
	mGrab(true), mDoGrab(true),
	mHide(true), mDoHide(true)
{
}

//-------------------------------------------------------------//
Win32NativeMouse::~Win32NativeMouse()
{
}

//-------------------------------------------------------------//
void Win32NativeMouse::setBuffered(bool buffered)
{
	mBuffered = buffered;
}

//-------------------------------------------------------------//
void Win32NativeMouse::handleMouse( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	WNMouseEvent evt;
	bool handled = true;

	if ( msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP )
    {
		if ( msg == WM_LBUTTONDOWN )
			WNAddBit(mState.buttons, MB_Left);
		else
			WNClrBit(mState.buttons, MB_Left);

		evt.type = msg == WM_LBUTTONDOWN ? 3 : 2;
		evt.button = MB_Left;
    }
    else if ( msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP )
    {
		if ( msg == WM_LBUTTONDOWN )
			WNAddBit(mState.buttons, MB_Middle);
		else
			WNClrBit(mState.buttons, MB_Middle);

		evt.type = msg == WM_MBUTTONDOWN ? 3 : 2;
		evt.button = MB_Middle;
    }
    else if ( msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP )
    {
		if ( msg == WM_LBUTTONDOWN )
			WNAddBit(mState.buttons, MB_Right);
		else
			WNClrBit(mState.buttons, MB_Right);

		evt.type = msg == WM_RBUTTONDOWN ? 3 : 2;
		evt.button = MB_Right;
    }
    else if ( msg == WM_MOUSEMOVE )
    {
		RECT rect;
        POINT point;
        ::GetCursorPos( &point );
		::GetWindowRect(hWnd, &rect);

		if (!mMouseInit)
		{
			mLastX = point.x;
			mLastY = point.y;
			mMouseInit = true;
		}

		int rx, ry;
		rx = point.x - mLastX;	
		ry = point.y - mLastY;
		mLastX = point.x;		
		mLastY = point.y;

		if (rx != 0 || ry != 0)
		{
			mState.X.rel = rx;	mState.Y.rel = ry;
			mState.X.abs += rx; mState.Y.abs += ry;

			mState.X.abs = WNClamp(mState.X.abs, 0, mState.width);
			mState.Y.abs = WNClamp(mState.Y.abs, 0, mState.height);

			if (mGrab && mDoGrab)
			{
				mLastX = (rect.left + rect.right) >> 1;
				mLastY = (rect.top + rect.bottom) >> 1;
				::SetCursorPos(mLastX, mLastY);
			}
		}

		evt.type = 0;
    }
    else if ( msg == WM_MOUSEWHEEL )
	{
		mState.Z.rel += short( HIWORD( wParam ) ) > 0 ? 120 : -120;
		mState.Z.abs = mState.Z.rel;
		evt.type = 1;
	}
	else handled = false;

	if (handled && mListener && mBuffered)
		mEvents.push_back(evt);
}

//-------------------------------------------------------------//
void Win32NativeMouse::capture()
{
	if (mListener && mBuffered && !mEvents.empty())
	{
		size_t nr = mEvents.size();
		for (size_t i=0; i<nr; ++i)
		{
			WNMouseEvent &evt = mEvents.at(i);

			if (evt.type <= 1)
			{
				MouseEvent arg(this, mState);
				if (mListener->mouseMoved(arg))
					break;
			}
			if (evt.type == 2)
			{
				MouseEvent arg(this, mState);
				if (mListener->mouseReleased(arg, (MouseButtonID)evt.button))
					break;
			}
			if (evt.type == 3)
			{
				MouseEvent arg(this, mState);
				if (mListener->mousePressed(arg, (MouseButtonID)evt.button))
					break;
			}
		}
		mEvents.clear();
	}
}

//-------------------------------------------------------------//
void Win32NativeMouse::_initialize()
{
	mDoHide = mHide;
	mDoGrab = mGrab;

	::ShowCursor(!mHide);
}

//-------------------------------------------------------------//
void Win32NativeMouse::changedFocus()
{
	if (mGrab) 
		mDoGrab= !mDoGrab;
	
	if (mHide)
	{
		mDoHide = !mDoHide;
		::ShowCursor(!mDoHide);
	}
}
