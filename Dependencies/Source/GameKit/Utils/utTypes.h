/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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
#ifndef _utTypes_h_
#define _utTypes_h_

#include "utCommon.h"
#include <memory.h>


#define _UT_CACHE_LIMIT 999


template <typename T> UT_INLINE void    utSwap(T &a, T &b)                              { T t(a); a = b; b = t; }
template <typename T> UT_INLINE T       utMax(const T &a, const T &b)                   { return a < b ? b : a; }
template <typename T> UT_INLINE T       utMin(const T &a, const T &b)                   { return a < b ? a : b; }
template <typename T> UT_INLINE T       utClamp(const T &v, const T &a, const T &b)     { return v < a ? a : v > b ? b : v; }


// List iterator access.
template <typename T>
class utListIterator
{
public:

	typedef typename T::Pointer Iterator;

	typedef typename T::ReferenceType ValueType;
	typedef typename T::ConstReferenceType ConstValueType;


protected:

	mutable Iterator m_iterator;
	mutable Iterator m_cur;


public:

	utListIterator() : m_iterator(0), m_cur(0) {}

	utListIterator(Iterator first) : m_iterator(first), m_cur(first) {}
	utListIterator(T &v) : m_iterator(v.begin()), m_cur(v.begin()) { }
	~utListIterator() {}

	UT_INLINE bool hasMoreElements(void) const {  return m_cur != 0;  }


	UT_INLINE void next(void) const
	{
		UT_ASSERT(hasMoreElements());
		m_cur = m_cur->getNext();
	}


	UT_INLINE ValueType getNext(void)
	{
		UT_ASSERT(hasMoreElements());

		ValueType ret = m_cur->getLink();
		m_cur = m_cur->getNext();
		return ret;
	}

	UT_INLINE ConstValueType getNext(void) const
	{
		UT_ASSERT(hasMoreElements());

		ValueType ret = m_cur->getLink();
		m_cur = m_cur->getNext();
		return ret;
	}

	UT_INLINE ValueType      peekNext(void)       { UT_ASSERT(hasMoreElements()); return m_cur->getLink(); }
	UT_INLINE ConstValueType peekNext(void) const { UT_ASSERT(hasMoreElements()); return m_cur->getLink(); }
};


template <typename T>
class utListReverseIterator
{
public:

	typedef typename T::Pointer Iterator;
	typedef typename T::ReferenceType ValueType;
	typedef typename T::ConstReferenceType ConstValueType;

protected:

	mutable Iterator    m_iterator;
	mutable Iterator    m_cur;


public:
	utListReverseIterator() : m_iterator(0), m_cur(0) {}

	utListReverseIterator(Iterator last) : m_iterator(last), m_cur(last) {}
	utListReverseIterator(T &v) : m_iterator(v.end()), m_cur(v.end()) {}


	~utListReverseIterator() {}


	UT_INLINE bool hasMoreElements(void) const { return m_cur != 0; }


	UT_INLINE void next(void) const
	{
		UT_ASSERT(hasMoreElements());

		m_cur = m_cur->getPrev();
	}

	UT_INLINE ValueType getNext(void)
	{
		UT_ASSERT(hasMoreElements());

		ValueType ret = m_cur->getLink();
		m_cur = m_cur->getPrev();
		return ret;
	}

	UT_INLINE ConstValueType getNext(void) const
	{
		UT_ASSERT(hasMoreElements());

		ValueType ret = m_cur->getLink();
		m_cur = m_cur->getPrev();
		return ret;
	}

	UT_INLINE ValueType      peekNext(void)        { UT_ASSERT(hasMoreElements()); return m_cur->getLink(); }
	UT_INLINE ConstValueType peekNext(void) const  { UT_ASSERT(hasMoreElements()); return m_cur->getLink(); }
};

template <typename T>
class utListClass
{
public:
	typedef T             *ValueType;
	typedef const T       *ConstValueType;
	typedef T             *Pointer;
	typedef T             *ReferenceType;
	typedef const T       *ConstReferenceType;

	typedef utListIterator<utListClass<T> >              Iterator;
	typedef utListReverseIterator<utListClass<T> >       ReverseIterator;

	// Const access
	typedef const utListIterator<utListClass<T> >        ConstIterator;
	typedef const utListReverseIterator<utListClass<T> > ConstReverseIterator;

	class Link
	{
	protected:
		T *m_next;
		T *m_prev;
		friend class utListClass;

	public:

		Link() : m_next(0), m_prev(0) {}
		~Link() {}

		UT_INLINE T *getNext(void) {return m_next;}
		UT_INLINE T *getPrev(void) {return m_prev;}
		UT_INLINE T *getLink(void) { return (T *)this; }
	};


public:

	utListClass() : m_first(0), m_last(0), m_size(0) {}

	~utListClass() { clear(); }

	void clear(void)
	{
		m_first = m_last = 0;
		m_size = 0;
	}

	void push_back(ValueType v)
	{
		v->m_prev = m_last;
		if (m_last) m_last->m_next = v;
		if (!m_first) m_first = v;
		m_last = v;
		m_size ++;
	}

	void push_front(ValueType v)
	{
		v->m_next = m_first;
		if (m_first) m_first->m_prev = v;
		if (!m_last) m_last = v;
		m_first = v;
		m_size ++;
	}

	void insert(ValueType from, ValueType v)
	{
		m_size ++;
		if (!m_first)
		{
			m_first = m_last = v;
			return;
		}

		if (from == 0)
		{
			// add to back
			v->m_prev = m_last;
			if (m_last) m_last->m_next = v;
			if (!m_first) m_first = v;
			m_last = v;
			return;
		}

		if (m_last == from) m_last = v;

		v->m_next = from->m_next;
		from->m_next = v;
		if (v->m_next) v->m_next->m_prev = v;
		v->m_prev = from;
	}

	ValueType find(ValueType v)
	{
		ValueType node = m_first;
		while (node)
		{
			if (node == v) return node;
			node = node->m_next;
		}
		return 0;
	}

	ValueType at(UTsize index)
	{
		UT_ASSERT(index < m_size);
		UTsize i = 0;
		ValueType node = m_first;
		while (node)
		{
			if (i == index) break;
			node = node->m_next;
			i++;
		}
		UT_ASSERT(node);
		return node;
	}

