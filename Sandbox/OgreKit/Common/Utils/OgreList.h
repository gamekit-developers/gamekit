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
#ifndef _OgreList_h_
#define _OgreList_h_

#include "OgreUtilsCommon.h"
#include "OgreUtilsAllocator.h"

namespace Ogre
{



template <typename T>
class List
{
public:

    class Link : public AllocObject
    {
    public:

        Link() : next(0), prev(0) {}
        Link(const T& v) : next(0), prev(0), link(v) {}

        Link*   next;
        Link*   prev;
        T       link;
    };

    typedef Link* Pointer;
    typedef T ValueType;
    typedef T& ReferenceType;
    typedef const T& ConstReferenceType;

public:
    List() : m_first(), m_last(0), m_size(0) {}
    ~List() { clear(); }

    
    void clear()
    {
        Link *node= m_first;
        while (node)
        {
            Link *temp= node;
            node= node->next;
            delete temp;
        }
        m_first= m_last= 0;
        m_size= 0;
    }

    void push_back(const T& v)
    {
        Link *link= new Link(v);
        link->prev= m_last;

        if (m_last)
            m_last->next= link;
        if (!m_first)
            m_first= link;

        m_last= link;
        m_size ++;
    }


    Pointer find(const T& v)
    {
        Link *node= m_first;
        while (node)
        {
            if (node->link == v)
                return node;
            node= node->next;
        }
        return 0;
    }



    void push_front(const T& v)
    {
        Link *link= new Link(v);

        link->next= m_first;
        if (m_first)
            m_first->prev= link;
        if (!m_last)
            m_last= link;
        m_first= link;

        m_size ++;
    }


    void insert(Pointer from, const T& v)
    {
        Link *link= new Link(v);
        m_size ++;

        if (!m_first)
        {
            m_first= m_last= link;
            return;
        }

        if (from == 0)
        {
            link->next= m_first;
            link->next->prev= link;
            m_first= link;
            return;
        }

        if (m_last == from)
            m_last= link;

        link->next= from->next;
        from->next= link;
        if (link->next)
            link->next->prev= link;
        link->prev= from;
    }
    ReferenceType at(size_t index)
    {
        OGRE_ASSERT(index < m_size);

        size_t i= 0;
        Link *node= m_first;

        while (node)
        {
            if (i == index)
                break;
            node= node->next;
            i++;
        }

        OGRE_ASSERT(node);
        return node->link;
    }
    
    ConstReferenceType at(size_t index) const
    {
        OGRE_ASSERT(index < m_size);

        size_t i= 0;
        Link *node= m_first;

        while (node)
        {
            if (i == index)
                break;
            node= node->next;
            i++;
        }

        OGRE_ASSERT(node);
        return node->link;
    }

    Pointer link_at(size_t index)
    {
        size_t i= 0;
        Link *node= m_first;

        while (node)
        {
            if (i == index)
                return node;

            node= node->next;
            i++;
        }
        return 0;
    }

    void remove(Pointer link)
    {
        if (!link || m_size == 0)
            return;

        if (link->next)
            link->next->prev= link->prev;

        if (link->prev)
            link->prev->next= link->next;

        if (m_last == link)
            m_last= link->prev;

        if (m_first == link)
            m_first= link->next;

        m_size -= 1;
    }

    void erase(Pointer link)
    {
        if (!link || m_size == 0)
            return;

        /// remove and delete
        remove(link);
        delete link;
    }

    OGRE_INLINE void pop_back()       { erase(end()); }
    OGRE_INLINE void pop_front()      { erase(begin()); }
    OGRE_INLINE bool empty() const    { return m_size == 0; }
    OGRE_INLINE size_t size()const    { return m_size; }
    OGRE_INLINE Pointer begin()       { return m_first; }
    OGRE_INLINE Pointer end()         { return m_last; }
    OGRE_INLINE ReferenceType front() { assert(m_first); return m_first->link; }
    OGRE_INLINE ReferenceType back()  { assert(m_last); return m_last->link; }

protected:

    Link*   m_first;
    Link*   m_last;
    size_t  m_size;
};


template <typename T>
class ListIterator
{
public:

    typedef typename T::Pointer     Iterator;
    typedef typename T::ValueType&  ValueType;

public:


    ListIterator(Iterator first) :
            m_iterator(first), m_cur(first)
    {
    }

    ListIterator(T& v) :
            m_iterator(v.begin()), m_cur(v.begin())
    {
    }

    ~ListIterator() {}

    OGRE_INLINE bool hasMoreElements(void)    { return (m_cur != 0); }
    OGRE_INLINE void next(void)               { OGRE_ASSERT(m_cur != 0); m_cur= m_cur->next; }

    OGRE_INLINE ValueType getNext(void)
    {
        OGRE_ASSERT(m_cur != 0);
        ValueType ret= m_cur->link;
        m_cur= m_cur->next;
        return ret;
    }
    OGRE_INLINE ValueType peekNext(void)
    {
        OGRE_ASSERT(m_cur != 0);
        Iterator next= m_cur->next;
        OGRE_ASSERT(next != 0);
        return next->link;
    }


protected:
    Iterator    m_iterator;
    Iterator    m_cur;
};


template <typename T>
class ListReverseIterator
{
public:

    typedef typename T::Pointer     Iterator;
    typedef typename T::ValueType&  ValueType;

public:


    ListReverseIterator(Iterator last) :
            m_iterator(last), m_cur(last)
    {
    }

    ListReverseIterator(T& v) :
            m_iterator(v.end()), m_cur(v.end())
    {
    }

    ~ListReverseIterator() {}


    OGRE_INLINE bool hasMoreElements(void)    { return (m_cur != 0); }
    OGRE_INLINE void next(void)               { OGRE_ASSERT(m_cur != 0); m_cur= m_cur->prev; }

    OGRE_INLINE ValueType getNext(void)
    {
        OGRE_ASSERT(m_cur != 0);
        ValueType ret= m_cur->link;
        m_cur= m_cur->prev;
        return ret;
    }

    OGRE_INLINE ValueType peekNext(void)
    {
        OGRE_ASSERT(m_cur != 0);
        Iterator next= m_cur->prev;
        OGRE_ASSERT(next != 0);
        return next->link;
    }


protected:
    Iterator    m_iterator;
    Iterator    m_cur;
};

}

#endif//_OgreList_h_

