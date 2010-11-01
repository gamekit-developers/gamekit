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

#ifndef GKRANDOMSENSOR_H
#define GKRANDOMSENSOR_H

#include "gkLogicSensor.h"
#include "utRandom.h"

class gkRandomSensor : public gkLogicSensor
{
private:
	UTuint32 m_seed;
	UTuint32 m_current;
	unsigned int m_count;

	utRandomNumberGenerator* m_randGen;

public:
	gkRandomSensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	~gkRandomSensor();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	bool query(void);
	void setSeed(UTuint32 v);

	GK_INLINE UTuint32 getSeed(void) const {return m_seed;}
};

#endif // GKRANDOMSENSOR_H
