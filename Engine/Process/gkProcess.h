/*
 * gkProcess.h
 *
 *  Created on: 04.07.2012
 *      Author: ttrocha
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
