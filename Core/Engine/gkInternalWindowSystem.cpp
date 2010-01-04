/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "OgreRenderWindow.h"
#include "OgreStringConverter.h"
#include "OgreRoot.h"
#include "OgreWindowEventUtilities.h"

#include "gkInternalWindowSystem.h"
#include "gkLogger.h"

#include "OIS.h"


using namespace Ogre;




// ----------------------------------------------------------------------------
class gkInternalWindowSystemPrivate :
			public OIS::MouseListener,
			public OIS::KeyListener,
			public Ogre::WindowEventListener
{
public:

	gkInternalWindowSystemPrivate(gkInternalWindowSystem *parent);
	virtual ~gkInternalWindowSystemPrivate();


	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);
	void windowResized(Ogre::RenderWindow* rw);

	int getKeyCode(int kc);

	Ogre::RenderWindow*		mOgre;
	OIS::InputManager*		mInputManager;
	OIS::Keyboard*			mKeyboard;
	OIS::Mouse*				mMouse;

	gkMouseDevice			mMouseData;
	gkKeyboardDevice		mKeyboardData;
	gkInternalWindowSystem *mParent;

};








// ----------------------------------------------------------------------------
gkInternalWindowSystem::gkInternalWindowSystem()
{
	mPrivate= new gkInternalWindowSystemPrivate(this);
}

// ----------------------------------------------------------------------------
gkInternalWindowSystem::~gkInternalWindowSystem()
{
	delete mPrivate;
}


// ----------------------------------------------------------------------------
RenderWindow* gkInternalWindowSystem::createWindow(const String& title, unsigned int w, unsigned int h, bool fs)
{
	GK_ASSERT(mPrivate);

	if (mPrivate->mOgre != 0)
	{
		gkLogMessage("Warning: gkInternalWindowSystem::createWindow only one window supported in this context");
		return mPrivate->mOgre;
	}

	RenderWindow *window= mPrivate->mOgre= Root::getSingleton().createRenderWindow(title, w, h, fs);
	window->setActive(true);
	mPrivate->mMouseData.window= window;
	Ogre::WindowEventUtilities::addWindowEventListener(window, mPrivate);


	// absolute in the ogre window
	mArea= mPrivate->mMouseData.area= gkRectangle(0, 0, w, h);

	try
	{
		size_t handle;
		window->getCustomAttribute("WINDOW", &handle);


		OIS::ParamList params;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		SetClassLong((HWND)handle, GCL_HICON,
					 (LONG)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1001)));
#endif
		params.insert(std::make_pair("WINDOW", StringConverter::toString(handle)));

		/// install OIS
		mPrivate->mInputManager= OIS::InputManager::createInputSystem(params);


		mPrivate->mKeyboard= (OIS::Keyboard*) mPrivate->mInputManager->createInputObject(OIS::OISKeyboard, true);
		mPrivate->mKeyboard->setEventCallback(mPrivate);

		mPrivate->mMouse= (OIS::Mouse*)mPrivate->mInputManager->createInputObject(OIS::OISMouse, true);
		mPrivate->mMouse->setEventCallback(mPrivate);


		const OIS::MouseState &st= mPrivate->mMouse->getMouseState();
		st.width= (int)mPrivate->mMouseData.area.width;
		st.height= (int)mPrivate->mMouseData.area.height;
	}

	catch (OIS::Exception &e)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, e.eText, "InternWindowSystem::createWindow");
	}

	mExitReq= false;
	return mPrivate->mOgre;
}

// ----------------------------------------------------------------------------
RenderWindow* gkInternalWindowSystem::getWindow(void)
{
	GK_ASSERT(mPrivate);
	GK_ASSERT(mPrivate->mOgre);
	return mPrivate->mOgre;
}

// ----------------------------------------------------------------------------
void gkInternalWindowSystem::processEvents(void)
{
	GK_ASSERT(mPrivate);
	GK_ASSERT(mPrivate->mOgre);

	if (!mExitReq)
	{
		WindowEventUtilities::messagePump();

		if (mPrivate->mOgre->isClosed())
			mExitReq= true;
	}

}

