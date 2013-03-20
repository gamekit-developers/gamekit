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

#include "Process/gkProcessManager.h"
#include "utTypes.h"
#include "gkLogger.h"



gkProcessManager::gkProcessManager() : m_pause(false) {

}

gkProcessManager::~gkProcessManager()
{
	clear();
}

void gkProcessManager::clear() {
	m_processList.clear();
}


void gkProcessManager::update(gkScalar delta)
{
	gkProcess* temp;

	if (!m_pause && m_processList.size()>0)
	{

		if (m_processList.size()==0)
			return;

		ProcessList::Iterator iter(m_processList);

		while (iter.hasMoreElements())
		{
			temp = iter.getNext();

			temp->_update(delta);
			if (temp->_isFinished())
				m_processList.erase(temp);
		};
	}
}

void gkProcessManager::addProcess(gkProcess* proc,bool overwrite)
{
	if (m_processList.find(proc))
	{
		if (overwrite)
			proc->_init();
	}
	else
	{
		m_processList.push_back(proc);
		proc->_init();
	}
}

void gkProcessManager::removeProcess(gkProcess* proc)
{
	m_processList.erase(proc);
}


int gkProcessManager::processCount(void)
{
	return m_processList.size();
}

