/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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
#include "Process/gkTranslationProcess.h"
#include "Process/gkOrientationProcess.h"
#include "Process/gkSequenceProcess.h"
#if OGREKIT_OPENAL_SOUND
#include "Process/gkSoundProcess.h"
#endif

gsProcess::gsProcess() : m_init(0),m_update(0),m_isFinished(0),m_process(0)
{}

gsProcess::gsProcess(gkProcess* proc) : m_init(0),m_update(0),m_isFinished(0),m_process(proc)
{
}

gsProcess::gsProcess(gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish) : m_process(0)
{
	m_init = new gkLuaEvent(init);
	m_update = new gkLuaEvent(update);
	m_isFinished = new gkLuaEvent(isFinished);
}

gsProcess::gsProcess(gsSelf self,gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish) : m_process(0)
{
	m_init = new gkLuaEvent(self,init);
	m_update = new gkLuaEvent(self,update);
	m_isFinished = new gkLuaEvent(self,isFinished);
}

gsProcess::~gsProcess()
{
 	if (m_init)
		delete m_init;
	if (m_update)
		delete m_update;
	if (m_isFinished)
		delete m_isFinished;

	// TODO: should the wrapper really kill the object? Makes absoletly sense,
	//       but the garbage-collector kills from time to time still needed classes.
	//       Therefore you have to keep references...
	if (m_process)
		delete m_process;

}

//set lua-class-functions
#define WRAP_HOOK_SETTER(NAME,LOWERNAME)\
gsProcess* NAME(gsSelf self,gsFunction func)\
{\
	if ( m_##LOWERNAME ){\
		delete m_##LOWERNAME;\
	}\
	m_##LOWERNAME = new gkLuaEvent(self,func);\
	return this;\
}

WRAP_HOOK_SETTER(gsProcess::setInit,init)
WRAP_HOOK_SETTER(gsProcess::setUpdate,update)
WRAP_HOOK_SETTER(gsProcess::setIsFinished,isFinished)

// set lua-functions
#define WRAP_HOOK_SETTER_FUNC(NAME,LOWERNAME)\
gsProcess* NAME(gsFunction func)\
{\
	if ( m_##LOWERNAME ){\
		delete m_##LOWERNAME;\
	}\
	m_##LOWERNAME = new gkLuaEvent(func);\
	return this;\
}

WRAP_HOOK_SETTER_FUNC(gsProcess::setInit,init)
WRAP_HOOK_SETTER_FUNC(gsProcess::setUpdate,update)
WRAP_HOOK_SETTER_FUNC(gsProcess::setIsFinished,isFinished)


void gsProcess::init()
{
	// is it a wrapped gkProcess?
	if (m_process)
	{
		m_process->_init();
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
		m_process->_update(delta);
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

int gsProcess::getLoopCount()
{
	return m_process?m_process->getLoopCount():m_loopCount;
}

void gsProcess::setLoopCount(int loopCount)
{
	if (m_process)
		m_process->setLoopCount(loopCount);
	else
		setLoopCount(loopCount);
}

bool gsProcess::isFinished()
{
	bool result =  true;

	// is it a wrapped gkProcess?
	if (m_process)
	{
		return m_process->_isFinished();
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


gsProcess*  gsProcess::get() {
	return this;
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

gsProcessManager::gsProcessManager(gkProcessManager* procManager) : m_processManager(procManager)
{}

gsProcessManager::~gsProcessManager()
{
	if (m_processManager)
	{
		m_processManager = 0;
	}
}


void gsProcessManager::removeProcess(gsProcess* proc)
{
	m_processManager->removeProcess(proc);
}

void gsProcessManager::addProcess(gsProcess* process)
{
	if (process)
	{
		return m_processManager->addProcess(process);
	}
}

#define RETURN_DEFAULTPROCESS(MSG)\
	gsDebugPrint(gkString("Process-Error::"+MSG).c_str());\
	return new gkWaitProcess(2);

/*	using %newobject-command in gsProcess.i tells swig that this
	method creates a new object. Swig/Lua will delete it at an appropriate time */
gkProcess* gsProcessManager::createWait(float time)
{
	gkWaitProcess* waitProc = new gkWaitProcess(time);
	return waitProc;
}

gkProcess* gsProcessManager::createTranslation(gsGameObject* obj, float time, const gsVector3& to)
{
	if (obj)
	{
		gkTranslationProcess* translationProcess = new gkTranslationProcess(obj->cast<gkGameObject>(),time,gkVector3(to));
		return translationProcess;
	}
	RETURN_DEFAULTPROCESS(gkString("Problem creating Translation-Process!"))
}

gkProcess* gsProcessManager::createTranslation(gsGameObject* obj, float time, const gsVector3& from, const gsVector3& to)
{
	if (obj)
	{
		gkTranslationProcess* translationProcess = new gkTranslationProcess(obj->cast<gkGameObject>(),time,gkVector3(from),gkVector3(to));
		return translationProcess;
	}
	RETURN_DEFAULTPROCESS(gkString("Problem creating Translation-Process!"))
}

gkProcess* gsProcessManager::createOrientation(gsGameObject* obj, float time, const gsVector3& from, const gsVector3& to)
{
	if (obj)
	{
		gkOrientationProcess* orientationProcess = new gkOrientationProcess(obj->cast<gkGameObject>(),time,gkVector3(from),gkVector3(to));
		return orientationProcess;
	}
	RETURN_DEFAULTPROCESS(gkString("Problem creating Orientation-Process!"))
}

gkProcess* gsProcessManager::createOrientation(gsGameObject* obj, float time,  const gsVector3& to)
{
	if (obj)
	{
		gkOrientationProcess* orientationProcess = new gkOrientationProcess(obj->cast<gkGameObject>(),time,gkVector3(to));
		return orientationProcess;
	}
	RETURN_DEFAULTPROCESS(gkString("Problem creating Orientation-Process!"))
}

gkProcess* gsProcessManager::createParallel(gsArray<gsProcess,gkProcess>& processes,float maxTime)
{
	gkParallelProcess* parallelProc = new gkParallelProcess(maxTime);

	for (int i=0;i<processes.size();i++) {
		gkProcess* proc = processes.at(i);
		if (proc)
			parallelProc->append(proc);
	}

	return parallelProc;
}

gkProcess* gsProcessManager::createSequence(gsArray<gsProcess,gkProcess>& processes,float maxTime)
{
	gkSequenceProcess* seqProcesses = new gkSequenceProcess(maxTime);

	for (int i=0;i<processes.size();i++) {
		gkProcess* proc = processes.at(i);
		if (proc)
			seqProcesses->append(proc);
	}

	return seqProcesses;
}



gkProcess* gsProcessManager::createSound(const gkString& soundName)
{
#ifdef OGREKIT_OPENAL_SOUND
	if (!soundName.empty())
	{
		gkSoundProcess* sound = new gkSoundProcess(soundName);

		if (!sound || !sound->hasValidSound())
			RETURN_DEFAULTPROCESS(gkString("Problem creating Sound-Process with name:"+soundName))

		return sound;
	}
	RETURN_DEFAULTPROCESS(gkString("Problem creating Sound-Process with empty name"))
#else
	// return null-pointer so you can check for nil in lua
	return 0;
#endif
}
