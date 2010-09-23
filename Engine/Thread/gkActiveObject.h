/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) Nestor Silveira.

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
#ifndef _gkActiveObject_h_
#define _gkActiveObject_h_

#include "gkNonCopyable.h"
#include "gkThread.h"
#include "gkQueue.h"
#include "gkSyncObj.h"
#include "gkPtrRef.h"

class gkActiveObject : public gkCall, gkNonCopyable
{
public:

	gkActiveObject(const gkString& name);

	virtual ~gkActiveObject();

	void enqueue(gkPtrRef<gkCall> call, bool front = false);

	void join();

	void reset();

	void resetButKeepLast();

	bool isEmpty() const;

	void run();

private:

	gkString m_name;

	gkThread* m_thread;

	gkQueue<gkPtrRef<gkCall> > m_queue;

	gkSyncObj m_syncObj;
};

#endif//_gkActiveObject_h_
