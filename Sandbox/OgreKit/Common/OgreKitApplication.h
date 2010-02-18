/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
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
#ifndef _OgreKitApplication_h_
#define _OgreKitApplication_h_


// ----------------------------------------------------------------------------
#include "OgrePrerequisites.h"
#include "OgreCommon.h"

#include "OgreKitPlugins.h"
#include "OgreKitMouse.h"
#include "OgreKitKeyboard.h"

class OgreKitApplicationPrivate;

// ----------------------------------------------------------------------------
class OgreKitApplication
{
private:
    friend class OgreKitApplicationPrivate;
	OgreKitApplicationPrivate*	m_private;

protected:
	Ogre::Root*					m_root;
	Ogre::String				m_resourcePath, m_cfg;
	Ogre::RenderWindow*			m_window;
	OgreKitPlugins				m_loader;

	// user input 
    OgreKitKeyboard             m_keyboard;
    OgreKitMouse                m_mouse;
    bool                        m_quit;

	
	virtual bool setup(void);
    virtual void update(Ogre::Real tick){}
    virtual void endFrame(void) {}


    void setupResources(void);

public:

    OgreKitApplication(const Ogre::String& config="OgreKit.cfg");
	virtual ~OgreKitApplication();

	virtual void go(void);
	virtual bool createScene(void) = 0;
};


#endif//_OgreKitApplication_h_
