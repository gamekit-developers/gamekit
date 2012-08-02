/*
 * gkWaitProcess.cpp
 *
 *  Created on: 04.07.2012
 *      Author: ttrocha
 */

#include "Process/gkWaitProcess.h"
#include "gkLogger.h"

gkWaitProcess::gkWaitProcess(gkScalar timeToWait, gkProcessFunction* func) : m_initialTime(timeToWait),m_timeCounter(timeToWait),m_func(func)
{}

gkWaitProcess::~gkWaitProcess()
{
}


bool gkWaitProcess::isFinished()
{
	if (m_timeCounter <= 0)
		return true;
	else
		return false;
}

void gkWaitProcess::init()
{
	m_timeCounter = m_initialTime;
}
void gkWaitProcess::update(gkScalar delta)
{
	m_timeCounter -= delta;
	if (m_func)
		m_func->update(delta);
	gkPrintf("update %f",m_timeCounter);
}
void gkWaitProcess::onFinish()
{
	gkPrintf("Finished");
}

