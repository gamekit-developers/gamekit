/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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
#ifndef GKNEARSENSOR_H
#define GKNEARSENSOR_H

#include "gkLogicSensor.h"

class gkNearSensor : public gkLogicSensor
{

private:
	gkScalar    m_range, m_resetrange;
	gkString    m_material, m_prop;
	bool        m_previous;
	utArray<gkGameObject*> m_nearObjList;

public:

	gkNearSensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkNearSensor() {}

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	bool query(void);

	GK_INLINE void setRange(gkScalar v)             {m_range = v;}
	GK_INLINE void setResetRange(gkScalar v)        {m_resetrange = v;}
	GK_INLINE void setMaterial(const gkString& v)   {m_material = v; m_prop = "";}
	GK_INLINE void setProperty(const gkString& v)   {m_prop = v; m_material = "";}

	GK_INLINE gkScalar getRange(void)               const {return m_range;}
	GK_INLINE gkScalar getResetRange(void)          const {return m_resetrange;}
	GK_INLINE const gkString& getMaterial(void)     const {return m_material;}
	GK_INLINE const gkString& getProperty(void)     const {return m_prop;}
	GK_INLINE const utArray<gkGameObject*> getNearObjects(void) const {return m_nearObjList;}
	GK_INLINE const int getNearObjectCount(void) 	    const {return m_nearObjList.size();}
	GK_INLINE const gkGameObject* getNearObject(int nr)  {return m_nearObjList[nr];}
};

#endif // GKNEARSENSOR_H
