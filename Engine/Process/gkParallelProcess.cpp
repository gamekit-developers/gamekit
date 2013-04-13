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

#include "Process/gkParallelProcess.h"
#include "gkLogger.h"
#include "gkGameObject.h"
#include "gkValue.h"


gkParallelProcess::gkParallelProcess(gkScalar maxTime)
	:	gkProcess(), m_masterProcess(0), m_currentTime(0),m_maxTime(maxTime),m_object(0)
{}

void gkParallelProcess::append(gkProcess* childProcess)
{
	if (childProcess && m_initalProcessList.find(childProcess)==0)
	{
		m_initalProcessList.push_back(childProcess);
	} else {
		gkLogger::write("Tried to apply a process to gkParallelProcess that was already in the processlist! This is not allowed!",true);
	}
}
void gkParallelProcess::remove(gkProcess* childProcess)
{
	if (childProcess)
	{
		m_initalProcessList.erase(childProcess);
	}
}
void gkParallelProcess::setMasterProcess(gkProcess* masterProcess)
{
	m_masterProcess = masterProcess;
}

void gkParallelProcess::init()
{
	m_currentTime = 0;
	// reinit process-list
	m_processList.clear();
	for (UTsize i=0;i<m_initalProcessList.size();i++)
	{
		gkProcess* proc = m_initalProcessList.at(i);
		proc->init();
		m_processList.push_back(proc);
	}

}

void gkParallelProcess::update(gkScalar delta)
{
	m_currentTime += delta;

	ProcessList::Iterator iter(m_processList);

	int count=0;
	while (iter.hasMoreElements())
	{
		count++;
		gkProcess* proc = iter.getNext();
		proc->update(delta);

		if (proc->isFinished())
			m_processList.erase(proc);
	}

}

bool gkParallelProcess::isFinished()
{
	if (m_processList.size()==0 ||
		(m_maxTime!=0 && m_currentTime >= m_maxTime) ||
		(m_masterProcess && m_masterProcess->isFinished()))
		return true;
	else
		return false;
}

