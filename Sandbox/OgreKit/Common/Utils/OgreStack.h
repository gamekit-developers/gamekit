/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _OgreStack_h_
#define _OgreStack_h_

#include "OgreUtilsCommon.h"
#include "OgreMemUtils.h"

namespace Ogre
{


template < typename T, size_t space = 64 >
class Stack
{
public:
	typedef T* Pointer;
	typedef const T* ConstPointer;
	typedef T ValueType;

public:

	Stack(size_t rsp = space) : m_size(0), m_capacity(0), m_top(0), m_stack(0)
	{
		reserve(rsp);
	}


	Stack(const Stack& o) :
			m_size(o.m_size), m_capacity(0), m_top(o.m_top), m_stack(0)
	{
		reserve(o.m_size);
		OGRE_ASSERT(m_stack && o.m_stack);
		copy(m_stack, o.m_stack, o.m_size);
	}

	~Stack() { clear(); }

	void clear(void)
	{
		if (m_stack != 0)
		{
			for (size_t i = 0; i < m_size; i++)
				m_stack[i].~T();
			utFree(m_stack);
		}
		m_size = 0;
		m_capacity = 0;
		m_top = 0;
	}


	void push(const T& v)
	{
		OGRE_ASSERT(m_top != OGRE_NPOS);

		if (m_size == m_capacity)
			reserve(m_size == 0 ? 8 : m_size * 2);

		m_stack[m_top] = v;
		++m_top;
		++m_size;
	}

	OGRE_INLINE T& pop(void)
	{
		OGRE_ASSERT(m_top != 0);
		--m_size;
		return m_stack[--m_top];
	}

	OGRE_INLINE T& top(void)
	{
		OGRE_ASSERT(m_top != 0);
		return m_stack[(m_top - 1)];
	}

	OGRE_INLINE const T& top(void) const
	{
		OGRE_ASSERT(m_top != 0);
		return m_stack[(m_top - 1)];
	}

	OGRE_INLINE T& peek(size_t offs)
	{
		OGRE_ASSERT(m_top != 0 && ((m_top - 1) - offs) != OGRE_NPOS);
		return m_stack[(m_top - 1)-offs];
	}

	OGRE_INLINE const T& peek(size_t offs) const
	{
		OGRE_ASSERT(m_top != 0 && ((m_top - 1) - offs) != OGRE_NPOS);
		return m_stack[(m_top - 1)-offs];
	}

	void reserve(size_t nr)
	{
		if (m_capacity < nr)
		{
			T* p = (T*)utCalloc(sizeof(T), nr);
			if (m_stack != 0)
			{
				copy(p, m_stack, m_size);
				utFree(m_stack);
			}
			m_stack = p;
			m_capacity = nr;
		}
	}

	OGRE_INLINE size_t capacity(void) const		   { return m_capacity; }
	OGRE_INLINE size_t size(void) const			   { return m_size; }
	OGRE_INLINE size_t itop(void) const			   { return m_top; }
	OGRE_INLINE bool empty(void) const				{ return m_size == 0; }
	OGRE_INLINE ConstPointer begin(void) const		{ return m_stack; }
	OGRE_INLINE Pointer begin(void)				   { return m_stack; }

protected:

	OGRE_INLINE void copy(Pointer dst, const Pointer src, size_t nr)
	{
		size_t i;
		for (i = 0; i < nr; i++)
			new(&dst[i]) T(src[i]);
	}


	size_t  m_size;
	size_t  m_capacity;
	size_t  m_top;
	Pointer m_stack;

};

template <typename T>
class StackIterator
{
public:
	typedef typename T::Pointer   Iterator;
	typedef typename T::ValueType ValueType;

public:

	StackIterator(Iterator begin, size_t top) : m_iterator(begin), m_top(top), m_capacity(top) {}
	StackIterator(T& v) : m_iterator(v.begin()), m_top(v.itop()), m_capacity(v.itop()) {}

	~StackIterator() {}


	OGRE_INLINE bool hasMoreElements(void) { return (m_iterator && m_top != 0 && m_top != OGRE_NPOS); }

	OGRE_INLINE ValueType getNext(void)
	{
		OGRE_ASSERT((m_iterator && (m_top - 1) != OGRE_NPOS));
		--m_top;
		return m_iterator[m_top+1];
	}

	OGRE_INLINE void next(void)
	{
		OGRE_ASSERT((m_iterator && (m_top - 1) != OGRE_NPOS));
		--m_top;
	}


	OGRE_INLINE ValueType peekNext(void)
	{
		OGRE_ASSERT((m_iterator && (m_top) != OGRE_NPOS));
		return m_iterator[m_top];
	}

protected:
	Iterator m_iterator;
	size_t m_top;
	const m_capacity;
};

}

#endif//_OgreStack_h_
