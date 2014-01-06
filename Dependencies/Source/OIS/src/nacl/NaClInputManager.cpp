/*
The zlib/libpng License

Copyright (c) 2005-2010 harkon.kr(http://gamekit.googlecode.com/)

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

#include "OISException.h"
#include "nacl/NaClInputManager.h"

#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <cstdio>

namespace OIS
{

NaClInputManager::NaClInputManager()
	:	InputManager("NaClInputManager"),
		mKeyboard(0)
{
	mFactories.push_back(this);
}

NaClInputManager::~NaClInputManager()
{

}

void NaClInputManager::_initialize( ParamList &paramList )
{
	ParamList::iterator i = paramList.find("WINDOW");
	if(i != paramList.end())
	{
		size_t whandle = strtoul(i->second.c_str(), 0, 10);		
    }

	//TODO: setup window
}

DeviceList NaClInputManager::freeDeviceList()
{
	DeviceList ret;

	//if( bAccelerometerUsed == false )
		ret.insert(std::make_pair(OISKeyboard, mInputSystemName));

	//if( bMultiTouchUsed == false )
		ret.insert(std::make_pair(OISMultiTouch, mInputSystemName));

	//if (mAccelerometer){
	//	ret.insert(std::make_pair(OISJoyStick, mInputSystemName));
	//}

	return ret;
}

int NaClInputManager::totalDevices(Type iType)
{
	switch(iType)
	{
        case OISKeyboard: return 1;
        case OISMultiTouch: return 1;
        case OISJoyStick : return 1;
        default: return 0;
	}
}

int NaClInputManager::freeDevices(Type iType)
{
	switch(iType)
	{
        case OISKeyboard: return 1; //bAccelerometerUsed ? 0 : 1;
        case OISMultiTouch: return 1; //bMultiTouchUsed ? 0 : 1;
        case OISJoyStick : return 1;
        default: return 0;
	}
}

bool NaClInputManager::vendorExist(Type iType, const std::string & vendor)
{
	if( ( iType == OISMultiTouch || iType == OISKeyboard || iType == OISJoyStick) && vendor == mInputSystemName )
		return true;

	return false;
}



Object* NaClInputManager::createObject(InputManager* creator, Type iType, bool bufferMode, const std::string & vendor)
{
	Object *obj = 0;
	

	switch(iType)
	{
	case OISKeyboard: 
		{		
			NaClKeyboard *keyboard = new NaClKeyboard(this);
			obj = keyboard;
			if (!mKeyboard) mKeyboard = keyboard;		
			break;
		}
		
#if 0
	case OISMultiTouch:
		{
			NaClMultiTouch* touch = new NaClMultiTouch(this);
			obj = touch;
			if (!mTouch) mTouch = touch;		
			break;
		}
	case OISJoyStick:
		{
			AndroidAccelerometer* accel = new AndroidAccelerometer(this,true);
			obj = accel;
			if (!mAccelerometer)
				mAccelerometer = accel;
			break;
		}
#endif
	default:
		break;
	}


	if( obj == 0 )
		OIS_EXCEPT(E_InputDeviceNonExistant, "No devices match requested type.");


	return obj;
}

void NaClInputManager::destroyObject(Object* obj)
{
	delete obj;

	//if (obj == mTouch) mTouch = 0;
	//else if (obj == mKeyboard) mKeyboard = 0;
}


//--

NaClKeyboard::NaClKeyboard(InputManager* creator) 
	:	Keyboard(creator->inputSystemName(), false, 1, 0) 
{
	resetBuffer();
}

void NaClKeyboard::resetBuffer()
{
	memset(mKeyBuffer, 0, sizeof(mKeyBuffer));
}

void NaClKeyboard::copyKeyStates(char keys[256]) const
{
	memcpy(keys, mKeyBuffer, sizeof(keys));
}

const std::string& NaClKeyboard::getAsString(KeyCode kc)
{
	char buf[2] = { (char)convertKey(0, kc).key, 0 };
	return std::string(buf);
}

bool NaClKeyboard::isKeyDown(KeyCode key) const 
{ 
	return key < 256 && mKeyBuffer[key] != 0;
}

void NaClKeyboard::injectKey(int action, int uniChar, int keyCode)
{
	KeyEvent evt = NaClKeyboard::convertKey(action, keyCode);
	evt.device = this;
	bool pressed = (action == 0);

	assert(evt.key < 256);
	mKeyBuffer[evt.key] = pressed;

	if (!mListener) return;
	
	if (pressed)
		mListener->keyPressed(evt);
	else
		mListener->keyReleased(evt);
}


//copied from ogre3d android samplebrower
KeyEvent NaClKeyboard::convertKey(int action, int keyCode)
{
	KeyCode kc = OIS::KC_UNASSIGNED;
	unsigned int txt = 0;
		
	switch(keyCode){
	case 7:
		kc = OIS::KC_0;
		txt = '0';
		break;
	case 8:
		kc = OIS::KC_1;
		txt = '1';
		break;
	case 9:
		kc = OIS::KC_2;
		txt = '2';
		break;
	case 10:
		kc = OIS::KC_3;
		txt = '3';
		break;
	case 11:
		kc = OIS::KC_4;
		txt = '4';
		break;
	case 12:
		kc = OIS::KC_5;
		txt = '5';
		break;
	case 13:
		kc = OIS::KC_6;
		txt = '6';
		break;
	case 14:
		kc = OIS::KC_7;
		txt = '7';
		break;
	case 15:
		kc = OIS::KC_8;
		txt = '8';
		break;
	case 16:
		kc = OIS::KC_9;
		txt = '9';
		break;
	case 69:
		kc = OIS::KC_MINUS;
		txt = '-';
		break;
	case 70:
		kc = OIS::KC_EQUALS;
		txt = '=';
		break;
	case 61:
		kc = OIS::KC_TAB;
		txt = '\t';
		break;
	case 67:
		kc = OIS::KC_BACK;
		txt = '\b';
		break;
	case 29:
		kc = OIS::KC_A;
		txt = 'A';
		break;
	case 30:
		kc = OIS::KC_B;
		txt = 'B';
		break;
	case 31:
		kc = OIS::KC_C;
		txt = 'C';
		break;
	case 32:
		kc = OIS::KC_D;
		txt = 'D';
		break;
	case 33:
		kc = OIS::KC_E;
		txt = 'E';
		break;
	case 34:
		kc = OIS::KC_F;
		txt = 'F';
		break;
	case 35:
		kc = OIS::KC_G;
		txt = 'G';
		break;
	case 36:
		kc = OIS::KC_H;
		txt = 'H';
		break;
	case 37:
		kc = OIS::KC_I;
		txt = 'I';
		break;
	case 38:
		kc = OIS::KC_J;
		txt = 'J';
		break;
	case 39:
		kc = OIS::KC_K;
		txt = 'K';
		break;
	case 40:
		kc = OIS::KC_L;
		txt = 'L';
		break;
	case 41:
		kc = OIS::KC_M;
		txt = 'M';
		break;
	case 42:
		kc = OIS::KC_N;
		txt = 'N';
		break;
	case 43:
		kc = OIS::KC_O;
		txt = 'O';
		break;
	case 44:
		kc = OIS::KC_P;
		txt = 'P';
		break;
	case 45:
		kc = OIS::KC_Q;
		txt = 'Q';
		break;
	case 46:
		kc = OIS::KC_R;
		txt = 'R';
		break;
	case 47:
		kc = OIS::KC_S;
		txt = 'S';
		break;
	case 48:
		kc = OIS::KC_T;
		txt = 'T';
		break;
	case 49:
		kc = OIS::KC_U;
		txt = 'U';
		break;
	case 50:
		kc = OIS::KC_V;
		txt = 'V';
		break;
	case 51:
		kc = OIS::KC_W;
		txt = 'W';
		break;
	case 52:
		kc = OIS::KC_X;
		txt = 'X';
		break;
	case 53:
		kc = OIS::KC_Y;
		txt = 'Y';
		break;
	case 54:
		kc = OIS::KC_Z;
		txt = 'Z';
		break;
	case 71:
		kc = OIS::KC_LBRACKET;
		txt = '[';
		break;
	case 72:
		kc = OIS::KC_RBRACKET;
		txt = ']';
		break;
	case 66:
		kc = OIS::KC_RETURN;
		txt = '\n';
		break;
	case 74:
		kc = OIS::KC_SEMICOLON;
		txt = ';';
		break;
	case 75:
		kc = OIS::KC_APOSTROPHE;
		txt = '\'';
		break;
	case 73:
		kc = OIS::KC_BACKSLASH;
		txt = '\\';
		break;
	case 55:
		kc = OIS::KC_COMMA;
		txt = ','; 
		break;
	case 56:
		kc = OIS::KC_PERIOD;
		txt = '.';
		break;
	case 76:
		kc = OIS::KC_SLASH;
		txt = '/';
		break;
	case 19:
		kc = OIS::KC_UP;
		break;
	case 20:
		kc = OIS::KC_DOWN;
		break;
	case 21:
		kc = OIS::KC_LEFT;
		break;
	case 22:
		kc = OIS::KC_RIGHT;
		break;
	}
		
	return KeyEvent(0, kc, txt);
}


//--


NaClMouse::NaClMouse(InputManager *creator, bool buffered, bool grab, bool hide)
	: Mouse(creator->inputSystemName(), buffered, 0, creator)
{
}

NaClMouse::~NaClMouse()
{
}

void NaClMouse::setBuffered(bool buffered)
{
}


void NaClMouse::capture()
{
}

Interface* NaClMouse::queryInterface(Interface::IType type) 
{
	return 0;
}


void NaClMouse::_initialize()
{
}


void NaClMouse::changedFocus()
{
}



}
