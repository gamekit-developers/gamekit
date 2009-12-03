/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
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
#ifndef _gkStack_h_
#define _gkStack_h_

#include "gkCommon.h"



// ----------------------------------------------------------------------------
template < typename T, size_t space= 64 >
class gkStack
{
public:
	typedef T* Pointer;
	typedef const T* ConstPointer;
	typedef T ValueType;

public:

	gkStack(size_t rsp= space) :
			m_size(0), m_capacity(0), m_top(0), m_stack(0)
	{
		reserve(rsp);
	}


	gkStack(const gkStack& o) :
			m_size(o.m_size), m_capacity(0), m_top(o.m_top), m_stack(0)
	{
		reserve(o.m_size);
		GK_ASSERT(m_stack && o.m_stack);
		copy(m_stack, o.m_stack, o.m_size);
	}

	~gkStack() { clear(); }

	void clear(void)
	{
		if (m_stack != 0)
		{
			delete []m_stack;
			m_stack= 0;
		}

		m_size= 0;
		m_capacity= 0;
		m_top= 0;
	}


	void push(const T& v)
	{
		GK_ASSERT(m_top != GK_NPOS);

		if (m_size == m_capacity)
			reserve(m_size == 0 ? 8 : m_size * 2);

		m_stack[m_top]= v;
		++m_top;
		++m_size;
	}

	GK_INLINE T& pop(void)
	{
		GK_ASSERT(m_top != 0);
		--m_size;
		return m_stack[--m_top];
	}

	GK_INLINE T& top(void)
	{
		GK_ASSERT(m_top != 0);
		return m_stack[(m_top - 1)];
	}

	GK_INLINE const T& top(void) const
	{
		GK_ASSERT(m_top != 0);
		return m_stack[(m_top - 1)];
	}

	GK_INLINE T& peek(size_t offs)
	{
		GK_ASSERT(m_top != 0 && ((m_top - 1) - offs) != GK_NPOS);
		return m_stack[(m_top - 1)-offs];
	}

	GK_INLINE const T& peek(size_t offs) const
	{
		GK_ASSERT(m_top != 0 && ((m_top - 1) - offs) != GK_NPOS);
		return m_stack[(m_top - 1)-offs];
	}

	void reserve(size_t nr)
	{
		if (m_capacity < nr)
		{
			Pointer temp= new T[nr];
			if (m_stack)
			{
				copy(temp, m_stack, m_size);
				delete []m_stack;
			}
			m_stack= temp;
			m_capacity= nr;
		}
	}

	GK_INLINE size_t capacity(void) const		   { return m_capacity; }
	GK_INLINE size_t size(void) const			   { return m_size; }
	GK_INLINE size_t itop(void) const			   { return m_top; }
	GK_INLINE bool empty(void) const				{ return m_size == 0; }
	GK_INLINE ConstPointer begin(void) const		{ return m_stack; }
	GK_INLINE Pointer begin(void)				   { return m_stack; }

protected:

	GK_INLINE void copy(Pointer dest, const Pointer src, size_t nr)
	{
		size_t i;
		for (i= 0; i < nr; i++)
			dest[i]= src[i];
	}


	size_t  m_size;		 /// current nr of elements in the stack
	size_t  m_capacity;	 /// allocated stack space
	size_t  m_top;		  /// current top element
	Pointer m_stack;		/// pointer to pointer of T

};


///////////////////////////////////////////////////////////////////////////////
template <typename T>
class gkStackIterator
{
public:
	typedef typename T::Pointer   Iterator;
	typedef typename T::ValueType ValueType;

public:

	gkStackIterator(Iterator begin, size_t top) : m_iterator(begin), m_top(top), m_capacity(top) {}
	gkStackIterator(T& v) : m_iterator(v.begin()), m_top(v.itop()), m_capacity(v.itop()) {}

	~gkStackIterator() {}

	GK_INLINE bool hasMoreElements(void)
	{
		return (m_iterator && m_top != 0 && m_top != GK_NPOS);
	}

	GK_INLINE ValueType getNext(void)
	{
		GK_ASSERT((m_iterator && (m_top - 1) != GK_NPOS));
		--m_top;
		return m_iterator[m_top+1];
	}

	GK_INLINE void next(void)
	{
		GK_ASSERT((m_iterator && (m_top - 1) != GK_NPOS));
		--m_top;
	}


	GK_INLINE ValueType peekNext(void)
	{
		GK_ASSERT((m_iterator && (m_top) != GK_NPOS));
		return m_iterator[m_top];
	}

protected:
	Iterator		m_iterator;		 // pointer access to stack
	size_t		  m_top;			  // top of stack
	const size_t	m_capacity;		// initial top
};




#endif//_gkStack_h_
