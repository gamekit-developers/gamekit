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
#ifndef _gkPtrRef_h_
#define _gkPtrRef_h_

#include "gkCommon.h"
#include "gkCriticalSection.h"

class gkReferences
{
public:

	gkReferences();

	virtual ~gkReferences();

	int addRef();

	int release();

	int getReferences() const;

private:

	int m_references;

	mutable gkCriticalSection m_cs;
};

template< class T >
class gkPtrRef
{
public:

	explicit gkPtrRef(T* p = 0)
		: m_obj(p)
	{
	}

	~gkPtrRef()
	{
		if (m_obj)
		{
			m_obj->release();
		}
	}

	gkPtrRef(const gkPtrRef& obj)
		: m_obj(obj.m_obj)
	{
		if (m_obj)
		{
			m_obj->addRef();
		}
	}

	gkPtrRef& operator = (const gkPtrRef& obj)
	{
		if (this != &obj)
		{
			if (obj.m_obj)
			{
				obj.m_obj->addRef();
			}

			if (m_obj)
			{
				m_obj->release();
			}

			m_obj = obj.m_obj;
		}

		return *this;
	}

	T* get() const
	{
		return m_obj;
	}


	T& operator*() const
	{
		GK_ASSERT(m_obj);

		return *m_obj;
	}

	T* operator->() const
	{
		GK_ASSERT(m_obj);

		return m_obj;
	}

private:

	T* m_obj;
};

#endif//_gkPtrRef_h_
