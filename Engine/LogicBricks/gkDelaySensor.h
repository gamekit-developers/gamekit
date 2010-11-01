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

#ifndef GKDELAYSENSOR_H
#define GKDELAYSENSOR_H

#include "gkLogicSensor.h"

class gkDelaySensor : public gkLogicSensor
{
private:
	unsigned int m_delay, m_duration, m_count;
	bool m_repeat;
public:
	gkDelaySensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkDelaySensor() {}

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	bool query(void);
	GK_INLINE void setDelay(unsigned int v)    {m_delay = v;}
	GK_INLINE void setDuration(unsigned int v) {m_duration = v;}
	GK_INLINE void setRepeat(bool v)           {m_repeat = v;}

	GK_INLINE unsigned int getDelay(void)      const {return m_delay;}
	GK_INLINE unsigned int getDuration(void)   const {return m_duration;}
	GK_INLINE bool         getRepeat(void)     const {return m_repeat;}

};

#endif // GKDELAYSENSOR_H
