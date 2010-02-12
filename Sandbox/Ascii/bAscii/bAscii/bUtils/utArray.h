/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
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
#ifndef _utArray_h_
#define _utArray_h_


#include "utCommon.h"
#include "utAllocator.h"


// -----------------------------------------------------------------------------
template <typename T>
class utArray
{
public:
	typedef	T* Pointer;
	typedef	const T* ConstPointer;
	typedef	T ValueType;
public:


	utArray(size_t nr, const T&	fill=T()) :
			m_size(0), m_capacity(0), m_data(0)
	{
		reserve(nr);
		for	(size_t	i=0; i<nr; i++)	push_back(fill);
	}

	utArray(size_t nr, T* v) :
			m_size(0), m_capacity(0), m_data(0)
	{
		resize(nr);
		fill(v,	nr);
	}

	utArray(const utArray<T>& o) :
			m_size(o.size()), m_capacity(0), m_data(0)
	{
		reserve(m_size);
		copy(m_data, o.m_data, m_size);
	}

	utArray() :	m_size(0), m_capacity(0), m_data(0)	{}
	~utArray() { clear(); }

	void clear(void)
	{
		if (m_data)
		{
			for	(size_t	i=0; i<m_size; i++)
				m_data[i].~T();

			m_alloc.free(m_data);
		}
		m_data = 0;
		m_capacity = 0;
		m_size = 0;
	}


	size_t find(const T& v)
	{
		for	(size_t	i =	0; i < m_size; i++)
		{
			if (m_data[i] == v)
				return i;
		}
		return UT_NPOS;
	}

	UT_INLINE void push_back(const T& v)
	{
		if (m_size == m_capacity)
			reserve(m_size == 0	? 8	: m_size * 2);
		new	(&m_data[m_size] ) T(v);
		m_size++;
	}


	UT_INLINE void pop_back(void)
	{
		m_size--;
		m_data[m_size].~T();
	}

	void resize(size_t nr, const T&	fill=T())
	{
		if (nr < m_size)
		{
			for	(size_t	i =	m_size;	i <	nr;	i++)
				m_data[i].~T();
		}
		else
		{
			if (nr > m_size)
				reserve(nr);
			for	(size_t	i=m_size;i<nr;i++)
				new	(&m_data[i]) T(fill);
		}

		m_size = nr;
	}


	void reserve(size_t	nr)
	{
		if (m_capacity < nr)
		{
			T* p = (T*)m_alloc.calloc(sizeof(T), nr);
			if (m_data != 0)
			{
				copy(p,	m_data,	m_size);
				m_alloc.free(m_data);
			}
			m_data = p;
			m_capacity = nr;
		}
	}

	UT_INLINE T& operator[](size_t idx)				{ UT_ASSERT(idx	>= 0 &&	idx	< m_capacity); return m_data[idx]; }
	UT_INLINE const	T& operator[](size_t idx) const	{ UT_ASSERT(idx	>= 0 &&	idx	< m_capacity); return m_data[idx]; }
	UT_INLINE T& at(size_t idx)						{ UT_ASSERT(idx	>= 0 &&	idx	< m_capacity); return m_data[idx]; }
	UT_INLINE const	T& at(size_t idx) const			{ UT_ASSERT(idx	>= 0 &&	idx	< m_capacity); return m_data[idx]; }
	UT_INLINE T& front(void)						{ UT_ASSERT(m_size > 0); return	m_data[0]; }
	UT_INLINE T& back(void)							{ UT_ASSERT(m_size > 0); return	m_data[m_size-1]; }

	///	raw	data access
	UT_INLINE ConstPointer	ptr(void) const			{ return m_data; }
	UT_INLINE Pointer		ptr(void)				{ return m_data; }
	UT_INLINE size_t capacity(void)	const			{ return m_capacity; }
	UT_INLINE size_t size(void)	const				{ return m_size; }
	UT_INLINE bool empty(void) const				{ return m_size	== 0;}

	utArray<T> &operator= (const utArray<T>	&o)
	{
		///	avoid if possible
		clear();
		size_t os =	o.size();
		if (os > 0)
		{
			resize(os);
			copy(m_data, o.m_data, os);
		}
		return *this;
	}

	UT_INLINE void copy(Pointer	dst, ConstPointer src, size_t size)
	{
		UT_ASSERT(size <= m_size);
		for	(size_t	i =	0; i < size; i++)
			new	(&dst[i]) T(src[i]);
	}

	UT_INLINE void fill(T *src,	size_t size)
	{
		UT_ASSERT(size <= m_size);
		for	(size_t	i =	0; i < size; i++)
			m_data[i] =	src[i];
	}

protected:
	size_t		m_size;
	size_t		m_capacity;
	Pointer		m_data;
	utAllocator	m_alloc;
};

// -----------------------------------------------------------------------------
template <typename T>
class utArrayIterator
{
public:

	typedef	typename T::Pointer		Iterator;
	typedef	typename T::ValueType&	ValueType;

public:
	utArrayIterator() {}

	utArrayIterator(Iterator begin,	size_t size) : m_iterator(begin), m_cur(0),	m_capacity(size) {}
	utArrayIterator(T& v) :	m_iterator(v.ptr()), m_cur(0), m_capacity(v.size())	{ }
	~utArrayIterator() {}


	UT_INLINE const	utArrayIterator& operator= (const utArrayIterator& o)
	{
		m_iterator = o.m_iterator;
		m_cur =	o.m_cur;
		m_capacity = o.m_capacity;
		return *this;
	}

	UT_INLINE bool hasMoreElements(void)	{ return (m_iterator &&	m_cur <	m_capacity); }
	UT_INLINE ValueType	getNext(void)		{ UT_ASSERT((m_iterator	&& (m_cur) < m_capacity)); return m_iterator[m_cur++]; }
	UT_INLINE void next(void)				{ UT_ASSERT((m_iterator	&& (m_cur) < m_capacity)); ++m_cur;}
	UT_INLINE ValueType	peekNext(void)		{ UT_ASSERT((m_iterator	&& (m_cur) < m_capacity)); return m_iterator[m_cur]; }

protected:
	Iterator	m_iterator;
	size_t		m_cur;
	size_t		m_capacity;
};

#endif//_utArray_h_
