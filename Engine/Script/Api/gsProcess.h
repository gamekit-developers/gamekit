/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Thomas Trocha(dertom)

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
#ifndef _gsProcess_h_
#define _gsProcess_h_

#include "Process/gkProcess.h"
#include "Script/Lua/gkLuaUtils.h"
#include "gsCommon.h"
#include "gsProcess.h"
#include "gsCore.h"
#include "gsMath.h"
#include "gkString.h"
#include "Process/gkProcessManager.h"

/** \addtogroup Process
	@{
*/

class gsProcess
#ifndef SWIG
	: public gkProcess
#endif
{
		friend class gsProcessManager;
private:
	gkProcess*  m_process;
	gkLuaEvent* m_init;
	gkLuaEvent* m_update;
	gkLuaEvent* m_isFinished;

public:
#ifndef SWIG
	gsProcess(gkProcess* proc);
#endif
	gsProcess();
	gsProcess(gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish);
	gsProcess(gsSelf self,gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish);
	virtual ~gsProcess();
	// lua class-methods
	gsProcess* setInit(gsSelf self,gsFunction init);
	gsProcess* setUpdate(gsSelf self,gsFunction update);
	gsProcess* setIsFinished(gsSelf self,gsFunction isFinished);
	// lua functions
	gsProcess* setInit(gsFunction init);
	gsProcess* setUpdate(gsFunction update);
	gsProcess* setIsFinished(gsFunction isFinished);

	void setLoopCount(int loopCount);
	int  getLoopCount();
	void suspend();
	void resume();
	bool isSuspended();

	bool isFinished();
	void init();
	void update(gkScalar delta);

	gsProcess* get();
};

class gsProcessManager
{
public:
	gsProcessManager(gkProcessManager* processManager);
	~gsProcessManager();

	void addProcess(gsProcess* process);
	void removeProcess(gsProcess* proc);

	gkProcess* createWait(float time);
	gkProcess* createTranslation(gsGameObject* obj, float time, const gsVector3& to);
	gkProcess* createTranslation(gsGameObject* obj, float time, const gsVector3& from, const gsVector3& to);
	gkProcess* createOrientation(gsGameObject* obj, float time, const gsVector3& toOrientation);
	gkProcess* createOrientation(gsGameObject* obj, float time, const gsVector3& fromOrientation, const gsVector3& toOrientation);
	gkProcess* createParallel(gsArray<gsProcess,gkProcess>& processes,float maxTime=0);
	gkProcess* createSequence(gsArray<gsProcess,gkProcess>& processes,float maxTime=0);
	gkProcess* createSound(const gkString& soundName);
	gkProcessManager* m_processManager;

};

/** @} */


#endif//_gsProcess_h_