// ----------------------------------------------------------------------------
void gkInternalWindowSystem::dispatchEvents(void)
{
	GK_ASSERT(mPrivate);
	GK_ASSERT(mPrivate->mKeyboard);
	GK_ASSERT(mPrivate->mMouse);

	mPrivate->mKeyboard->capture();
	mPrivate->mMouse->capture();
}

// ----------------------------------------------------------------------------
void gkInternalWindowSystem::endFrame(void)
{
	GK_ASSERT(mPrivate);
	// clear some states
	mPrivate->mMouseData.moved= false;
	mPrivate->mMouseData.wheelDelta= 0.0;
}

// ----------------------------------------------------------------------------
void gkInternalWindowSystem::swap(void)
{
}


// ----------------------------------------------------------------------------
const gkMouseDevice& gkInternalWindowSystem::getMouse(void)
{
	GK_ASSERT(mPrivate);
	return mPrivate->mMouseData;
}

// ----------------------------------------------------------------------------
const gkKeyboardDevice& gkInternalWindowSystem::getKeyboard(void)
{
	GK_ASSERT(mPrivate);
	return mPrivate->mKeyboardData;
}


// ----------------------------------------------------------------------------
gkInternalWindowSystemPrivate::gkInternalWindowSystemPrivate(gkInternalWindowSystem *parent) :
		mOgre(0), mInputManager(0), mKeyboard(0), mMouse(0), mParent(parent)
{
}


// ----------------------------------------------------------------------------
gkInternalWindowSystemPrivate::~gkInternalWindowSystemPrivate()
{
	if (mInputManager)
	{
		if (mKeyboard)
			mInputManager->destroyInputObject(mKeyboard);

		if (mMouse)
			mInputManager->destroyInputObject(mMouse);

		OIS::InputManager::destroyInputSystem(mInputManager);

		mInputManager= 0;
		mKeyboard= 0;
		mMouse= 0;
	}
	Ogre::WindowEventUtilities::removeWindowEventListener(mOgre, this);
}

// ----------------------------------------------------------------------------
bool gkInternalWindowSystemPrivate::mouseMoved(const OIS::MouseEvent &arg)
{
	mMouseData.position.x= arg.state.X.abs;
	mMouseData.position.y= arg.state.Y.abs;
	mMouseData.relitave.x= arg.state.X.rel;
	mMouseData.relitave.y= arg.state.Y.rel;
	mMouseData.absolute.x= arg.state.X.abs;
	mMouseData.absolute.y= arg.state.Y.abs;

	if (arg.state.Z.rel != 0)
		mMouseData.wheelDelta= arg.state.Z.rel > 0 ? 1.0 : -1.0;

	mMouseData.moved= true;
	return false;
}

// ----------------------------------------------------------------------------
bool gkInternalWindowSystemPrivate::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return false;
}

// ----------------------------------------------------------------------------
bool gkInternalWindowSystemPrivate::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return false;
}

// ----------------------------------------------------------------------------
bool gkInternalWindowSystemPrivate::keyPressed(const OIS::KeyEvent &arg)
{
	GK_ASSERT(mParent);

	if (!mParent->isEscapeKeyDisabled() && arg.key == OIS::KC_ESCAPE)
	{
		mParent->exit();
		return true;
	}

	int kc= getKeyCode(arg.key);
	mKeyboardData.keys[kc]= 0;
	mKeyboardData.keys[kc] |= gkKeyboardDevice::STATE_PRESSED;
	mKeyboardData.was_pressed= true;
	mKeyboardData.key_count += 1;
	return false;
}

// ----------------------------------------------------------------------------
bool gkInternalWindowSystemPrivate::keyReleased(const OIS::KeyEvent &arg)
{
	int kc= getKeyCode(arg.key);

	mKeyboardData.keys[kc]= 0;
	mKeyboardData.keys[kc] |= gkKeyboardDevice::STATE_RELEASED;
	mKeyboardData.was_pressed= true;
	mKeyboardData.key_count -= 1;

	return false;
}

// ----------------------------------------------------------------------------
void gkInternalWindowSystemPrivate::windowResized(Ogre::RenderWindow* rw)
{
	unsigned short sz = rw->getNumViewports();
	gkRectangle &rect = mParent->getArea();
	rect.width = rw->getWidth();
	rect.height = rw->getHeight();

	for (unsigned short i = 0; i<sz; ++i)
	{
		Ogre::Viewport *vp = rw->getViewport(i);
		Ogre::Camera *cam = vp->getCamera();
		cam->setAspectRatio(rect.width / rect.height);
	}
}

