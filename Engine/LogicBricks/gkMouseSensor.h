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
#ifndef _gkMouseSensor_h_
#define _gkMouseSensor_h_

#include "gkLogicSensor.h"
#include "gkLogicDispatcher.h"
#include "gkWindowSystem.h"

// Mouse event handler
class gkMouseDispatch : public gkAbstractDispatcher
{
public:
	gkMouseDispatch();
	virtual ~gkMouseDispatch();

	void dispatch(void);
};


class gkMouseSensor : public gkLogicSensor
{
public:

	enum Type
	{
		MOUSE_NILL = 0,
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		MOUSE_MOTION,
		MOUSE_WHEEL_UP,
		MOUSE_WHEEL_DOWN,
		MOUSE_MOUSE_OVER,
		MOUSE_MOUSE_OVER_ANY,
	};

protected:

	int m_type;
	bool m_last;

	Ogre::RaySceneQuery* m_rayQuery;
	bool rayTest(void);

public:
	gkMouseSensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkMouseSensor();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	bool query(void);

	GK_INLINE void setType(int type)       {m_type = type;}
	GK_INLINE int  getType(void)     const {return m_type;}
};


#endif//_gkMouseSensor_h_
