/*
 * gkProcess.cpp
 *
 *  Created on: 04.07.2012
 *      Author: ttrocha
 */

#include "Process/gkProcess.h"

gkProcess::gkProcess() : m_deleteAfterFinish(false),m_followUp(0),m_handle(0),m_suspended(false) {}
gkProcess::~gkProcess() {}

bool gkProcess::isFinished(){
	return false;
}

void gkProcess::init(){
//	gkLogger::write("Process init!",true);
}

void gkProcess::update(gkScalar delta){
//	gkLogger::write("Process update!",true);
}

void gkProcess::onFinish(bool cancled) {
//	gkLogger::write("Process finish!",true);
}

void gkProcess::setFollowUp(gkProcess* followUp)
{
	m_followUp = followUp;
}