// ----------------------------------------------------------------------------
int gkInternalWindowSystemPrivate::getKeyCode(int kc)
{
#define CASE(ret, c) case (c): return ret

	switch (kc)
	{
		CASE(KC_ACCENTGRAVEKEY   ,	  OIS::KC_GRAVE);
		CASE(KC_ZEROKEY		  ,	  OIS::KC_0);
		CASE(KC_ONEKEY		   ,	  OIS::KC_1);
		CASE(KC_TWOKEY		   ,	  OIS::KC_2);
		CASE(KC_THREEKEY		 ,	  OIS::KC_3);
		CASE(KC_FOURKEY		  ,	  OIS::KC_4);
		CASE(KC_FIVEKEY		  ,	  OIS::KC_5);
		CASE(KC_SIXKEY		   ,	  OIS::KC_6);
		CASE(KC_SEVENKEY		 ,	  OIS::KC_7);
		CASE(KC_EIGHTKEY		 ,	  OIS::KC_8);
		CASE(KC_NINEKEY		  ,	  OIS::KC_9);
		CASE(KC_PAD0			 ,	  OIS::KC_NUMPAD0);
		CASE(KC_PAD1			 ,	  OIS::KC_NUMPAD1);
		CASE(KC_PAD2			 ,	  OIS::KC_NUMPAD2);
		CASE(KC_PAD3			 ,	  OIS::KC_NUMPAD3);
		CASE(KC_PAD4			 ,	  OIS::KC_NUMPAD4);
		CASE(KC_PAD5			 ,	  OIS::KC_NUMPAD5);
		CASE(KC_PAD6			 ,	  OIS::KC_NUMPAD6);
		CASE(KC_PAD7			 ,	  OIS::KC_NUMPAD7);
		CASE(KC_PAD8			 ,	  OIS::KC_NUMPAD8);
		CASE(KC_PAD9			 ,	  OIS::KC_NUMPAD9);
		CASE(KC_PADSLASHKEY	  ,	  OIS::KC_DIVIDE);
		CASE(KC_PADENTER		 ,	  OIS::KC_NUMPADENTER);
		CASE(KC_PADEQUALS		,	  OIS::KC_NUMPADEQUALS);
		CASE(KC_PADASTERKEY	  ,	  OIS::KC_MULTIPLY);
		CASE(KC_PADPLUSKEY	   ,	  OIS::KC_ADD);
		CASE(KC_AKEY			 ,	  OIS::KC_A);
		CASE(KC_BKEY			 ,	  OIS::KC_B);
		CASE(KC_CKEY			 ,	  OIS::KC_C);
		CASE(KC_DKEY			 ,	  OIS::KC_D);
		CASE(KC_EKEY			 ,	  OIS::KC_E);
		CASE(KC_FKEY			 ,	  OIS::KC_F);
		CASE(KC_GKEY			 ,	  OIS::KC_G);
		CASE(KC_HKEY			 ,	  OIS::KC_H);
		CASE(KC_IKEY			 ,	  OIS::KC_I);
		CASE(KC_JKEY			 ,	  OIS::KC_J);
		CASE(KC_KKEY			 ,	  OIS::KC_K);
		CASE(KC_LKEY			 ,	  OIS::KC_L);
		CASE(KC_MKEY			 ,	  OIS::KC_M);
		CASE(KC_NKEY			 ,	  OIS::KC_N);
		CASE(KC_OKEY			 ,	  OIS::KC_O);
		CASE(KC_PKEY			 ,	  OIS::KC_P);
		CASE(KC_QKEY			 ,	  OIS::KC_Q);
		CASE(KC_RKEY			 ,	  OIS::KC_R);
		CASE(KC_SKEY			 ,	  OIS::KC_S);
		CASE(KC_TKEY			 ,	  OIS::KC_T);
		CASE(KC_UKEY			 ,	  OIS::KC_U);
		CASE(KC_VKEY			 ,	  OIS::KC_V);
		CASE(KC_WKEY			 ,	  OIS::KC_W);
		CASE(KC_XKEY			 ,	  OIS::KC_X);
		CASE(KC_YKEY			 ,	  OIS::KC_Y);
		CASE(KC_ZKEY			 ,	  OIS::KC_Z);
		CASE(KC_CAPSLOCKKEY	  ,	  OIS::KC_CAPITAL);
		CASE(KC_LEFTCTRLKEY	  ,	  OIS::KC_LCONTROL);
		CASE(KC_LEFTALTKEY	   ,	  OIS::KC_LMENU);
		CASE(KC_RIGHTALTKEY	  ,	  OIS::KC_RMENU);
		CASE(KC_RIGHTCTRLKEY	 ,	  OIS::KC_RCONTROL);
		CASE(KC_RIGHTSHIFTKEY	,	  OIS::KC_RSHIFT);
		CASE(KC_LEFTSHIFTKEY	 ,	  OIS::KC_LSHIFT);
		CASE(KC_ESCKEY		   ,	  OIS::KC_ESCAPE);
		CASE(KC_TABKEY		   ,	  OIS::KC_TAB);
		CASE(KC_RETKEY		   ,	  OIS::KC_RETURN);
		CASE(KC_SPACEKEY		 ,	  OIS::KC_SPACE);
		CASE(KC_BACKSPACEKEY	 ,	  OIS::KC_BACK);
		CASE(KC_DELKEY		   ,	  OIS::KC_DELETE);
		CASE(KC_SEMICOLONKEY	 ,	  OIS::KC_SEMICOLON);
		CASE(KC_PERIODKEY		,	  OIS::KC_PERIOD);
		CASE(KC_COMMAKEY		 ,	  OIS::KC_COMMA);
		CASE(KC_QUOTEKEY		 ,	  OIS::KC_APOSTROPHE);
		CASE(KC_MINUSKEY		 ,	  OIS::KC_MINUS);
		CASE(KC_SLASHKEY		 ,	  OIS::KC_SLASH);
		CASE(KC_BACKSLASHKEY	 ,	  OIS::KC_BACKSLASH);
		CASE(KC_EQUALKEY		 ,	  OIS::KC_EQUALS);
		CASE(KC_LEFTARROWKEY	 ,	  OIS::KC_LEFT);
		CASE(KC_DOWNARROWKEY	 ,	  OIS::KC_DOWN);
		CASE(KC_RIGHTARROWKEY	,	  OIS::KC_RIGHT);
		CASE(KC_UPARROWKEY	   ,	  OIS::KC_UP);
		CASE(KC_F1KEY			,	  OIS::KC_F1);
		CASE(KC_F2KEY			,	  OIS::KC_F2);
		CASE(KC_F3KEY			,	  OIS::KC_F3);
		CASE(KC_F4KEY			,	  OIS::KC_F4);
		CASE(KC_F5KEY			,	  OIS::KC_F5);
		CASE(KC_F6KEY			,	  OIS::KC_F6);
		CASE(KC_F7KEY			,	  OIS::KC_F7);
		CASE(KC_F8KEY			,	  OIS::KC_F8);
		CASE(KC_F9KEY			,	  OIS::KC_F9);
		CASE(KC_F10KEY		   ,	  OIS::KC_F10);
		CASE(KC_F11KEY		   ,	  OIS::KC_F11);
		CASE(KC_F12KEY		   ,	  OIS::KC_F12);
		CASE(KC_HOMEKEY		  ,	  OIS::KC_HOME);
		CASE(KC_PAGEUPKEY		,	  OIS::KC_PGUP);
		CASE(KC_PAGEDOWNKEY	  ,	  OIS::KC_PGDOWN);
		CASE(KC_ENDKEY		   ,	  OIS::KC_END);
		CASE(KC_INSERTKEY		,	  OIS::KC_INSERT);
		CASE(KC_PAUSEKEY		 ,	  OIS::KC_PAUSE);
		CASE(KC_UNKNOWNKEY	   ,	  OIS::KC_AT);
		CASE(KC_UNKNOWNKEY	   ,	  OIS::KC_NUMLOCK);
		CASE(KC_UNKNOWNKEY	   ,	  OIS::KC_RWIN);
		CASE(KC_UNKNOWNKEY	   ,	  OIS::KC_SCROLL);
	}
	return KC_NONE;

#undef CASE
}
