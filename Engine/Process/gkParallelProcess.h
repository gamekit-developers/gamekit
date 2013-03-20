/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Thomas Trocha

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

#ifndef GKPARALLELPROCESS_H_
#define GKPARALLELPROCESS_H_

#include "Process/gkProcess.h"
#include "gkMathUtils.h"

class gkParallelProcess : public gkProcess {

public:
	gkParallelProcess(gkScalar maxTime=0);
	virtual ~gkParallelProcess() {}

	void append(gkProcess* childProcess);
	void remove(gkProcess* childProcess);
	void setMasterProcess(gkProcess* masterProcess);

	bool isFinished();
	void init();
	void update(gkScalar delta);


private:
	typedef utList<gkProcess*> ProcessList;

	gkGameObject* m_object;

	gkProcess* m_masterProcess;
	ProcessList m_initalProcessList,m_processList;
	gkScalar m_maxTime;
	gkScalar m_currentTime;

};

#endif /* GKPARALLELPROCESS_H_ */
