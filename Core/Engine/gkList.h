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
#ifndef _gkList_h_
#define _gkList_h_

#include "gkCommon.h"



// ----------------------------------------------------------------------------
template <typename T>
class gkLink
{
public:
	gkLink() : next(0), prev(0) {}
	gkLink(const T& v) : next(0), prev(0), link(v) {}

	gkLink*   next;
	gkLink*   prev;
	T		 link;
};

// ----------------------------------------------------------------------------
template <typename T>
class gkList
{
public:
	typedef gkLink<T> Link;
	typedef Link* Pointer;
	typedef T ValueType;
	typedef T& ReferenceType;

public:
	gkList() : m_first(), m_last(0), m_size(0) {}
	~gkList() { clear(); }


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
		size_t i= 0;
		Link *node= m_first;

		while (node)
		{
			if (i == index)
				return node->link;

			node= node->next;
			i++;
		}

		return T(0);
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
	void erase(Pointer link)
	{
		if (!link || m_size == 0)
			return;

		/// remove and delete
		remove(link);
		delete link;
	}

	GK_INLINE void pop_back()	   { erase(end()); }
	GK_INLINE void pop_front()	  { erase(begin()); }
	GK_INLINE bool empty() const	{ return m_size == 0; }
	GK_INLINE size_t size()const	{ return m_size; }
	GK_INLINE Pointer begin()	   { return m_first; }
	GK_INLINE Pointer end()		 { return m_last; }
	GK_INLINE ReferenceType front() { GK_ASSERT(m_first); return m_first->link; }
	GK_INLINE ReferenceType back()  { GK_ASSERT(m_last); return m_last->link; }

protected:

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


	Link*   m_first;
	Link*   m_last;
	size_t  m_size;
};



// ----------------------------------------------------------------------------
template <typename T>
class gkListIterator
{
public:

	typedef typename T::Pointer	 Iterator;
	typedef typename T::ValueType&  ValueType;

public:


	gkListIterator(Iterator first) :
			m_iterator(first), m_cur(first)
	{
	}

	gkListIterator(T& v) :
			m_iterator(v.begin()), m_cur(v.begin())
	{
	}

	~gkListIterator() {}

	GK_INLINE bool hasMoreElements(void)
	{
		return (m_cur != 0);
	}


	GK_INLINE ValueType getNext(void)
	{
		GK_ASSERT(m_cur != 0);
		ValueType ret= m_cur->link;
		m_cur= m_cur->next;
		return ret;
	}

	GK_INLINE void next(void)
	{
		GK_ASSERT(m_cur != 0);
		m_cur= m_cur->next;
	}

	GK_INLINE ValueType peekNext(void)
	{
		GK_ASSERT(m_cur != 0);
		return m_cur->link;
	}

protected:
	Iterator	m_iterator;		 // pointer access
	Iterator	m_cur;			  // current index
};



// ----------------------------------------------------------------------------
template <typename T>
class gkListReverseIterator
{
public:

	typedef typename T::Pointer	 Iterator;
	typedef typename T::ValueType&  ValueType;

public:


	gkListReverseIterator(Iterator last) :
			m_iterator(last), m_cur(last)
	{
	}

	gkListReverseIterator(T& v) :
			m_iterator(v.end()), m_cur(v.end())
	{
	}

	~gkListReverseIterator() {}


	GK_INLINE bool hasMoreElements(void)
	{
		return (m_cur != 0);
	}


	GK_INLINE ValueType getNext(void)
	{
		GK_ASSERT(m_cur != 0);
		ValueType ret= m_cur->link;
		m_cur= m_cur->prev;
		return ret;
	}

	GK_INLINE void next(void)
	{
		GK_ASSERT(m_cur != 0);
		m_cur= m_cur->prev;
	}


	GK_INLINE ValueType peekNext(void)
	{
		GK_ASSERT(m_cur != 0);
		return m_cur->link;
	}


protected:
	Iterator	m_iterator;		 // pointer access
	Iterator	m_cur;			  // current index
};


#endif//_gkList_h_
