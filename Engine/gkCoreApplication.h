/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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
#ifndef _gkCoreApplication_h_
#define _gkCoreApplication_h_


#include "gkEngine.h"
#include "gkUserDefs.h"


// Main application interface
class gkCoreApplication : public gkEngine::Listener
{
protected:

	// Internal startup
	bool initialize(void);


protected:
	gkEngine*   m_engine;
	gkUserDefs  m_prefs;

	virtual bool setup(void) {return false;}
	virtual void tick(gkScalar rate) {};


public:
	gkCoreApplication();
	virtual ~gkCoreApplication();

	// access to user options ( apply before a call to run )
	gkUserDefs& getPrefs(void) {return m_prefs;}

	void run(void);
};

#endif//_gkCoreApplication_h_
