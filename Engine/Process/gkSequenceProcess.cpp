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

#include "Process/gkSequenceProcess.h"
#include "gkLogger.h"
#include "gkGameObject.h"
#include "gkValue.h"


gkSequenceProcess::gkSequenceProcess(gkScalar maxTime)
	:	gkProcess(),  m_currentTime(0),m_maxTime(maxTime), m_currentProcessNr(0), m_currentProcess(0),m_isFinished(false)
{}



void gkSequenceProcess::append(gkProcess* childProcess)
{
	if (childProcess)
	{
		m_processList.push_back(childProcess);
	}
}
void gkSequenceProcess::remove(gkProcess* childProcess)
{
	if (childProcess)
	{
		m_processList.erase(childProcess);
	}
}


void gkSequenceProcess::init()
{
	m_isFinished = false;
	m_currentTime = 0;
	m_currentProcessNr = 0;
	m_currentProcess = m_processList.at(0);
	m_currentProcess->init();
}

void gkSequenceProcess::update(gkScalar delta)
{
	if (!m_isFinished)
	{
		m_currentTime += delta;

		m_currentProcess->update(delta);
	}

}

bool gkSequenceProcess::isFinished()
{
	if (m_currentProcess->isFinished()) {
		m_currentProcessNr++;
		// did we reach the end of the sequential list?
		if (m_currentProcessNr >= (int)m_processList.size()){
			return true;
		} else {
			m_currentProcess = m_processList.at(m_currentProcessNr);
			m_currentProcess->init();
		}
	}

	// did we ran out of time?
	if (m_maxTime!=0 && m_currentTime >= m_maxTime)
		return true;
	else
		return false;
}





