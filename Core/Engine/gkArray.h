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
#ifndef _gkArray_h_
#define _gkArray_h_


#include "gkCommon.h"





// ----------------------------------------------------------------------------
template <typename T>
class gkArray
{
public:
	typedef T* Pointer;
	typedef const T* ConstPointer;
	typedef T ValueType;
public:

	gkArray(const T& v, size_t nr) :
			m_size(0), m_capacity(0), m_data(0)
	{
		reserve(nr);
		for (size_t i= 0; i < nr; i++)
			push_back(v);
	}

	gkArray(size_t nr) :
			m_size(0), m_capacity(0), m_data(0)
	{
		reserve(nr);
	}

	gkArray(const gkArray& o) :
			m_size(o.size()), m_capacity(0), m_data(0)
	{

		/// avoid if possible
		reserve(m_size);
		copy(m_data, o.m_data, m_size);
	}

	gkArray() : m_size(0), m_capacity(0), m_data(0) {}
	~gkArray() { clear(); }

	void clear(void)
	{
		if (m_data)
			(delete [](m_data));

		m_data= 0;
		m_capacity= 0;
		m_size= 0;
	}

	size_t find(const T& v)
	{
		for (size_t i= 0; i < m_size; i++)
			if (m_data[i] == v)
				return i;
		return GK_NPOS;
	}


	GK_INLINE void push_back(const T& v)
	{
		if (m_size == m_capacity)
			reserve(m_size  == 0 ? 8 : m_size * 2);

		m_data[m_size]= v;
		m_size++;
	}


	GK_INLINE void pop_back(void)
	{
		m_size--;
		m_data[m_size].~T();
	}


	GK_INLINE T& front(void)
	{
		GK_ASSERT(m_size > 0);
		return m_data[0];
	}

	GK_INLINE T& back(void)
	{
		GK_ASSERT(m_size > 0);
		return m_data[m_size-1];
	}

	void resize(size_t nr)
	{
		if (nr < m_size)
		{
			for (size_t i= m_size; i < nr; i++)
				m_data[i].~T();
		}
		else
		{
			if (nr > m_size)
				reserve(nr);
		}
		m_size= nr;
	}


	void reserve(size_t nr)
	{
		if (m_capacity < nr)
		{
			T* p= new T[nr];
			if (m_data != 0)
			{
				copy(p, m_data, m_size);
				delete []m_data;
			}

			m_data= p;
			m_capacity= nr;
		}
	}

	size_t remove(const T& v)
	{
		size_t i= find(v);
		if (i != GK_NPOS && i < m_size)
		{
			swap(i, m_size - 1);
			pop_back();
		}
		return i;
	}

	GK_INLINE T& operator[](size_t idx)			 { GK_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	GK_INLINE const T& operator[](size_t idx) const { GK_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	GK_INLINE T& at(size_t idx)					 { GK_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	GK_INLINE const T& at(size_t idx) const		 { GK_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	GK_INLINE void swap(size_t a, size_t b)		 { UT_Swap<T>(m_data[a], m_data[b]); }

	/// raw data access
	GK_INLINE ConstPointer  ptr(void) const		 { return m_data; }
	GK_INLINE Pointer	   ptr(void)			   { return m_data; }
	GK_INLINE void expand(size_t nr)				{ reserve(m_size + nr); }
	GK_INLINE size_t capacity(void) const		   { return m_capacity; }
	GK_INLINE size_t size(void) const			   { return m_size; }
	GK_INLINE bool empty(void) const				{ return m_size == 0;}


	gkArray<T> &operator= (const gkArray<T> &o)
	{

		/// avoid if possible
		clear();
		size_t os= o.size();
		if (os > 0)
		{
			resize(os);
			copy(m_data, o.m_data, os);
		}
		return *this;
	}


	GK_INLINE void copy(Pointer dst, ConstPointer src, size_t size)
	{
		GK_ASSERT(size <= m_size);
		for (size_t i= 0; i < size; i++)
			dst[i]= src[i];
	}

protected:

	size_t  m_size;		 /// nr of elements in the array
	size_t  m_capacity;	 /// maximum space allocated
	Pointer m_data;		 /// data access
};


// ----------------------------------------------------------------------------
template <typename T>
class gkArrayIterator
{
public:

	typedef typename T::Pointer	 Iterator;
	typedef typename T::ValueType&  ValueType;

public:
	gkArrayIterator() {}

	gkArrayIterator(Iterator begin, size_t size) :
			m_iterator(begin), m_cur(0), m_capacity(size)
	{
	}

	gkArrayIterator(T& v) :
			m_iterator(v.ptr()), m_cur(0), m_capacity(v.size())
	{
	}


	~gkArrayIterator() {}


	GK_INLINE const gkArrayIterator& operator= (const gkArrayIterator& o)
	{
		m_iterator= o.m_iterator;
		m_cur= o.m_cur;
		m_capacity= o.m_capacity;
		return *this;
	}

	GK_INLINE bool hasMoreElements(void)
	{
		return (m_iterator && m_cur < m_capacity);
	}


	GK_INLINE ValueType getNext(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur++];
	}

	GK_INLINE void next(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		++m_cur;
	}


	GK_INLINE ValueType peekNext(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur];
	}

protected:
	Iterator	m_iterator;		 // pointer access to stack
	size_t	  m_cur;			  // current index
	size_t	  m_capacity;		 // initial capacity
};




#endif//_gkArray_h_
