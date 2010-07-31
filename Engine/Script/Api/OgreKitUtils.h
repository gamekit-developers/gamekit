/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _OgreKitUtils_h_
#define _OgreKitUtils_h_

#include "OgreKitCommon.h"

namespace OgreKit
{

// Referenced counted ptr for freeing script objects with lua's gc
// adapted from gkPtrRef.h
// Rule of thumb is to have a Pointer<T> On classes that return * from members
// this allows the returned object to be deleted without specal user code.
template<class T>
class Pointer
{

public:
#ifndef SWIG
	// internal

	explicit Pointer(T *p = 0)
		:   m_obj(p), m_ref(0)
	{
		addRef();
	}

	~Pointer()
	{
		release();
	}

	Pointer(const Pointer &obj)
		:   m_obj(obj.m_obj), m_ref(0)
	{
		addRef();
	}

	Pointer &operator = (const Pointer &obj)
	{
		if(this != &obj)
		{
			obj.addRef();
			release();
			m_obj = obj.m_obj;
			m_ref = obj.m_ref;
		}
		return *this;
	}

#endif


	bool isNull(void) const                         { return m_obj == 0; }

	bool operator ==(const Pointer &rhs) const      { return m_obj == rhs.m_obj; }
	bool operator !=(const Pointer &rhs) const      { return m_obj != rhs.m_obj; }

	T *operator->() const
	{
		GK_ASSERT(m_obj);
		return m_obj;
	}


private:

	void addRef(void) const
	{
		if (m_obj)
			++m_ref;
	}

	void release(void) const
	{
		if(m_obj)
		{
			if (--m_ref == 0)
			{
				delete m_obj;
				m_obj = 0;
			}
		}
	}

	mutable T   *m_obj;
	mutable int  m_ref;
};




}

#endif//_OgreKitUtils_h_
