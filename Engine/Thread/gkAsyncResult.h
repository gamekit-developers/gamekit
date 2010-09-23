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

#ifndef _gkAsyncResult_h_
#define _gkAsyncResult_h_

#include "gkPtrRef.h"
#include "gkCriticalSection.h"
#include "gkSyncObj.h"


template< typename T >
class gkAsyncResult
{
	template< typename K >
	class gkObj : public gkReferences
	{
	public:

		gkObj() : m_hasValue(0) {}

		~gkObj() {}

		const K& getResult()
		{
			{
				gkCriticalSection::Lock tmp(m_cs);

				--m_hasValue;
			}

			m_syncObj.wait();

			return m_value;
		}

		void setResult(const K& value)
		{
			gkCriticalSection::Lock tmp(m_cs);

			m_value = value;

			++m_hasValue;

			m_syncObj.signal();
		}


		bool hasResult() const { return m_hasValue ? true : false; }

		void reset()
		{
			gkCriticalSection::Lock tmp(m_cs);

			while (m_hasValue > 0)
			{
				--m_hasValue;

				m_syncObj.wait();
			}

			while (m_hasValue < 0)
			{
				++m_hasValue;

				m_syncObj.signal();
			}
		}

	private:

		K m_value;

		int m_hasValue;

		gkSyncObj m_syncObj;

		gkCriticalSection m_cs;
	};

public:

	gkAsyncResult();

	gkAsyncResult(const gkAsyncResult& obj);

	~gkAsyncResult();

	gkAsyncResult& operator = (const gkAsyncResult& obj);

	gkAsyncResult& operator = (const T& value);

	const T& getResult();

	bool hasResult() const;

	void reset();

private:

	gkObj<T>* m_obj;
};

template< typename T >
gkAsyncResult<T>::gkAsyncResult()
	: m_obj(new gkObj<T>)
{
}

template< typename T >
gkAsyncResult<T>::~gkAsyncResult()
{
	m_obj->release();
}

template< typename T >
gkAsyncResult<T>::gkAsyncResult(const gkAsyncResult& obj)
	: m_obj(obj.m_obj)
{
	m_obj->addRef();
}

template< typename T >
const T& gkAsyncResult<T>::getResult()
{
	return m_obj->getResult();
}

template< typename T>
gkAsyncResult<T>&  gkAsyncResult<T>::operator = (const gkAsyncResult& obj)
{
	if (this != &obj)
	{
		obj.m_obj->addRef();

		m_obj->release();

		m_obj = obj.m_obj;
	}

	return *this;
}

template< typename T>
gkAsyncResult<T>& gkAsyncResult<T>::operator = (const T& value)
{
	m_obj->setResult(value);

	return *this;
}

template< typename T>
bool gkAsyncResult<T>::hasResult() const
{
	return m_obj->hasResult();
}

template< typename T>
void gkAsyncResult<T>::reset()
{
	m_obj->reset();
}

#endif //_gkAsyncResult_h_
