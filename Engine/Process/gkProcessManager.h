/*
 * gkProcessManager.h
 *
 *  Created on: 04.07.2012
 *      Author: ttrocha
 */

#ifndef GKPROCESSMANAGER_H_
#define GKPROCESSMANAGER_H_

#include "gkMathUtils.h"
#include "Process/gkProcess.h"
#include "utTypes.h"
#include "Process/gkProcessManager.h"
#include "utSingleton.h"

class gkProcessFunction
{
public:
	gkProcessFunction() {}
	virtual ~gkProcessFunction() {}
	virtual void update(gkScalar delta){};
};

class gkProcessManager: public utSingleton<gkProcessManager> {

public:
	gkProcessManager();
	virtual ~gkProcessManager();

	int addProcess(gkProcess* proc);

	gkProcess* getProcessByHandle(UTint32 handle);

	void removeProcess(gkProcess* proc);
	void removeProcessByHandle(UTint32 handle);

	void update(gkScalar delta);

	int processCount(void);

	void pause() { m_pause = true;}
	void resume() { m_pause = false;}

	void clear();

private:
	typedef utList<gkProcess*> ProcessList;

	ProcessList m_processList;
	ProcessList m_removeProcessList;
	bool m_pause;
	UTint32 m_handleCounter;

    UT_DECLARE_SINGLETON(gkProcessManager);
};



#endif /* GKPROCESSMANAGER_H_ */