	ConstValueType at(UTsize index) const
	{
		UT_ASSERT(index < m_size);
		UTsize i = 0;
		ValueType node = m_first;
		while (node)
		{
			if (i == index) break;
			node = node->m_next;
			i++;
		}
		UT_ASSERT(node);
		return node;
	}

	ValueType erase(ValueType v)
	{
		if (!v || m_size == 0)
			return 0;
		if (v->m_next) v->m_next->m_prev = v->m_prev;
		if (v->m_prev) v->m_prev->m_next = v->m_next;
		if (m_last == v) m_last = v->m_prev;
		if (m_first == v) m_first = v->m_next;
		m_size -= 1;
		return v;
	}

	UT_INLINE ValueType pop_back(void)          { return erase(end()); }
	UT_INLINE ValueType pop_front(void)         { return erase(begin()); }

	UT_INLINE bool empty(void) const            { return m_size == 0; }
	UT_INLINE UTsize size(void)const            { return m_size; }

	UT_INLINE ValueType begin(void)             { UT_ASSERT(m_first); return m_first; }
	UT_INLINE ValueType end(void)               { UT_ASSERT(m_last); return m_last; }
	UT_INLINE ConstValueType begin(void) const  { UT_ASSERT(m_first); return m_first; }
	UT_INLINE ConstValueType end(void) const    { UT_ASSERT(m_last); return m_last; }


	UT_INLINE Iterator              iterator(void)               { return m_first ? Iterator(m_first) : Iterator(); }
	UT_INLINE ReverseIterator       reverseIterator(void)        { return m_last ? ReverseIterator(m_last) : ReverseIterator(); }
	UT_INLINE ConstIterator         iterator(void) const         { return m_first ? ConstIterator(m_first) : ConstIterator(); }
	UT_INLINE ConstReverseIterator  reverseIterator(void) const  { return m_last ? ConstReverseIterator(m_last) : ConstReverseIterator(); }


protected:

	T *m_first;
	T *m_last;
	UTsize m_size;
};

template <typename T>
class utList
{
public:

	class Link
	{
	protected:
		Link     *next;
		Link     *prev;
		friend class utList;

	public:

		Link() : next(0), prev(0) {}
		Link(const T &v) : next(0), prev(0), link(v) {}


		UT_INLINE Link  *getNext(void)  {return next;}
		UT_INLINE Link  *getPrev(void)  {return prev;}
		UT_INLINE T     &getLink(void)  {return link;}
		T link;
	};

	typedef Link              *Pointer;
	typedef const Link        *ConstPointer;
	typedef T                  ValueType;
	typedef const T            ConstValueType;
	typedef T                 &ReferenceType;
	typedef const T           &ConstReferenceType;


	typedef utListIterator<utList<T> >        Iterator;
	typedef utListReverseIterator<utList<T> > ReverseIterator;
	typedef const utListIterator<utList<T> >  ConstIterator;
	typedef const utListIterator<utList<T> >  ConstReverseIterator;


public:
	utList() : m_first(0), m_last(0), m_size(0) {}
	~utList() { clear(); }

	void clear(void)
	{
		Link *node = m_first;
		while (node)
		{
			Link *temp = node;
			node = node->next;
			delete temp;
		}
		m_first = m_last = 0;
		m_size = 0;
	}

	UT_INLINE void push_back(const T &v)
	{
		Link *link = new Link(v);
		link->prev = m_last;
		if (m_last) m_last->next = link;
		if (!m_first) m_first = link;
		m_last = link;
		m_size ++;
	}

	UT_INLINE void push_front(const T &v)
	{
		Link *link = new Link(v);
		link->next = m_first;
		if (m_first) m_first->prev = link;
		if (!m_last) m_last = link;
		m_first = link;
		m_size ++;
	}

	Pointer find(const T &v)
	{
		Link *node = m_first;
		while (node)
		{
			if (node->link == v) return node;
			node = node->next;
		}
		return 0;
	}

	ReferenceType at(UTsize index)
	{
		UT_ASSERT(index < m_size);
		UTsize i = 0;
		Link *node = m_first;
		while (node)
		{
			if (i == index) break;
			node = node->next;
			i++;
		}
		UT_ASSERT(node);
		return node->link;
	}

	ConstReferenceType at(UTsize index) const
	{
		UT_ASSERT(index < m_size);
		UTsize i = 0;
		Link *node = m_first;
		while (node)
		{
			if (i == index) break;
			node = node->next;
			i++;
		}
		UT_ASSERT(node);
		return node->link;
	}

	Pointer link_at(UTsize index)
	{
		UTsize i = 0;
		Link *node = m_first;
		while (node)
		{
			if (i == index) return node;
			node = node->next;
			i++;
		}
		return 0;
	}

	void erase(Pointer link)
	{
		if (!link || m_size == 0)
			return;
		if (link->next) link->next->prev = link->prev;
		if (link->prev) link->prev->next = link->next;
		if (m_last == link) m_last = link->prev;
		if (m_first == link) m_first = link->next;

		m_size -= 1;
		delete link;
		if (m_size == 0)
			clear();
	}

	void erase(const T &v)
	{
		Pointer fnd = find(v);
		if (fnd)
			erase(fnd);
	}

	void sort(bool (*cmp)(const T &a, const T &b))
	{
		UTsize i, n=m_size;
		Link *a;
		bool swapped = false;
		if (n < 2 || !cmp)
			return;
		do
		{
			a = m_first;
			for (i=0; a && a->next && i<n-1; i++, a = a->next)
			{
				if (cmp(a->link, a->next->link))
				{
					swap(a, a->next);
					swapped = true;
				}
			}
			n-=1;
		}
		while (swapped && n != UT_NPOS);
	}

	UT_INLINE void pop_back(void)       { erase(end()); }
	UT_INLINE void pop_front(void)      { erase(begin()); }


	UT_INLINE bool empty(void) const    { return m_size == 0; }
	UT_INLINE UTsize size(void)const    { return m_size; }

