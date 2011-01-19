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
#include <assert.h>
#include <stdlib.h>
#include <map>
#include "win32native/Win32NativeInputManager.h"
#include "win32native/Win32NativeKeyboard.h"
#include "win32native/Win32NativeMouse.h"
#include "OISException.h"

using namespace OIS;


//GWLP_USERDATA is used by Ogre WindowEventUtilities.
typedef std::map<HWND, Win32NativeInputManager*> Win32InputMgrMap;
static Win32InputMgrMap gMgr;


//-------------------------------------------------------------//
static LRESULT WINAPI OIS_SystemProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win32InputMgrMap::iterator i = gMgr.find(hWnd);
	if (i != gMgr.end())
		return i->second->_proc(hWnd, msg, wParam, lParam);

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

//-------------------------------------------------------------//
Win32NativeInputManager::Win32NativeInputManager() :
		OIS::InputManager("Win32NativeInputManager"),
		mKeyboard(0), mMouse(0), mOldProc(0), mGrab(true), mHide(true), mHandle(0), mPass(false)
{
	mFactories.push_back(this);
}

//-------------------------------------------------------------//
Win32NativeInputManager::~Win32NativeInputManager()
{
	if (mHandle)
	{
		if (mOldProc != 0) //restore old
			::SetWindowLongPtr(mHandle, GWL_WNDPROC, (LONG_PTR)mOldProc);

		gMgr.erase(mHandle);
	}
}

//-------------------------------------------------------------//
void Win32NativeInputManager::_initialize(ParamList &paramList)
{
	// grab window
	ParamList::iterator it= paramList.find("WINDOW");
	if (it == paramList.end())
		OIS_EXCEPT(E_InvalidParam, "Win32NativeInputManager::Win32NativeInputManager >> No HWND found!");

	mHandle = (HWND)strtoul(it->second.c_str(), 0, 10);
	if (IsWindow(mHandle) == 0)
		OIS_EXCEPT(E_General, "Win32NativeInputManager::Win32NativeInputManager >> The sent HWND is not valid!");

	// save old
	mOldProc= (WNDPROC)::GetWindowLongPtr(mHandle, GWL_WNDPROC);
	if (!mOldProc)
		OIS_EXCEPT(E_General, "Win32NativeInputManager::Win32NativeInputManager >> Window has no existing procedure");

	gMgr[mHandle] = this;

	ParamList::iterator i = paramList.begin(), e = paramList.end();
	for( ; i != e; ++i ) 
	{
		if( i->first == "w32_mouse" )
        {
            if (i->second == "DISCL_FOREGROUND")
				mHide = false;
            else if (i->second == "DISCL_NONEXCLUSIVE")
				mGrab= false;
        }

		if( i->first == "w32_pass_event" )
			mPass = true;
	}

	if (mOldProc != 0)
	{
		// add subclass
		::SetWindowLongPtr(mHandle, GWL_WNDPROC, (LONG_PTR)OIS_SystemProc);
	}
}

//-------------------------------------------------------------//
LRESULT WINAPI Win32NativeInputManager::_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (mMouse)
			mMouse->changedFocus();
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		if (mMouse != 0)
		{
			mMouse->handleMouse(hWnd, msg, wParam, lParam);
			if (!mPass) return 0;
		}
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (mKeyboard)
		{
			mKeyboard->handleKey(msg, wParam, lParam);
			if (!mPass) return 0;
		}
		break;
	}
	return ::CallWindowProc(mOldProc, hWnd, msg, wParam, lParam);
}

//-------------------------------------------------------------//
DeviceList Win32NativeInputManager::freeDeviceList()
{
	return DeviceList();
}

//-------------------------------------------------------------//
int Win32NativeInputManager::totalDevices(Type iType)
{
	switch (iType)
	{
	case OISKeyboard:	return 1;
	case OISMouse:		return 1;
	default: return 0;
	}
}

//-------------------------------------------------------------//
int Win32NativeInputManager::freeDevices(Type iType)
{
	switch (iType)
	{
	case OISKeyboard:	return mKeyboard ? 0 : 1;
	case OISMouse:		return mMouse ? 0 : 1;
	default: return 0;
	}
}

//-------------------------------------------------------------//
bool Win32NativeInputManager::vendorExist(Type iType, const std::string & vendor)
{
	return true;
}

//-------------------------------------------------------------//
Object* Win32NativeInputManager::createObject(InputManager* creator, Type iType, bool bufferMode, const std::string & vendor)
{
	Object *ret= 0;
	switch (iType)
	{
	case OISKeyboard:
		if (!mKeyboard)
			ret= mKeyboard= new Win32NativeKeyboard(this, bufferMode);
		break;
	case OISMouse:
		if (!mMouse)
		{
			ret= mMouse= new Win32NativeMouse(this, bufferMode, mGrab, mHide);
			mMouse->_setHandle(mHandle);
		}
		break;
	}
	return ret;
}

//-------------------------------------------------------------//
void Win32NativeInputManager::destroyObject(Object* obj)
{
	delete obj;
}
