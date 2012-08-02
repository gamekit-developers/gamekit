/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2012 Thomas Trocha

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