	UT_INLINE Pointer begin(void)               { return m_first; }
	UT_INLINE Pointer end(void)                 { return m_last; }
	UT_INLINE ConstPointer begin(void)const     { return m_first; }
	UT_INLINE ConstPointer end(void)const       { return m_last; }

	UT_INLINE ReferenceType front(void)         { UT_ASSERT(m_first); return m_first->link; }
	UT_INLINE ReferenceType back(void)          { UT_ASSERT(m_last); return m_last->link; }

	UT_INLINE Iterator              iterator(void)               { return m_first ? Iterator(m_first) : Iterator(); }
	UT_INLINE ReverseIterator       reverseIterator(void)        { return m_last ? ReverseIterator(m_last) : ReverseIterator(); }
	UT_INLINE ConstIterator         iterator(void) const         { return m_first ? ConstIterator(m_first) : ConstIterator(); }
	UT_INLINE ConstReverseIterator  reverseIterator(void) const  { return m_last ? ConstReverseIterator(m_last) : ConstReverseIterator(); }

protected:

	Link     *m_first;
	Link     *m_last;
	UTsize    m_size;


	void swap(Link *a, Link *b)
	{
		ValueType v(a->link);
		a->link = b->link;
		b->link = v;
	}
};

template <typename T>
class utArrayIterator
{
public:

	typedef typename T::Pointer Iterator;

	// T::ValueType as reference
	typedef typename T::ReferenceType      ValueType;
	typedef typename T::ConstReferenceType ConstValueType;


protected:

	mutable Iterator   m_iterator;
	mutable UTsize     m_cur;
	mutable UTsize     m_capacity;


public:


	utArrayIterator() : m_iterator(0), m_cur(0), m_capacity(0) {}
	utArrayIterator(Iterator begin, UTsize size) : m_iterator(begin), m_cur(0), m_capacity(size) {}
	utArrayIterator(T &v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) { }

	~utArrayIterator() {}

	UT_INLINE bool           hasMoreElements(void) const { return m_iterator && m_cur < m_capacity; }
	UT_INLINE ValueType      getNext(void)               { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];  }
	UT_INLINE ConstValueType getNext(void) const         { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];  }
	UT_INLINE void           next(void) const            { UT_ASSERT(hasMoreElements()); ++m_cur; }
	UT_INLINE ValueType      peekNext(void)              { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }
	UT_INLINE ConstValueType peekNext(void) const        { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }

};

template <typename T>
class utArray
{
public:
	typedef T           *Pointer;
	typedef const T     *ConstPointer;

	typedef T            ValueType;
	typedef const T      ConstValueType;

	typedef T           &ReferenceType;
	typedef const T     &ConstReferenceType;

	typedef utArrayIterator<utArray<T> >       Iterator;
	typedef const utArrayIterator<utArray<T> > ConstIterator;

public:
	utArray() : m_size(0), m_capacity(0), m_data(0), m_cache(0)  {}

	utArray(const utArray<T>& o)
		: m_size(o.size()), m_capacity(0), m_data(0), m_cache(0)
	{
		reserve(m_size);
		copy(m_data, o.m_data, m_size);
	}

	~utArray() { clear(); }

	void clear(bool useCache=false)
	{
		if (!useCache)
		{
			if (m_data)
				delete []m_data;
			m_data = 0;
			m_capacity = 0;
			m_size = 0;
			m_cache = 0;
		}
		else
		{
			++m_cache;
			if (m_cache > _UT_CACHE_LIMIT )
				clear(false);
			else m_size = 0;
		}
	}

	UTsize find(const T &v)
	{
		for (UTsize i = 0; i < m_size; i++)
		{
			if (m_data[i] == v)
				return i;
		}
		return UT_NPOS;
	}

	UT_INLINE void push_back(const T &v)
	{
		if (m_size == m_capacity)
			reserve(m_size == 0 ? 8 : m_size * 2);

		m_data[m_size] = v;
		m_size++;
	}

	UT_INLINE void pop_back(void)
	{
		m_size--;
		m_data[m_size].~T();
	}


	void erase(const T &v)
	{
		erase(find(v));
	}

	void erase(UTsize pos)
	{
		if (m_size > 0)
		{
			if (pos != UT_NPOS)
			{
				swap(pos, m_size-1);
				m_size--;
				m_data[m_size].~T();
			}
		}
	}

	void resize(UTsize nr)
	{
		if (nr < m_size)
		{
			for (UTsize i = m_size; i < nr; i++)
				m_data[i].~T();
		}
		else
		{
			if (nr > m_size)
				reserve(nr);
		}
		m_size = nr;
	}

	void resize(UTsize nr, const T &fill)
	{
		if (nr < m_size)
		{
			for (UTsize i = m_size; i < nr; i++)
				m_data[i].~T();
		}
		else
		{
			if (nr > m_size)
				reserve(nr);
			for (UTsize i = m_size; i < nr; i++)
				m_data[i] = fill;

		}
		m_size = nr;
	}

	void reserve(UTsize nr)
	{

		if (m_capacity < nr)
		{
			T *p = new T[nr];
			if (m_data != 0)
			{
				copy(p, m_data, m_size);
				delete []m_data;
			}
			m_data = p;
			m_capacity = nr;
		}
	}

	void sort(bool (*cmp)(const T &a, const T &b))
	{
		UTsize i, n=m_size;
		bool swapped = false;
		if (n < 2 || !cmp)
			return;
		do
		{
			if (n-1 == UT_NPOS)
				break;
			for (i=0; /*m_data[i] && m_data[i+1] && */i<(n-1); i++)
			{
				if (cmp(m_data[i], m_data[i+1]))
				{
					swap(i, i+1);
					swapped = true;
				}
			}
			n-=1;
		}
		while (swapped && n != UT_NPOS);
	}

