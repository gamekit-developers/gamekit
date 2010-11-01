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
#ifndef _gkKeyboardSensor_h_
#define _gkKeyboardSensor_h_

#include "gkLogicSensor.h"
#include "gkLogicDispatcher.h"
#include "gkWindowSystem.h"



class gkKeyDispatch : public gkAbstractDispatcher
{
public:
	gkKeyDispatch();
	virtual ~gkKeyDispatch();

	void dispatch(void);
};



class gkKeyboardSensor : public gkLogicSensor
{
protected:
	int m_key, m_mod0, m_mod1;
	bool m_allKeys;

public:
	gkKeyboardSensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkKeyboardSensor() {}

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	bool query(void);


	GK_INLINE void setKey(int v)      {m_key  = v;}
	GK_INLINE void setMod0(int v)     {m_mod0 = v;}
	GK_INLINE void setMod1(int v)     {m_mod1 = v;}
	GK_INLINE void setAllKeys(bool v) {m_allKeys  = v;}
	
	GK_INLINE int  getKey(void)       const {return m_key;}
	GK_INLINE int  getMod0(void)      const {return m_mod0;}
	GK_INLINE int  getMod1(void)      const {return m_mod1;}
	GK_INLINE bool getAllKeys(void)   const {return m_allKeys;}
};


#endif//_gkKeyboardSensor_h_
