/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "gsProcess.h"
#include "Process/gkProcessManager.h"
#include "Process/gkWaitProcess.h"


gsProcess::gsProcess() : m_init(0),m_update(0),m_isFinished(0),m_onFinished(0),m_process(0),m_handle(0)
{}

gsProcess::gsProcess(gkProcess* proc) : m_init(0),m_update(0),m_isFinished(0),m_onFinished(0),m_process(proc),m_handle(0)
{
}

gsProcess::gsProcess(gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish) : m_process(0), m_handle(0)
{
	m_init = new gkLuaEvent(init);
	m_update = new gkLuaEvent(update);
	m_isFinished = new gkLuaEvent(isFinished);
	m_onFinished = new gkLuaEvent(onFinish);
}

gsProcess::gsProcess(gsSelf self,gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish) : m_process(0), m_handle(0)
{
	m_init = new gkLuaEvent(self,init);
	m_update = new gkLuaEvent(self,update);
	m_isFinished = new gkLuaEvent(self,isFinished);
	m_onFinished = new gkLuaEvent(self,onFinish);
}

gsProcess::~gsProcess()
{
	if (m_init)
		delete m_init;
	if (m_update)
		delete m_update;
	if (m_isFinished)
		delete m_isFinished;
	if (m_onFinished)
		delete m_onFinished;

	// TODO: should the wrapper really kill the object?
	if (m_process)
		delete m_process;

	gkPrintf("released gsProcess");
}

void gsProcess::setInit(gsSelf self,gsFunction init)
{
	if (m_init){
		delete m_init;
	}
	m_init = new gkLuaEvent(self,init);
}

void gsProcess::setUpdate(gsSelf self,gsFunction update)
{
	if (m_update){
		delete m_init;
	}
	m_update = new gkLuaEvent(self,update);
}

void gsProcess::setIsFinished(gsSelf self,gsFunction isFinished)
{
	if (m_isFinished){
		delete m_isFinished;
	}
	m_isFinished = new gkLuaEvent(self,isFinished);
}

void gsProcess::setOnFinish(gsSelf self,gsFunction onFinished)
{
	if (m_onFinished){
		delete m_onFinished;
	}
	m_onFinished = new gkLuaEvent(self,onFinished);
}


void gsProcess::init()
{
	// is it a wrapped gkProcess?
	if (m_process)
	{
		m_process->init();
		return;
	}

	// is there a lua-function?
	if (m_init)
	{
		m_init->beginCall();
		m_init->addArgument(true);
		m_init->call();
	}
}

void gsProcess::update(gkScalar delta)
{
	// is it a wrapped gkProcess?
	if (m_process)
	{
		m_process->update(delta);
		return;
	}

	// is there a lua-function?
	if (m_update)
	{
		m_update->beginCall();
		m_update->addArgument(delta);
		m_update->call();
	}
}

bool gsProcess::isFinished()
{
	bool result =  true;

	// is it a wrapped gkProcess?
	if (m_process)
	{
		return m_process->isFinished();
	}

	// is there a lua-function?
	if (m_isFinished)
	{
		m_isFinished->beginCall();
		m_isFinished->addArgument(true);
		m_isFinished->call(result);
	}

	return result;
}

void gsProcess::onFinish(bool canceled)
{
	// is it a wrapped gkProcess?
	if (m_process)
	{
		m_process->onFinish(canceled);
		return;
	}

	// is there a lua-function?
	if (m_onFinished)
	{
		m_onFinished->beginCall();
		m_onFinished->addArgument(true);
		m_onFinished->call();
	}
}


void gsProcess::setFollowUp(gsProcess* followUp)
{
	gkProcess::setFollowUp(followUp);
}

void gsProcess::suspend()
{
	setSuspend(true);
}

void gsProcess::resume()
{
	setSuspend(false);
}

bool gsProcess::isSuspended()
{
	return gkProcess::isSuspended();
}

gsProcess* gsProcessManager::getProcessByHandle(int handle)
{
	gkProcess* proc = gkProcessManager::getSingleton().getProcessByHandle(handle);
	gsProcess* gsProc = static_cast<gsProcess*>( proc);
	return gsProc;
}

void gsProcessManager::removeProcess(gsProcess* proc)
{
	gkProcessManager::getSingleton().removeProcess(proc);
}
void gsProcessManager::removeProcessByHandle(int handle)
{
	gkProcessManager::getSingleton().removeProcessByHandle(handle);
}

int gsProcessManager::addProcess(gsProcess* process)
{
	if (process)
	{
//		if (process->m_process)
//			return gkProcessManager::getSingleton().addProcess(process->m_process);
//		else
//			return gkProcessManager::getSingleton().addProcess(process);

		return gkProcessManager::getSingleton().addProcess(process);

	}


	return 0;
}


/*	using %newobject-command in gsProcess.i tells swig that this
	method creates a new object. Swig/Lua will delete it at an appropriate time */
gsProcess* gsProcessManager::waitProcess(float time)
{
	gkWaitProcess* waitProc = new gkWaitProcess(time);
	gsProcess* wrappedProcess = new gsProcess(waitProc);
	return wrappedProcess;
}



