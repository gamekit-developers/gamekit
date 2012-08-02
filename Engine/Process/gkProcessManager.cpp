/*
 * gkProcessManager.cpp
 *
 *  Created on: 04.07.2012
 *      Author: ttrocha
 */

#include "Process/gkProcessManager.h"
#include "utTypes.h"
#include "gkLogger.h"



gkProcessManager::gkProcessManager() : m_pause(false), m_handleCounter(0) {

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
		// remove stopped process
		utListIterator<ProcessList> iter(m_removeProcessList);
		while (iter.hasMoreElements())
		{
			gkProcess* proc = iter.getNext();
			// call finish-hook (canceled-finish)
			proc->onFinish(true);
			m_processList.erase(proc,false);
		}

		if (m_processList.size()==0)
			return;
		// update all processes
		utList<gkProcess*>::Pointer start = m_processList.begin();
		utList<gkProcess*>::Pointer end = m_processList.end();

		while(1)
		{
			temp = start->getLink();

			// skip all this when the process is suspended
			if (!temp->isSuspended())
			{
				temp->update(delta);
				if (temp->isFinished())
				{
					// call finish-hook (normal finish)
					temp->onFinish(false);
					if (temp->getFollowUp())
					{
						// replace the finished process by its followup and set the handler
						// that let us keep track of this process-chain
						start->link=temp->getFollowUp();
						start->link->init();
						start->link->m_handle=temp->m_handle;
					}
					else
					{
						m_processList.erase(temp,false);
					}
					if (temp->deleteAfterFinish()){
						delete temp;
					}
				}

			}


			// reached the end?
			if (start==end)
			{
				break;
			}
			start = start->getNext();

		};
	}
}

int gkProcessManager::addProcess(gkProcess* proc)
{
	int procHandle = m_handleCounter++;
	proc->m_handle = procHandle;
	m_processList.push_back(proc);
	proc->init();
	return 0;
}

gkProcess* gkProcessManager::getProcessByHandle(UTint32 handle)
{
	utListIterator<ProcessList> iter(m_processList);
	while (iter.hasMoreElements())
	{
		gkProcess* proc = iter.getNext();
		if (proc->getHandle() == handle)
		{
			return proc;
		}
	}
	return NULL;
}

void gkProcessManager::removeProcess(gkProcess* proc)
{
	m_processList.erase(proc);
}

void gkProcessManager::removeProcessByHandle(UTint32 handle)
{
	utListIterator<ProcessList> iter(m_processList);
	while (iter.hasMoreElements())
	{
		gkProcess* proc = iter.getNext();
		if (proc->getHandle() == handle)
		{
			m_removeProcessList.push_back(proc);
			break;
		}
	}
}

int gkProcessManager::processCount(void)
{
	return m_processList.size();
}