	UT_INLINE T &operator[](UTsize idx)             { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	UT_INLINE const T &operator[](UTsize idx) const { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	UT_INLINE T &at(UTsize idx)                     { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	UT_INLINE const T &at(UTsize idx) const         { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	UT_INLINE T &front(void)                        { UT_ASSERT(m_size > 0); return m_data[0]; }
	UT_INLINE T &back(void)                         { UT_ASSERT(m_size > 0); return m_data[m_size-1]; }


	UT_INLINE ConstPointer ptr(void) const          { return m_data; }
	UT_INLINE Pointer      ptr(void)                { return m_data; }
	UT_INLINE bool         valid(void) const        { return m_data != 0;}

	UT_INLINE UTsize capacity(void) const           { return m_capacity; }
	UT_INLINE UTsize size(void) const               { return m_size; }
	UT_INLINE bool empty(void) const                { return m_size == 0;}


	UT_INLINE Iterator       iterator(void)       { return m_data && m_size > 0 ? Iterator(m_data, m_size) : Iterator(); }
	UT_INLINE ConstIterator  iterator(void) const { return m_data && m_size > 0 ? ConstIterator(m_data, m_size) : ConstIterator(); }

	utArray<T> &operator= (const utArray<T> &rhs)
	{
		if (this != &rhs)
		{
			clear();
			UTsize os = rhs.size();
			if (os > 0)
			{
				resize(os);
				copy(m_data, rhs.m_data, os);
			}
		}

		return *this;
	}

	UT_INLINE void copy(Pointer dst, ConstPointer src, UTsize size)
	{
		UT_ASSERT(size <= m_size);
		for (UTsize i = 0; i < size; i++) dst[i] = src[i];
	}

protected:

	void swap(UTsize a, UTsize b)
	{
		ValueType t= m_data[a];
		m_data[a] = m_data[b];
		m_data[b] = t;
	}

	UTsize      m_size;
	UTsize      m_capacity;
	Pointer     m_data;
	int         m_cache;
};

template <typename T>
class utStackIterator
{
public:
	typedef typename T::Pointer            Iterator;
	typedef typename T::ReferenceType      ValueType;
	typedef typename T::ConstReferenceType ConstValueType;

protected:

	mutable Iterator m_iterator;
	mutable UTsize   m_top;
	const UTsize     m_capacity;

public:
	utStackIterator() : m_iterator(0), m_top(0), m_capacity(0)  {}

	utStackIterator(Iterator begin, UTsize top) : m_iterator(begin), m_top(top), m_capacity(top) {}
	utStackIterator(T &v) : m_iterator(v.begin()), m_top(v.itop()), m_capacity(v.itop()) {}

	~utStackIterator() {}


	UT_INLINE bool hasMoreElements(void) const { return (m_iterator && m_top != 0 && m_top != UT_NPOS); }


	UT_INLINE ValueType getNext(void)
	{
		UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
		--m_top;
		return m_iterator[m_top];
	}

	UT_INLINE ConstValueType getNext(void) const
	{
		UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
		--m_top;
		return m_iterator[m_top];
	}


	UT_INLINE void next(void) const
	{
		UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
		--m_top;
	}

	UT_INLINE ValueType peekNext(void)
	{
		UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
		return m_iterator[m_top-1];
	}

	UT_INLINE ConstValueType peekNext(void) const
	{
		UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
		return m_iterator[m_top-1];
	}

};

template <typename T>
class utStack
{
public:
	typedef T *Pointer;
	typedef const T *ConstPointer;

	typedef T        ValueType;
	typedef const T  ConstValueType;


	typedef T &ReferenceType;
	typedef const T &ConstReferenceType;


	typedef utStackIterator<utStack<T> >       Iterator;
	typedef const utStackIterator<utStack<T> > ConstIterator;

public:

	utStack() : m_size(0), m_capacity(0), m_top(0), m_stack(0) {}
	utStack(UTsize rsp) : m_size(0), m_capacity(0), m_top(0), m_stack(0) { reserve(rsp); }


	utStack(const utStack &o) : m_size(o.m_size), m_capacity(0), m_top(o.m_top), m_stack(0)
	{
		reserve(o.m_size);
		UT_ASSERT(m_stack && o.m_stack);
		copy(m_stack, o.m_stack, o.m_size);
	}

	~utStack() { clear(); }

	void clear(void)
	{
		if (m_stack != 0)
		{
			delete []m_stack;
			m_stack = 0;
		}

		m_size = 0;
		m_capacity = 0;
		m_top = 0;
	}


	void push(const T &v)
	{
		UT_ASSERT(m_top != UT_NPOS);
		if (m_size == m_capacity)
			reserve(m_size == 0 ? 8 : m_size * 2);
		m_stack[m_top] = v;
		++m_top;
		++m_size;
	}

	UT_INLINE ReferenceType pop(void)               { UT_ASSERT(m_top != 0); --m_size; return m_stack[--m_top]; }
	UT_INLINE ReferenceType top(void)               { UT_ASSERT(m_top != 0); return m_stack[(m_top - 1)]; }
	UT_INLINE ConstReferenceType top(void) const    { UT_ASSERT(m_top != 0); return m_stack[(m_top - 1)]; }

	UT_INLINE ReferenceType peek(UTsize offs)
	{
		UT_ASSERT(m_top != 0 && ((m_top - 1) - offs) != UT_NPOS);
		return m_stack[(m_top - 1)-offs];
	}

	UT_INLINE ConstReferenceType peek(UTsize offs) const
	{
		UT_ASSERT(m_top != 0 && ((m_top - 1) - offs) != UT_NPOS);
		return m_stack[(m_top - 1)-offs];
	}

	void reserve(UTsize nr)
	{
		if (m_capacity < nr)
		{
			T *temp = new T[nr];
			if (m_stack)
			{
				copy(temp, m_stack, m_size);
				delete []m_stack;
			}
			m_stack = temp;
			m_capacity = nr;
		}
	}

	UT_INLINE UTsize capacity(void) const           { return m_capacity; }
	UT_INLINE UTsize size(void) const               { return m_size; }
	UT_INLINE UTsize itop(void) const               { return m_top; }
	UT_INLINE bool empty(void) const                { return m_size == 0; }
	UT_INLINE ConstPointer ptr(void) const          { return m_stack; }
	UT_INLINE Pointer ptr(void)                     { return m_stack; }

	UT_INLINE Iterator      iterator(void)       { return m_stack && m_size > 0 ? Iterator(m_stack, m_size) : Iterator(); }
	UT_INLINE ConstIterator iterator(void) const { return m_stack && m_size > 0 ? ConstIterator(m_stack, m_size) : ConstIterator(); }


protected:


	UT_INLINE void copy(Pointer dest, const Pointer src, UTsize nr)
	{
		UTsize i;
		for (i = 0; i < nr; i++)
			dest[i] = src[i];
	}

	UTsize  m_size;
	UTsize  m_capacity;
	UTsize  m_top;
	Pointer m_stack;
};

template <typename T>
class utHashTableIterator
{
public:

	typedef typename T::Pointer        Iterator;
	typedef typename T::Entry          &Pair;
	typedef typename T::ConstEntry     &ConstPair;

	typedef typename T::ReferenceKeyType         KeyType;
	typedef typename T::ReferenceValueType       ValueType;
	typedef typename T::ConstReferenceKeyType    ConstKeyType;
	typedef typename T::ConstReferenceValueType  ConstValueType;

protected:

	mutable Iterator m_iterator;
	mutable UTsize   m_cur;
	const UTsize     m_capacity;


public:
	utHashTableIterator() : m_iterator(0), m_cur(0), m_capacity(0)  {}
	utHashTableIterator(Iterator begin, UTsize size) : m_iterator(begin), m_cur(0), m_capacity(size) { }
	utHashTableIterator(T &v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) {}

	~utHashTableIterator() {}

	UT_INLINE bool      hasMoreElements(void) const  { return (m_iterator && m_cur < m_capacity); }
	UT_INLINE Pair      getNext(void)                { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];}
	UT_INLINE ConstPair getNext(void) const          { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];}
	UT_INLINE void      next(void) const             { UT_ASSERT(hasMoreElements()); ++m_cur; }


	UT_INLINE Pair      peekNext(void)               { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }
	UT_INLINE KeyType   peekNextKey(void)            { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur].first;}
	UT_INLINE ValueType peekNextValue(void)          { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur].second; }

