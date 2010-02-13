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
#ifndef _OgreArray_h_
#define _OgreArray_h_


#include "OgreUtilsCommon.h"
#include "OgreMemUtils.h"

namespace Ogre
{


template <typename T>
class Array
{
public:
    typedef T*          Pointer;
    typedef const T*    ConstPointer;
    typedef T           ValueType;
public:

    Array(size_t nr) :
            m_size(0), m_capacity(0), m_data(0)
    {
        reserve(nr);
    }

    Array(size_t nr, T* v) :
            m_size(0), m_capacity(0), m_data(0)
    {
        resize(nr);
        fill(v, nr);
    }

    Array(const Array<T>& o) :
            m_size(o.size()), m_capacity(0), m_data(0)
    {
        reserve(m_size);
        copy(m_data, o.m_data, m_size);
    }

    Array() : m_size(0), m_capacity(0), m_data(0) {}
    ~Array() { clear(); }

    void clear(void)
    {
        if (m_data)
        {
            for (size_t i = 0; i < m_size; i++)
                m_data[i].~T();

            utFree(m_data);
        }
        m_data = 0;
        m_capacity = 0;
        m_size = 0;
    }


    size_t find(const T& v)
    {
        for (size_t i = 0; i < m_size; i++)
        {
            if (m_data[i] == v)
                return i;
        }
        return OGRE_NPOS;
    }

    OGRE_INLINE void push_back(const T& v)
    {
        if (m_size == m_capacity)
            reserve(m_size == 0 ? 8 : m_size * 2);
        new(&m_data[m_size]) T(v);
        m_size++;
    }


    OGRE_INLINE void pop_back(void)
    {
        m_size--;
        m_data[m_size].~T();
    }

    void resize(size_t nr, const T& fill=T())
    {
        if (nr < m_size)
        {
            for (size_t i = m_size; i < nr; i++)
                m_data[i].~T();
        }
        else
        {
            if (nr > m_size)
                reserve(nr);

			for (size_t i=m_size; i<nr; i++)
				 new(&m_data[i]) T(fill);
        }

        m_size = nr;
    }


    void reserve(size_t nr)
    {
        if (m_capacity < nr)
        {
            T* p = (T*)utCalloc(sizeof(T), nr);
            if (m_data != 0)
            {
                copy(p, m_data, m_size);
                utFree(m_data);
            }
            m_data = p;
            m_capacity = nr;
        }
    }

    OGRE_INLINE T& operator[](size_t idx)                 { OGRE_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    OGRE_INLINE const T& operator[](size_t idx) const     { OGRE_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    OGRE_INLINE T& at(size_t idx)                         { OGRE_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    OGRE_INLINE const T& at(size_t idx) const             { OGRE_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    OGRE_INLINE T& front(void)                            { OGRE_ASSERT(m_size > 0); return m_data[0]; }
    OGRE_INLINE T& back(void)                             { OGRE_ASSERT(m_size > 0); return m_data[m_size-1]; }

    ///    raw    data access
    OGRE_INLINE ConstPointer ptr(void) const      { return m_data; }
    OGRE_INLINE Pointer ptr(void)                 { return m_data; }
    OGRE_INLINE size_t capacity(void) const       { return m_capacity; }
    OGRE_INLINE size_t size(void) const           { return m_size; }
    OGRE_INLINE bool empty(void) const            { return m_size == 0;}

    Array<T> &operator= (const Array<T> &o)
    {
        ///    avoid if possible
        clear();
        size_t os = o.size();
        if (os > 0)
        {
            resize(os);
            copy(m_data, o.m_data, os);
        }
        return *this;
    }

    OGRE_INLINE void copy(Pointer dst, ConstPointer src, size_t size)
    {
        OGRE_ASSERT(size <= m_size);
        for (size_t i = 0; i < size; i++)
            new(&dst[i]) T(src[i]);
    }

    OGRE_INLINE void fill(T *src, size_t size)
    {
        OGRE_ASSERT(size <= m_size);
        for (size_t i = 0; i < size; i++)
            new(&m_data[i]) T(src[i]);
    }

protected:
    size_t       m_size;
    size_t       m_capacity;
    Pointer      m_data;
};


template <typename T>
class ArrayIterator
{
public:

    typedef typename T::Pointer     Iterator;
    typedef typename T::ValueType&  ValueType;

public:
    ArrayIterator() {}

    ArrayIterator(Iterator begin, size_t size) : m_iterator(begin), m_cur(0), m_capacity(size) {}
    ArrayIterator(T& v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) { }
    ~ArrayIterator() {}

    OGRE_INLINE const ArrayIterator& operator= (const ArrayIterator& o)
    {
        m_iterator = o.m_iterator;
        m_cur =    o.m_cur;
        m_capacity = o.m_capacity;
        return *this;
    }

    OGRE_INLINE bool hasMoreElements(void)    { return (m_iterator && m_cur < m_capacity); }
    OGRE_INLINE ValueType getNext(void)       { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); return m_iterator[m_cur++]; }
    OGRE_INLINE void next(void)               { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); ++m_cur;}
    OGRE_INLINE ValueType peekNext(void)      { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); return m_iterator[m_cur]; }

protected:
    Iterator    m_iterator;
    size_t        m_cur;
    size_t        m_capacity;
};
}

#endif//_OgreArray_h_
