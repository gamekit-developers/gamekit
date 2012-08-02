/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2012 Thomas Trocha(dertom)

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
	gkLuaEvent* m_onFinished;

public:
#ifndef SWIG
	gsProcess(gkProcess* proc);
#endif
	gsProcess();
	gsProcess(gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish);
	gsProcess(gsSelf self,gsFunction init,gsFunction update,gsFunction isFinished, gsFunction onFinish);
	virtual ~gsProcess();

	void setInit(gsSelf self,gsFunction init);
	void setUpdate(gsSelf self,gsFunction update);
	void setIsFinished(gsSelf self,gsFunction isFinished);
	void setOnFinish(gsSelf self,gsFunction onFinish);

	void suspend();
	void resume();
	bool isSuspended();

	bool isFinished();
	void init();
	void update(gkScalar delta);
	void onFinish(bool canceled);
	void setFollowUp(gsProcess* followUp);
	gsProcess* proc() { return this;}
};

class gsProcessManager
{
public:
	gsProcessManager(gkProcessManager* processManager);
	~gsProcessManager();

	int addProcess(gsProcess* process);
	gsProcess* getProcessByHandle(int handle);
	void removeProcess(gsProcess* proc);
	void removeProcessByHandle(int handle);

	gkProcess* waitProcess(float time);

	gkProcessManager* m_processManager;

};

#endif//_gsProcess_h_
