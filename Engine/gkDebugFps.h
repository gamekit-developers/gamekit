/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): xat
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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
#ifndef _gkDebugFps_h_
#define _gkDebugFps_h_

#include "gkVariable.h"

class gkDebugFps
{
private:

	bool m_isInit, m_isShown;

	gkString m_keys;

	Ogre::Overlay*           m_over;
	Ogre::OverlayContainer*  m_cont;
	Ogre::OverlayElement*    m_key, *m_val;


public:

	gkDebugFps();
	~gkDebugFps();


	void initialize(void);
	void show(bool v);


	bool isShown(void) {return m_isShown;}

	void draw(void);
};

#endif // _gkDebugFps_h_
