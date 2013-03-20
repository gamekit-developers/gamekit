/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Thomas Trocha

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

#include "Process/gkTranslationProcess.h"
#include "gkLogger.h"
#include "gkGameObject.h"


gkTranslationProcess::gkTranslationProcess(gkGameObject* obj,gkScalar time,const gkVector3& toPos)
	:	m_object(obj), m_toOnly(true), m_translationTime(time), m_timeCounter(0),
	 	m_destPos(toPos), m_initalPos(obj->getPosition())
{
}

gkTranslationProcess::gkTranslationProcess(gkGameObject* obj,gkScalar time,const gkVector3& fromPos, const gkVector3& toPos)
	: 	m_object(obj), m_toOnly(false), m_translationTime(time), m_timeCounter(0),
	  	m_destPos(toPos), m_initalPos(fromPos)
{

}

gkTranslationProcess::~gkTranslationProcess()
{
}


bool gkTranslationProcess::isFinished()
{
	if (m_timeCounter >= m_translationTime)
		return true;
	else
		return false;
}

void gkTranslationProcess::init()
{
	// if we didn't specify a fixed fromPos we take the current as startPoint
	if (m_toOnly)
		m_initalPos = m_object->getPosition();

	m_direction = m_destPos - m_initalPos;
	m_timeCounter = 0;
}
void gkTranslationProcess::update(gkScalar delta)
{
	m_timeCounter += delta;

	if (m_object)
	{
		m_object->setPosition(m_initalPos + m_direction * (m_timeCounter / m_translationTime));
	}
}




