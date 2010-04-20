/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#include "gkThread.h"

#ifdef WIN32
# include "BulletMultiThreaded/Win32ThreadSupport.h"
typedef Win32ThreadSupport          gkThreadSupport;
typedef Win32ThreadFunc             gkThreadFunc;
typedef Win32lsMemorySetupFunc      gklsMemorySetupFunc;

typedef Win32ThreadSupport::Win32ThreadConstructionInfo gkThreadConstruction;

#else
# include "BulletMultiThreaded/PosixThreadSupport.h"
typedef PosixThreadSupport gkThreadSupport;
typedef PosixThreadFunc             gkThreadFunc;
typedef PosixlsMemorySetupFunc      gklsMemorySetupFunc;

typedef PosixThreadSupport::ThreadConstructionInfo gkThreadConstruction;
#endif

gkThreadInterface::gkThreadInterface(const char *uniqueID)
{
	gkThreadConstruction c(const_cast<char*>(uniqueID), entry, memorySetup);

	m_interface = new gkThreadSupport(c);
}

gkThreadInterface::~gkThreadInterface()
{
    delete m_interface;
}

void gkThreadInterface::start(gkAbstractTaskHandler *handler)
{
    m_interface->sendRequest(1, (ppu_address_t)handler, 0);
}

void gkThreadInterface::wait(void)
{
    unsigned int a0, a1;

    m_interface->waitForResponse(&a0, &a1);
}

////////////////////////////////////////////

#ifdef WIN32
gkCriticalSection::gkCriticalSection()
{		
	::InitializeCriticalSection(&m_cs);
}
	
gkCriticalSection::~gkCriticalSection()
{
	::DeleteCriticalSection(&m_cs);
}

void gkCriticalSection::BeginLock()
{
	::EnterCriticalSection(&m_cs);
}

void gkCriticalSection::EndLock()
{
	::LeaveCriticalSection(&m_cs);
}

gkCriticalSection::Lock::Lock(gkCriticalSection& obj)
:m_obj(obj)
{
	::EnterCriticalSection(&m_obj.m_cs);
}

gkCriticalSection::Lock::~Lock()
{	
	::LeaveCriticalSection(&m_obj.m_cs);
}

#else
gkCriticalSection::gkCriticalSection()
{		
	pthread_mutex_init(&m_cs, NULL);
}
	
gkCriticalSection::~gkCriticalSection()
{
	pthread_mutex_destroy(&m_cs);
}

void gkCriticalSection::BeginLock()
{
	pthread_mutex_lock(&m_cs);
}

void gkCriticalSection::EndLock()
{
	pthread_mutex_unlock(&m_cs);
}

gkCriticalSection::Lock::Lock(gkCriticalSection& obj)
:m_obj(obj)
{
	pthread_mutex_lock(&m_obj.m_cs);
}

gkCriticalSection::Lock::~Lock()
{	
	pthread_mutex_unlock(&m_obj.m_cs);
}
#endif

