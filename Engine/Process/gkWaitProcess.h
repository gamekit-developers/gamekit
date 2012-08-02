/*
 * gkWaitProcess.h
 *
 *  Created on: 04.07.2012
 *      Author: ttrocha
 */

#ifndef GKWAITPROCESS_H_
#define GKWAITPROCESS_H_

#include "Process/gkProcess.h"
#include "gkMathUtils.h"

class gkWaitProcess : public gkProcess {

public:
	gkWaitProcess(gkScalar timeToWait,gkProcessFunction* func=0);
	virtual ~gkWaitProcess();
	bool isFinished();
	void init();
	void update(gkScalar delta);
	void onFinish();
private:
	gkScalar m_initialTime;
	gkScalar m_timeCounter;
	gkProcessFunction* m_func;
};

#endif /* GKWAITPROCESS_H_ */
