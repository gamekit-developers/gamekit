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

#ifndef GKMESSAGESENSOR_H
#define GKMESSAGESENSOR_H

#include "gkGameObject.h"
#include "gkLogicSensor.h"
#include "gkMessageManager.h"

class gkMessageSensor : public gkLogicSensor
{
private:
	gkMessageManager::GenericMessageListener* m_listener;
	utArray<gkMessageManager::Message>       m_messages;



public:
	gkMessageSensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	~gkMessageSensor();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	bool query(void);
	GK_INLINE void            setSubject(const gkString& v)       {m_listener->m_subjectFilter = v;}
	GK_INLINE const gkString& getSubject(void)              const {return m_listener->m_subjectFilter;}
};

#endif // GKMESSAGESENSOR_H
