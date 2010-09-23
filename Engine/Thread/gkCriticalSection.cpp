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
#include "gkCriticalSection.h"
#include "gkCommon.h"

#ifdef WIN32
#include <process.h>
#endif


////////////////////////////////////////////

gkCriticalSection::gkCriticalSection()
{
#ifdef WIN32
	InitializeCriticalSection(&m_cs);
#else
	int failed = pthread_mutex_init(&m_cs, NULL);
	GK_ASSERT(!failed);
#endif
}

gkCriticalSection::~gkCriticalSection()
{
#ifdef WIN32
	DeleteCriticalSection(&m_cs);
#else
	int failed = pthread_mutex_destroy(&m_cs);
	GK_ASSERT(!failed);
#endif
}

void gkCriticalSection::BeginLock()
{
#ifdef WIN32
	EnterCriticalSection(&m_cs);
#else
	int failed = pthread_mutex_lock(&m_cs);
	GK_ASSERT(!failed);
#endif

}

void gkCriticalSection::EndLock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_cs);
#else
	int failed = pthread_mutex_unlock(&m_cs);
	GK_ASSERT(!failed);
#endif
}

gkCriticalSection::Lock::Lock(gkCriticalSection& obj)
	: m_obj(obj)
{
#ifdef WIN32
	EnterCriticalSection(&m_obj.m_cs);
#else
	int failed = pthread_mutex_lock(&m_obj.m_cs);
	GK_ASSERT(!failed);
#endif
}

gkCriticalSection::Lock::~Lock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_obj.m_cs);
#else
	int failed = pthread_mutex_unlock(&m_obj.m_cs);
	GK_ASSERT(!failed);
#endif
}
