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
#ifndef _gkWindowSystem_h_
#define _gkWindowSystem_h_

#include "gkCommon.h"
#include "gkMouse.h"
#include "gkKeyboard.h"



// ----------------------------------------------------------------------------
class gkWindowSystem
{
public:

	typedef enum WindowBackend
	{
		OGRE_BACKEND,
		// OTHERS ?
	}WindowBackend;

public:

	static gkWindowSystem *initialize(WindowBackend backend);
	static void finalize(void);
	static gkWindowSystem& getSingleton(void);
	static gkWindowSystem* getSingletonPtr(void);


	virtual void processEvents(void)= 0;
	virtual void dispatchEvents(void) {};
	virtual void endFrame(void) {};

	virtual Ogre::RenderWindow* createWindow(const Ogre::String& title, unsigned int w, unsigned int h, bool fs)= 0;
	virtual Ogre::RenderWindow* getWindow(void)= 0;

	void exit(void);
	bool exitRequest(void);
	void disableEscapeKey(void);
	bool isEscapeKeyDisabled(void);

	virtual const gkMouseDevice& getMouse(void)= 0;
	virtual const gkKeyboardDevice& getKeyboard(void)= 0;

	const gkRectangle& getArea(void) const;
	gkRectangle& getArea(void);

protected:
	gkWindowSystem();
	virtual ~gkWindowSystem();

	virtual void setup(void) {};

	bool mExitReq, mEscDisable;
	gkRectangle		  mArea;
};

// ----------------------------------------------------------------------------
GK_INLINE const gkRectangle& gkWindowSystem::getArea(void) const
{
	return mArea;
}

// ----------------------------------------------------------------------------
GK_INLINE gkRectangle& gkWindowSystem::getArea(void)
{
	return mArea;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkWindowSystem::disableEscapeKey(void)
{
	mEscDisable= true;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkWindowSystem::isEscapeKeyDisabled(void)
{
	return mEscDisable;
}







#endif//_gkWindowSystem_h_