	UT_INLINE ConstPair      peekNext(void)  const     { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }
	UT_INLINE ConstKeyType   peekNextKey(void) const   { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur].first;}
	UT_INLINE ConstValueType peekNextValue(void) const { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur].second; }
};



// magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
#define _UT_INITIAL_FNV  0x9E3779B1
#define _UT_INITIAL_FNV2 0x9E3779B9
#define _UT_MULTIPLE_FNV 0x1000193
#define _UT_TWHASH(key)         \
        key += ~(key << 15);    \
        key ^=  (key >> 10);    \
        key +=  (key << 3);     \
        key ^=  (key >> 6);     \
        key += ~(key << 11);    \
        key ^=  (key >> 16);

class utCharHashKey
{
protected:
	char *m_key;
	mutable UThash m_hash;

public:
	utCharHashKey() : m_key(0), m_hash(UT_NPOS) {}
	utCharHashKey(char *k) : m_key(k), m_hash(UT_NPOS) {hash();}
	utCharHashKey(const char *k) : m_key(const_cast<char *>(k)), m_hash(UT_NPOS) {}

	// Copy constructor
	utCharHashKey(const utCharHashKey &k) : m_key(k.m_key), m_hash(k.m_hash) {}

	UT_INLINE const char *key(void) const {return m_key;}

	UThash hash(void) const
	{
		if (!m_key) return UT_NPOS;

		// use cached hash
		if (m_hash != UT_NPOS) return m_hash;

		// Fowler / Noll / Vo (FNV) Hash
		m_hash = (UThash)_UT_INITIAL_FNV;
		for (int i = 0; m_key[i]; i++)
		{
			m_hash = m_hash ^(m_key[i]);    // xor  the low 8 bits
			m_hash = m_hash * _UT_MULTIPLE_FNV;  // multiply by the magic number
		}
		return m_hash;
	}

	UT_INLINE bool operator== (const utCharHashKey &v) const    {return hash() == v.hash();}
	UT_INLINE bool operator!= (const utCharHashKey &v) const    {return hash() != v.hash();}
	UT_INLINE bool operator== (const UThash &v) const           {return hash() == v;}
	UT_INLINE bool operator!= (const UThash &v) const           {return hash() != v;}
};

class utIntHashKey
{
protected:
	UTint32 m_key;
public:
	utIntHashKey() : m_key(0) {}
	utIntHashKey(UTint32 k) : m_key(k) {}
	utIntHashKey(const utIntHashKey &k) : m_key(k.m_key) {}

	UT_INLINE UTint32 key(void)  const  { return m_key; }
	UT_INLINE UThash  hash(void) const  { return static_cast<UThash>(m_key) * _UT_INITIAL_FNV; }

	UT_INLINE bool operator== (const utIntHashKey &v) const {return hash() == v.hash();}
	UT_INLINE bool operator!= (const utIntHashKey &v) const {return hash() != v.hash();}
	UT_INLINE bool operator== (const UThash &v) const       {return hash() == v;}
	UT_INLINE bool operator!= (const UThash &v) const       {return hash() != v;}
};

class utPointerHashKey
{
protected:
	void *m_key;

public:
	utPointerHashKey() : m_key(0) {}
	utPointerHashKey(void *k) : m_key(k) {}
	utPointerHashKey(const utPointerHashKey &k) : m_key(k.m_key) {}

	UT_INLINE void          *key(void)       {return m_key;}
	UT_INLINE const void    *key(void) const {return m_key;}


	UT_INLINE UThash hash(void) const
	{
		UThash key = static_cast<UThash>(reinterpret_cast<UTuintPtr>(m_key));
		_UT_TWHASH(key);
		return key;
	}


	UT_INLINE bool operator== (const utPointerHashKey &v) const {return hash() == v.hash();}
	UT_INLINE bool operator!= (const utPointerHashKey &v) const {return hash() != v.hash();}
	UT_INLINE bool operator== (const UThash &v) const           {return hash() == v;}
	UT_INLINE bool operator!= (const UThash &v) const           {return hash() != v;}
};

template<typename Key, typename Value>
struct utHashEntry
{
	Key    first;
	Value  second;

