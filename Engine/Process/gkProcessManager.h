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

class gkProcessManager {

public:
	gkProcessManager();
	virtual ~gkProcessManager();

	void addProcess(gkProcess* proc, bool overwrite=true);

	void removeProcess(gkProcess* proc);

	void update(gkScalar delta);

	int processCount(void);
	gkProcess* getProcessAt(int idx);

	void pause() { m_pause = true;}
	void resume() { m_pause = false;}

	void clear();

private:
	typedef utList<gkProcess*> ProcessList;

	ProcessList m_processList;
	ProcessList m_removeProcessList;
	bool m_pause;
};



#endif /* GKPROCESSMANAGER_H_ */
