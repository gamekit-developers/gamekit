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
#ifndef _gkQueue_h_
#define _gkQueue_h_

#include "gkNonCopyable.h"
#include "gkCriticalSection.h"
#include "gkCommon.h"

template<typename T>
class gkQueue : gkNonCopyable
{
public:

	gkQueue(const gkString& name);

	~gkQueue();

	void push(const T& obj, bool front = false);

	bool pop(T& obj);

	void petitionToFinish();

	bool isEmpty() const;

	void reset();

	void resetButKeepLast();


private:

	gkQueue();

private:

	mutable gkCriticalSection m_cs;

	gkSyncObj m_syncObj;

	std::deque<T> m_queue;

	bool m_petitionToFinish;

	gkString m_name;
};

template< typename T >
gkQueue<T>::gkQueue(const gkString& name)
	: m_name(name)
{
}

template< typename T >
gkQueue<T>::~gkQueue()
{
}

template< typename T >
void gkQueue<T>::push(const T& obj, bool front)
{
	gkCriticalSection::Lock guard(m_cs);

	if (front)
	{
		m_queue.push_front(obj);
	}
	else
	{
		m_queue.push_back(obj);
	}

	m_syncObj.signal();
}

template< typename T >
bool gkQueue<T>::pop(T& obj)
{
	m_syncObj.wait();

	gkCriticalSection::Lock guard(m_cs);

	if (m_queue.empty())
	{
		if (m_petitionToFinish)
		{
			return false;
		}
		else
		{
			GK_ASSERT(false);

			return false;
		}
	}

	obj = m_queue.front();

	m_queue.pop_front();

	return true;
}

template< typename T >
void gkQueue<T>::petitionToFinish()
{
	gkCriticalSection::Lock guard(m_cs);

	m_petitionToFinish = true;

	m_syncObj.signal();
}

template< typename T >
bool gkQueue<T>::isEmpty() const
{
	gkCriticalSection::Lock guard(m_cs);

	return m_queue.empty();
}


template< typename T >
void gkQueue<T>::reset()
{
	gkCriticalSection::Lock guard(m_cs);

	m_queue.clear();
}

template< typename T >
void gkQueue<T>::resetButKeepLast()
{
	gkCriticalSection::Lock guard(m_cs);

	while (m_queue.size() > 1)
	{
		m_syncObj.wait();

		m_queue.pop_front();
	}
}



#endif//_gkQueue_h_