	utHashEntry() {}
	utHashEntry(const Key &k, const Value &v) : first(k), second(v) {}
	UT_INLINE const utHashEntry &operator=(const utHashEntry &rhs)
	{
		first = rhs.first;
		second = rhs.second;
		return *this;
	}
	UT_INLINE bool operator==(const utHashEntry &rhs) const
	{
		return first == rhs.first && second == rhs.second;
	}
};

#define _UT_UTHASHTABLE_HASH(key)   ((key.hash() & (m_capacity - 1)))
#define _UT_UTHASHTABLE_HKHASH(key) ((hk & (m_capacity - 1)))
// Initial table size
#define _UT_UTHASHTABLE_INIT     32
#define _UT_UTHASHTABLE_EXPANSE  (m_size * 2)

// Force power of two table size.
#define _UT_UTHASHTABLE_FORCE_POW2 1
#define _UT_UTHASHTABLE_STAT       0
#define _UT_UTHASHTABLE_STAT_ALLOC 0


#if _UT_UTHASHTABLE_FORCE_POW2 == 1
#define _UT_UTHASHTABLE_POW2(x) \
	--x; x |= x >> 16; x |= x >> 8; x |= x >> 4; \
	x |= x >> 2; x |= x >> 1; ++x;

#define _UT_UTHASHTABLE_IS_POW2(x) (x && !((x-1) & x))
#endif


#if _UT_UTHASHTABLE_STAT == 1
# include <stdio.h>
# include <typeinfo>
#endif


template < typename Key, typename Value>
class utHashTable
{
public:
	typedef utHashEntry<Key, Value>        Entry;
	typedef const utHashEntry<Key, Value>  ConstEntry;

	typedef Entry  *EntryArray;
	typedef UTsize *IndexArray;


	typedef Key            KeyType;
	typedef Value          ValueType;

	typedef const Key      ConstKeyType;
	typedef const Value    ConstValueType;

	typedef Value          &ReferenceValueType;
	typedef const Value    &ConstReferenceValueType;

	typedef Key            &ReferenceKeyType;
	typedef const Key      &ConstReferenceKeyType;

	typedef EntryArray Pointer;
	typedef const Entry *ConstPointer;


	typedef utHashTableIterator<utHashTable<Key, Value> > Iterator;
	typedef const utHashTableIterator<utHashTable<Key, Value> > ConstIterator;


public:

	utHashTable()
		:    m_size(0), m_capacity(0), m_lastPos(UT_NPOS),
		     m_iptr(0), m_nptr(0), m_bptr(0), m_cache(0)
	{
	}

	utHashTable(UTsize capacity)
		:    m_size(0), m_capacity(0), m_lastPos(UT_NPOS),
		     m_iptr(0), m_nptr(0), m_bptr(0), m_cache(0)
	{
	}

	utHashTable(const utHashTable &rhs)
		:    m_size(0), m_capacity(0), m_lastPos(UT_NPOS),
		     m_iptr(0), m_nptr(0), m_bptr(0), m_cache(0)
	{
		doCopy(rhs);
	}

	~utHashTable() { clear(); }

	utHashTable<Key, Value> &operator = (const utHashTable<Key, Value> &rhs)
	{
		if (this != &rhs)
			doCopy(rhs);
		return *this;
	}

