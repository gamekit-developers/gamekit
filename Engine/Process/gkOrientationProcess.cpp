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

#include "Process/gkOrientationProcess.h"
#include "gkLogger.h"
#include "gkGameObject.h"
#include "gkValue.h"


gkOrientationProcess::gkOrientationProcess(gkGameObject* obj,gkScalar time,const gkQuaternion& destOrientation)
	:	m_object(obj), m_toOnly(true), m_translationTime(time), m_timeCounter(0),
	 	m_initalOrientation(obj->getOrientation()), m_destOrientation(destOrientation)
{

}


gkOrientationProcess::gkOrientationProcess(gkGameObject* obj,gkScalar time,const gkQuaternion& startOrientation, const gkQuaternion& destOrientation)
:	m_object(obj), m_toOnly(false), m_translationTime(time), m_timeCounter(0),
 	m_initalOrientation(startOrientation), m_destOrientation(destOrientation)
{}

gkOrientationProcess::gkOrientationProcess(gkGameObject* obj,gkScalar time,const gkVector3& destOrientation)
	:	m_object(obj), m_toOnly(true), m_translationTime(time), m_timeCounter(0),
	 	m_initalOrientation(obj->getOrientation()), m_destOrientation(gkEuler(destOrientation).toQuaternion())
{}


gkOrientationProcess::gkOrientationProcess(gkGameObject* obj,gkScalar time,const gkVector3& startOrientation, const gkVector3& destOrientation)
:	m_object(obj), m_toOnly(false), m_translationTime(time), m_timeCounter(0),
 	m_initalOrientation(gkEuler(startOrientation).toQuaternion()), m_destOrientation(gkEuler(destOrientation).toQuaternion())
{}


gkOrientationProcess::~gkOrientationProcess()
{
}


bool gkOrientationProcess::isFinished()
{
	if (m_timeCounter >= m_translationTime)
		return true;
	else
		return false;
}

void gkOrientationProcess::init()
{
	// if we didn't specify a fixed fromPos we take the current as startPoint
	if (m_toOnly)
		m_initalOrientation = m_object->getOrientation();

	m_timeCounter = 0;
}

void gkOrientationProcess::update(gkScalar delta)
{
	m_timeCounter += delta;

	if (m_object)
	{
		m_object->setOrientation( gkQuaternion::Slerp((m_timeCounter / m_translationTime),m_initalOrientation,m_destOrientation));
	}
}



