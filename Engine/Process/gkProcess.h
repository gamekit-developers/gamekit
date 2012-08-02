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

#ifndef _gkProcess_h_
#define _gkProcess_h_
#include "gkMathUtils.h"

class gkProcess {
	friend class gkProcessManager;
public:
	gkProcess();
	virtual ~gkProcess();

	virtual bool isFinished();
	void setFollowUp(gkProcess* followUp);
	gkProcess* getFollowUp(void){ return m_followUp;}
	virtual void init();
	virtual void update(gkScalar delta);
	virtual void onFinish(bool canceled);
	GK_INLINE bool deleteAfterFinish() { return m_deleteAfterFinish;}
	GK_INLINE UTint32 getHandle(void) { return m_handle; }
	GK_INLINE bool isSuspended(void) { return m_suspended;}
	GK_INLINE void setSuspend(bool suspend) { m_suspended = suspend; }

protected:
	bool m_deleteAfterFinish;
	gkProcess* m_followUp;
	UTint32 m_handle;
	bool m_suspended;
};


#endif /* GKPROCESS_H_ */