	void clear(bool useCache = false)
	{
		if (!useCache)
		{
			m_size = m_capacity = 0;
			m_lastKey = UT_NPOS;
			m_lastPos = UT_NPOS;
			m_cache = 0;

			delete [] m_bptr;
			delete [] m_iptr;
			delete [] m_nptr;
			m_bptr = 0; m_iptr = 0; m_nptr = 0;
		}
		else
		{
			++m_cache;
			if (m_cache > _UT_CACHE_LIMIT)
				clear(false);
			else
			{
				m_size = 0;
				m_lastKey = UT_NPOS;
				m_lastPos = UT_NPOS;


				UTsize i;

				// Must zero 
				for (i=0; i<m_capacity; ++i)
				{
					m_iptr[i] = UT_NPOS;
					m_nptr[i] = UT_NPOS;
				}
			}
		}

	}
	Value              &at(UTsize i)                    { UT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	Value              &operator [](UTsize i)           { UT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	const Value        &at(UTsize i)const               { UT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	const Value        &operator [](UTsize i) const     { UT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	Key                &keyAt(UTsize i)                 { UT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].first; }
	const Key          &keyAt(UTsize i)const            { UT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].first; }


	// Find and cache key
	Value* get(const Key &key)
	{
		if (!m_bptr || m_size == 0)
			return (Value*)0;


		UThash hr = key.hash();

		if (m_lastKey != hr)
		{
			UTsize i = find(key);
			if (i == UT_NPOS) return (Value*)0;


			UT_ASSERT(i >=0 && i < m_size);

			m_lastKey = hr;
			m_lastPos = i;
		}

		return &m_bptr[m_lastPos].second;
	}


	Value*         operator [](const Key &key)       { return get(key); }
	const Value*   operator [](const Key &key) const { return get(key); }

	UTsize find(const Key &key) const
	{
		if (m_capacity == 0 || m_capacity == UT_NPOS || m_size == 0)
			return UT_NPOS;

		UTsize hk = key.hash();

		// Short cut.
		if (m_lastPos != UT_NPOS && m_lastKey == hk)
			return m_lastPos;


		UThash hr = _UT_UTHASHTABLE_HKHASH(hk);

		UT_ASSERT(m_bptr && m_iptr && m_nptr);

		UTsize fh = m_iptr[hr];
		while (fh != UT_NPOS && (key != m_bptr[fh].first))
			fh = m_nptr[fh];


		if (fh != UT_NPOS)
		{
			m_lastKey = hk;
			m_lastPos = fh;

			UT_ASSERT(fh >= 0  && fh < m_size);
		}
		return fh;
	}



	void erase(const Key &key) {remove(key);}

	void remove(const Key &key)
	{
		UThash hash, lhash;
		UTsize index, pindex, findex;

		findex = find(key);
		if (findex == UT_NPOS || m_capacity == 0 || m_size == 0)
			return;

		m_lastKey = UT_NPOS;
		m_lastPos = UT_NPOS;
		UT_ASSERT(m_bptr && m_iptr && m_nptr);


		hash = _UT_UTHASHTABLE_HASH(key);

		index  = m_iptr[hash];
		pindex = UT_NPOS;
		while (index != findex)
		{
			pindex = index;
			index = m_nptr[index];
		}

		if (pindex != UT_NPOS)
		{
			UT_ASSERT(m_nptr[pindex] == findex);
			m_nptr[pindex] = m_nptr[findex];
		}
		else
			m_iptr[hash] = m_nptr[findex];

		UTsize lindex = m_size - 1;
		if (lindex == findex)
		{
			--m_size;
			m_bptr[m_size].~Entry();
			return;
		}

		lhash = _UT_UTHASHTABLE_HASH(m_bptr[lindex].first);
		index  = m_iptr[lhash];
		pindex = UT_NPOS;
		while (index != lindex)
		{
			pindex = index;
			index = m_nptr[index];
		}

		if (pindex != UT_NPOS)
		{
			UT_ASSERT(m_nptr[pindex] == lindex);
			m_nptr[pindex] = m_nptr[lindex];
		}
		else
			m_iptr[lhash] = m_nptr[lindex];

		m_bptr[findex] = m_bptr[lindex];
		m_nptr[findex] = m_iptr[lhash];
		m_iptr[lhash] = findex;

		--m_size;
		m_bptr[m_size].~Entry();
		return;
	}

	bool insert(const Key &key, const Value &val)
	{
		if (find(key) != UT_NPOS)
			return false;

		if (m_size == m_capacity)
			reserve(m_size == 0 ? _UT_UTHASHTABLE_INIT : _UT_UTHASHTABLE_EXPANSE);

		const UThash hr = _UT_UTHASHTABLE_HASH(key);

		UT_ASSERT(m_bptr && m_iptr && m_nptr);
		m_bptr[m_size] = Entry(key, val);
		m_nptr[m_size] = m_iptr[hr];
		m_iptr[hr] = m_size;

		++m_size;
		return true;
	}


	UT_INLINE Pointer ptr(void)             { return m_bptr; }
	UT_INLINE ConstPointer ptr(void) const  { return m_bptr; }
	UT_INLINE bool valid(void) const        { return m_bptr != 0;}


	UT_INLINE UTsize size(void) const       { return m_size; }
	UT_INLINE UTsize capacity(void) const   { return m_capacity; }
	UT_INLINE bool empty(void) const        { return m_size == 0; }


	Iterator        iterator(void)       { return m_bptr && m_size > 0 ? Iterator(m_bptr, m_size) : Iterator(); }
	ConstIterator   iterator(void) const { return m_bptr && m_size > 0 ? ConstIterator(m_bptr, m_size) : ConstIterator(); }


	void reserve(UTsize nr)
	{
		if (m_capacity < nr && nr != UT_NPOS)
			rehash(nr);
	}

#if _UT_UTHASHTABLE_STAT == 1

	void report(void) const
	{
		if (m_capacity == 0 || m_capacity == UT_NPOS || m_size == 0)
			return;

		UT_ASSERT(m_bptr && m_iptr && m_nptr);


		UTsize min_col= m_size, max_col = 0;
		UTsize i, tot=0, avg = 0;
		for (i=0; i<m_size; ++i)
		{
			Key &key = m_bptr[i].first;

			UThash hr = _UT_UTHASHTABLE_HASH(key);

			UTsize nr = 0;

			UTsize fh = m_iptr[hr];
			while (fh != UT_NPOS && (key != m_bptr[fh].first))
			{
				fh = m_nptr[fh];
				nr++;
			}

			if (nr < min_col)
				min_col = nr;
			if (nr > max_col)
				max_col = nr;

			tot += nr;
			avg += nr ? 1 : 0;
		}

#if _UT_UTHASHTABLE_FORCE_POW2 == 1
		printf("Results using forced power of 2 expansion.\n\n");
#else
		printf("Results using unaltered expansion.\n\n");
#endif
		printf("\tTotal number of collisions %i for a table of size %i.\n\t\tusing (%s)\n", tot, m_size, typeid(Key).name());
		printf("\tThe minimum number of collisions per key: %i\n", min_col);
		printf("\tThe maximum number of collisions per key: %i\n", max_col);

		int favr = (int)(100.f * ((float)avg / (float)m_size));
		printf("\tThe average number of key collisions: %i\n\n", favr);

		if (tot == 0)
			printf("\nCongratulations lookup is 100%% linear!\n\n");
		else if (favr >  35)
			printf("\nImprove your hash function!\n\n");
	}
#endif



private:

	void doCopy(const utHashTable<Key, Value> &rhs)
	{
		if (rhs.empty())
			clear();
		else if (rhs.valid())
		{			
			reserve(rhs.m_capacity);

			UTsize i, b;
			m_size     = rhs.m_size;
			m_capacity = rhs.m_capacity;

			b = m_size > 0 ? m_size - 1 : 0;

			// Zero i & n (from end of buffer)
			for (i=b; i<m_capacity; ++i)
				m_nptr[i] = m_iptr[i] = UT_NPOS;

			for (i=0; i<m_size; ++i)
			{
				m_bptr[i] = rhs.m_bptr[i];
				m_iptr[i] = rhs.m_iptr[i];
				m_nptr[i] = rhs.m_nptr[i];
			}
		}

	}

	template<typename ArrayType>
	void reserveType(ArrayType **old, UTsize nr, bool cpy=false)
	{
		UTsize i;
		ArrayType *nar = new ArrayType[nr];
		if ((*old)!= 0)
		{
			if (cpy)
			{
				const ArrayType *oar = (*old);
				for (i = 0; i < m_size; i++) nar[i] = oar[i];
			}
			delete [](*old);
		}
		(*old) = nar;
	}


	void rehash(UTsize nr)
	{
#if _UT_UTHASHTABLE_FORCE_POW2

		if (!_UT_UTHASHTABLE_IS_POW2(nr))
		{
			_UT_UTHASHTABLE_POW2(nr);
		}

#if _UT_UTHASHTABLE_STAT_ALLOC == 1
		printf("Expanding tables: %i\n", nr);
#endif
		UT_ASSERT(_UT_UTHASHTABLE_IS_POW2(nr));


#else

#if _UT_UTHASHTABLE_STAT_ALLOC == 1
		printf("Expanding tables: %i\n", nr);
#endif

#endif

		reserveType<Entry>(&m_bptr, nr, true);
		reserveType<UTsize>(&m_iptr, nr);
		reserveType<UTsize>(&m_nptr, nr);

		m_capacity = nr;
		UT_ASSERT(m_bptr && m_iptr && m_nptr);


		UTsize i, h;
		for (i=0; i<m_capacity; ++i)
			m_iptr[i] = m_nptr[i] = UT_NPOS;

		for (i = 0; i < m_size; i++)
		{
			h = _UT_UTHASHTABLE_HASH(m_bptr[i].first);
			m_nptr[i] = m_iptr[h];
			m_iptr[h] = i;
		}
	}



	UTsize m_size, m_capacity;
	mutable UTsize m_lastPos;
	mutable UTsize m_lastKey;

	IndexArray m_iptr;
	IndexArray m_nptr;
	EntryArray m_bptr;
	UTsize m_cache;
};




UT_INLINE UThash gkHash(int v)
{
	utIntHashKey hk(v);
	return hk.hash();
}


UT_INLINE UThash gkHash(const char *v)
{
	utCharHashKey hk(v);
	return hk.hash();
}



UT_INLINE UThash gkHash(void *v)
{
	utPointerHashKey hk(v);
	return hk.hash();
}

template <typename T>
class utHashSetIterator
{
public:

	typedef typename T::Pointer         Iterator;
	typedef typename T::ValueType       &ValueType;
	typedef typename T::ConstValueType  &ConstValueType;


protected:

	mutable Iterator m_iterator;
	mutable UTsize   m_cur;
	const UTsize     m_capacity;


public:
	utHashSetIterator() : m_iterator(0), m_cur(0), m_capacity(0)  {}
	utHashSetIterator(Iterator begin, UTsize size) : m_iterator(begin), m_cur(0), m_capacity(size) { }
	utHashSetIterator(T &t) : m_iterator(t.ptr()), m_cur(0), m_capacity(t.size()) { }
	~utHashSetIterator() {}

	UT_INLINE bool             hasMoreElements(void) const  { return (m_iterator && m_cur < m_capacity); }
	UT_INLINE ValueType        getNext(void)                { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur++].second;}
	UT_INLINE ConstValueType   getNext(void) const          { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur++].second;}
	UT_INLINE void             next(void) const             { UT_ASSERT(hasMoreElements()); ++m_cur; }
	UT_INLINE ValueType        peekNext(void)               { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur].second; }
	UT_INLINE ConstValueType   peekNext(void) const         { UT_ASSERT(hasMoreElements()); return m_iterator[m_cur].second; }
};

