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
#ifndef _gkThread_h_
#define _gkThread_h_

#include "gkCommon.h"

#ifdef WIN32
#include <windows.h>
#endif


class gkAbstractTaskHandler
{
public:

    gkAbstractTaskHandler() {}

	virtual ~gkAbstractTaskHandler() {}

    virtual void run() = 0;
};

class btThreadSupportInterface;

class gkThreadInterface
{
protected:

    btThreadSupportInterface *m_interface;

    static void entry(void *ptr, void *lsMem)
    {
        static_cast<gkAbstractTaskHandler*>(ptr)->run();
    }

    static void* memorySetup()
    {
        return 0;
    }

public:

    gkThreadInterface(const char *uniqueID);
    
	~gkThreadInterface();

    void start(gkAbstractTaskHandler *handler);

    void wait(void);
};

class gkNonCopyable
{
protected:
    gkNonCopyable(){};

    ~gkNonCopyable(){};

private:  
	
    gkNonCopyable( const gkNonCopyable& );
    
	const gkNonCopyable& operator=( const gkNonCopyable& );
}; 

class gkCriticalSection : gkNonCopyable
{
public:

	gkCriticalSection();

	~gkCriticalSection();

	class Lock
	{
	public:
		Lock(gkCriticalSection& obj);
		~Lock();

	private:
		gkCriticalSection& m_obj;
	};

	friend class Lock;

	void BeginLock();

	void EndLock();

private:

	#ifdef WIN32
	CRITICAL_SECTION m_cs;
	#else
	pthread_mutex_t m_cs;
	#endif
};

#endif//_gkThread_h_
