/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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

#ifndef _gkWindowAndroid_h_
#define _gkWindowAndroid_h_
#include "OISJoyStick.h"


// Internal interface
class gkWindowAndroid : 
	public gkWindow,
	public OIS::MultiTouchListener
{
	virtual bool setupInput(const gkUserDefs& prefs);

public:
	gkWindowAndroid();
	virtual ~gkWindowAndroid();
	
	virtual void dispatch(void);
	virtual void process(void);
	
	bool touchPressed(const OIS::MultiTouchEvent& arg);
	bool touchReleased(const OIS::MultiTouchEvent& arg);
	bool touchMoved(const OIS::MultiTouchEvent& arg);
	bool touchCancelled(const OIS::MultiTouchEvent& arg);

	void transformInputState(OIS::MultiTouchState& state);

	bool axisMoved( const OIS::JoyStickEvent &arg, int axis );


	OIS::MultiTouch*        m_itouch;
	OIS::JoyStick*			m_iacc;
};

#endif //_gkWindowIOS_h_