template<typename T>
class utHashSet
{
public:

	class THashKey
	{
	protected:
		T m_key;
		mutable UThash m_cache;
	public:
		THashKey() : m_key(T()), m_cache(UT_NPOS) {}
		THashKey(const T &k) : m_key(k), m_cache(UT_NPOS) {}
		THashKey(const THashKey &k) : m_key(k.m_key), m_cache(k.m_cache) {}

		UT_INLINE UThash hash(void) const
		{
			if (m_cache != UT_NPOS) return m_cache;
			m_cache = gkHash(m_key);
			return m_cache;

		}
		UT_INLINE bool operator== (const THashKey &v) const     {return hash() == v.hash();}
		UT_INLINE bool operator!= (const THashKey &v) const     {return hash() != v.hash();}
		UT_INLINE bool operator== (const UThash &v) const       {return hash() == v;}
		UT_INLINE bool operator!= (const UThash &v) const       {return hash() != v;}
	};


	typedef utHashTable<THashKey, T>               Hash;
	typedef typename Hash::Pointer                 Pointer;
	typedef typename Hash::ConstPointer            ConstPointer;
	typedef typename Hash::ValueType               ValueType;
	typedef typename Hash::ConstValueType          ConstValueType;
	typedef utHashSetIterator<utHashSet<T> >       Iterator;
	typedef const utHashSetIterator<utHashSet<T> > ConstIterator;

public:


	utHashSet() {}
	utHashSet(const utHashSet &oth) : m_table(oth) {}
	~utHashSet() {m_table.clear();}


	void clear(bool useCache=false)
	{
		m_table.clear(useCache);
	}

	bool insert(const T &v)
	{
		return m_table.insert(v, v);
	}

	void erase(const T &v)
	{
		m_table.remove(v);
	}


	UTsize find(const T &v) { return m_table.find(v); }


	UT_INLINE T       &operator[](UTsize idx)        { UT_ASSERT(idx >= 0 && idx < size()); return m_table.at(idx); }
	UT_INLINE const T &operator[](UTsize idx) const  { UT_ASSERT(idx >= 0 && idx < size()); return m_table.at(idx); }
	UT_INLINE T       &at(UTsize idx)                { UT_ASSERT(idx >= 0 && idx < size()); return m_table.at(idx); }
	UT_INLINE const T &at(UTsize idx) const          { UT_ASSERT(idx >= 0 && idx < size()); return m_table.at(idx); }

	UT_INLINE UTsize size(void)  const              { return m_table.size(); }
	UT_INLINE bool   empty(void) const              { return m_table.empty();}
	UT_INLINE Pointer      ptr(void)                { return m_table.ptr();}
	UT_INLINE ConstPointer ptr(void) const          { return m_table.ptr();}

	UT_INLINE Iterator       iterator(void)       {return empty() ? Iterator() : Iterator(m_table.ptr(), m_table.size());}
	UT_INLINE ConstIterator  iterator(void) const
	{
		return empty() ? ConstIterator() :
		       ConstIterator(const_cast<Pointer>(m_table.ptr()), m_table.size());
	}


	utHashSet<T>& operator=(const utHashSet<T>& rhs)
	{
		if (this != &rhs)
			m_table = rhs.m_table;
		return *this;
	}


#if _UT_UTHASHTABLE_STAT == 1
	void report(void) const { m_table.report(); }
#endif

private:

	Hash m_table;

};


#endif//_utTypes_h_
