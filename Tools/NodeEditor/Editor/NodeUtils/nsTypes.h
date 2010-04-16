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
#ifndef _nsTypes_h_
#define _nsTypes_h_

#include "NodeUtils/nsCommon.h"
#include <memory.h>

// List where each link is T. Not as flexible as nsList, but requires less memory
template <typename T>
class nsListClass
{
protected:
    T *m_first;
    T *m_last;
    NSsize m_size;

public:
    typedef const T *ConstValueType;
    typedef T *ValueType;
    typedef T *Pointer;
    typedef T *ReferenceType;


    // T Links
    class Link
    {
    protected:
        T *m_next;
        T *m_prev;
        friend class nsListClass;

    public:

        Link() : m_next(0), m_prev(0) {}
        ~Link() {}

        NS_INLINE T *getNext(void) {return m_next;}
        NS_INLINE T *getPrev(void) {return m_prev;}
        NS_INLINE T *getLink(void) {return (T *)this;}
    };
public:

    // default constructor
    nsListClass() : m_first(0), m_last(0), m_size(0) {}
    ~nsListClass() { clear(); }

    // free links (no memory freed)
    void clear(void)
    {
        m_first = m_last = 0;
        m_size = 0;
    }

    // Pushes an element to the back of the list
    void push_back(ValueType v)
    {
        v->m_prev = m_last;
        if (m_last) m_last->m_next = v;
        if (!m_first) m_first = v;
        m_last = v;
        m_size ++;
    }

    // Pushes an element to the front of the list
    void push_front(ValueType v)
    {
        v->m_next = m_first;
        if (m_first) m_first->m_prev = v;
        if (!m_last) m_last = v;
        m_first = v;
        m_size ++;
    }

    // Inserts and element
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


    // find using a linear search (avoid if possible)
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


    // Gets indexed value (using a linear search (avoid if possible))
    ValueType at(NSsize index)
    {
        NS_ASSERT(index < m_size);
        NSsize i = 0;
        ValueType node = m_first;
        while (node)
        {
            if (i == index) break;
            node = node->m_next;
            i++;
        }
        NS_ASSERT(node);
        return node;
    }

    // Gets indexed value (using a linear search (avoid if possible))
    ConstValueType at(NSsize index) const
    {
        NS_ASSERT(index < m_size);
        NSsize i = 0;
        ValueType node = m_first;
        while (node)
        {
            if (i == index) break;
            node = node->m_next;
            i++;
        }
        NS_ASSERT(node);
        return node;
    }


    // Removes the element (no memory freed)
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

    NS_INLINE ValueType pop_back(void)          { return erase(end()); }
    NS_INLINE ValueType pop_front(void)         { return erase(begin()); }

    // Size queries

    NS_INLINE bool empty(void) const            { return m_size == 0; }
    NS_INLINE NSsize size(void)const            { return m_size; }


    // Safe data access

    NS_INLINE ValueType begin(void)             { return m_first; }
    NS_INLINE ValueType end(void)               { return m_last; }
    NS_INLINE ConstValueType begin(void) const  { return m_first; }
    NS_INLINE ConstValueType end(void) const    { return m_last; }
};


// List where each link is a wrapper of T. More flexible than nsListClass, but requires more memory
template <typename T>
class nsList
{
public:

    class Link
    {
    protected:
        Link     *next;
        Link     *prev;
        friend class nsList;

    public:

        Link() : next(0), prev(0) {}
        Link(const T &v) : next(0), prev(0), link(v) {}


        NS_INLINE Link *getNext(void) {return next;}
        NS_INLINE Link *getPrev(void) {return prev;}
        NS_INLINE T &getLink(void)      {return link;}
        T link;
    };

    typedef Link *Pointer;
    typedef const Link *ConstPointer;
    typedef T ValueType;
    typedef T &ReferenceType;
    typedef const T &ConstReferenceType;

    void swap(Link *a, Link *b)
    {
        ValueType v = a->link;
        a->link = b->link;
        b->link = v;
    }


public:

    // default constructor
    nsList() : m_first(0), m_last(0), m_size(0) {}
    ~nsList() { clear(); }



    // free links
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

    // Pushes an element to the back of the list
    NS_INLINE void push_back(const T &v)
    {
        Link *link = new Link(v);
        link->prev = m_last;
        if (m_last) m_last->next = link;
        if (!m_first) m_first = link;
        m_last = link;
        m_size ++;
    }

    // Pushes an element to the front of the list
    NS_INLINE void push_front(const T &v)
    {
        Link *link = new Link(v);
        link->next = m_first;
        if (m_first) m_first->prev = link;
        if (!m_last) m_last = link;
        m_first = link;
        m_size ++;
    }

    // Inserts and element
    void insert(Pointer from, const T &v)
    {
        Link *link = new Link(v);
        m_size ++;
        if (!m_first)
        {
            m_first = m_last = link;
            return;
        }
        if (from == 0)
        {
            link->next = m_first;
            link->next->prev = link;
            m_first = link;
            return;
        }
        if (m_last == from) m_last = link;
        link->next = from->next;
        from->next = link;
        if (link->next) link->next->prev = link;
        link->prev = from;
    }

    // find using a linear search (avoid if possible)
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

    // Gets indexed value (using a linear search (avoid if possible))
    ReferenceType at(NSsize index)
    {
        NS_ASSERT(index < m_size);
        NSsize i = 0;
        Link *node = m_first;
        while (node)
        {
            if (i == index) break;
            node = node->next;
            i++;
        }
        NS_ASSERT(node);
        return node->link;
    }

    // Gets indexed value (using a linear search (avoid if possible))
    ConstReferenceType at(NSsize index) const
    {
        NS_ASSERT(index < m_size);
        NSsize i = 0;
        Link *node = m_first;
        while (node)
        {
            if (i == index) break;
            node = node->next;
            i++;
        }
        NS_ASSERT(node);
        return node->link;
    }

    // Gets indexed link (using a linear search (avoid if possible))
    Pointer link_at(NSsize index)
    {
        NSsize i = 0;
        Link *node = m_first;
        while (node)
        {
            if (i == index) return node;
            node = node->next;
            i++;
        }
        return 0;
    }

    // Removes the element
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

        if (m_size == 0) clear();
    }

    void erase(T ptr)
    {

        Pointer p = find(ptr);
        if (p) erase(p);
    }


    // Bubble sort
    void sort(bool (*cmp)(const T &a, const T &b))
    {
        NSsize i, n=m_size;
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
        while (swapped && n != NS_NPOS);
    }
    void sort(bool (*cmp)(T a, T b))
    {
        NSsize i, n=m_size;
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
        while (swapped && n != NS_NPOS);
    }


    NS_INLINE void pop_back(void)       { erase(end()); }
    NS_INLINE void pop_front(void)      { erase(begin()); }

    // Size queries

    NS_INLINE bool empty(void) const    { return m_size == 0; }
    NS_INLINE NSsize size(void)const    { return m_size; }

    // Safe data access

    NS_INLINE Pointer begin(void)               { return m_first; }
    NS_INLINE Pointer end(void)                 { return m_last; }
    NS_INLINE ConstPointer begin(void)const     { return m_first; }
    NS_INLINE ConstPointer end(void)const       { return m_last; }

    NS_INLINE ReferenceType front(void) { NS_ASSERT(m_first); return m_first->link; }
    NS_INLINE ReferenceType back(void)  { NS_ASSERT(m_last); return m_last->link; }

protected:
    Link     *m_first;
    Link     *m_last;
    NSsize   m_size;
};

// List iterator access, Similar to Ogre iterator wrappers

template <typename T>
class nsListIterator
{
public:

    typedef typename T::Pointer         Iterator;
    typedef typename T::ReferenceType   ValueType;

protected:
    Iterator    m_iterator;
    Iterator    m_cur;


public:

    // Construct with list front
    nsListIterator(Iterator first) : m_iterator(first), m_cur(first) {}

    // Construct with reference to the list
    nsListIterator(T &v) : m_iterator(v.begin()), m_cur(v.begin()) { }
    ~nsListIterator() {}


    NS_INLINE bool hasMoreElements(void)    { return (m_cur != 0); }
    NS_INLINE void next(void)               { NS_ASSERT(m_cur != 0); m_cur = m_cur->getNext(); }


    NS_INLINE ValueType getNext(void)
    {
        NS_ASSERT(m_cur != 0);
        ValueType ret = m_cur->getLink();
        m_cur = m_cur->getNext();
        return ret;
    }
    NS_INLINE ValueType peekNext(void)
    {
        NS_ASSERT(m_cur != 0);
        return m_cur->getLink();
    }
};


// List reverse iterator access, Similar to Ogre iterator wrappers
template <typename T>
class nsListReverseIterator
{
public:

    typedef typename T::Pointer         Iterator;
    typedef typename T::ReferenceType   ValueType;

protected:
    Iterator    m_iterator;
    Iterator    m_cur;

public:
    // Construct with list back
    nsListReverseIterator(Iterator last) : m_iterator(last), m_cur(last) {}

    // Construct with reference to the list
    nsListReverseIterator(T &v) : m_iterator(v.end()), m_cur(v.end()) {}
    ~nsListReverseIterator() {}


    NS_INLINE bool hasMoreElements(void)    { return (m_cur != 0); }
    NS_INLINE void next(void)               { NS_ASSERT(m_cur != 0); m_cur = m_cur->getPrev(); }

    NS_INLINE ValueType getNext(void)
    {
        NS_ASSERT(m_cur != 0);
        ValueType ret = m_cur->getLink();
        m_cur = m_cur->getPrev();
        return ret;
    }

    NS_INLINE ValueType peekNext(void)
    {
        NS_ASSERT(m_cur != 0);
        return m_cur->getLink();
    }
};


// Simple array access, similar to std::vector
template <typename T>
class nsArray
{
public:
    typedef    T          *Pointer;
    typedef    const T    *ConstPointer;
    typedef    T           ValueType;

public:

    // Empty constructor
    nsArray() : m_size(0), m_capacity(0), m_data(0), m_cacheLimit(9999), m_curCache(0) {}

    // Array copy constructor (avoid where possible, pass by reference instead)
    nsArray(const nsArray<T>& o)
        : m_size(o.size()), m_capacity(0), m_data(0), m_cacheLimit(9999), m_curCache(0)
    {
        reserve(m_size);
        copy(m_data, o.m_data, m_size);
    }

    ~nsArray() { clear(); }

    // Free used buffers
    void clear(bool useCache=false)
    {
        if (!useCache)
        {
            if (m_data)
                delete []m_data;
            m_data = 0;
            m_capacity = 0;
            m_size = 0;
            m_curCache = 0;
        }
        else
        {
            // resize(0) is used a lot, so
            // to prevent array buffers form
            // becoming to big. Clear array in
            // a cycle. ( from now on use clear(true) )

            ++m_curCache;

            if (m_curCache > m_cacheLimit)
                clear(false);
            else m_size = 0;
        }
    }

    // Find using linear search
    NSsize find(const T &v)
    {
        for (NSsize i = 0; i < m_size; i++)
        {
            if (m_data[i] == v)
                return i;
        }
        return NS_NPOS;
    }


    // Appends an element to the back of the array
    NS_INLINE void push_back(const T &v)
    {
        if (m_size == m_capacity)
            reserve(m_size == 0 ? 8 : m_size * 2);

        m_data[m_size] = v;
        m_size++;
    }


    // Removes the last element
    NS_INLINE void pop_back(void)
    {
        m_size--;
        m_data[m_size].~T();
    }

    // Expands the buffer
    void resize(NSsize nr)
    {
        if (nr < m_size)
        {
            for (NSsize i = m_size; i < nr; i++)
                m_data[i].~T();
        }
        else
        {
            if (nr > m_size)
                reserve(nr);
        }
        m_size = nr;
    }


    // Expands the buffer and fills empty data
    void resize(NSsize nr, const T &fill)
    {
        if (nr < m_size)
        {
            for (NSsize i = m_size; i < nr; i++)
                m_data[i].~T();
        }
        else
        {
            if (nr > m_size)
                reserve(nr);
            for (NSsize i = m_size; i < nr; i++)
                m_data[i] = fill;

        }
        m_size = nr;
    }


    // Allocate buffers
    void reserve(NSsize nr)
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


    // Safe data access

    NS_INLINE T &operator[](NSsize idx)             { NS_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    NS_INLINE const T &operator[](NSsize idx) const { NS_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    NS_INLINE T &at(NSsize idx)                     { NS_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    NS_INLINE const T &at(NSsize idx) const         { NS_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    NS_INLINE T &front(void)                        { NS_ASSERT(m_size > 0); return m_data[0]; }
    NS_INLINE T &back(void)                         { NS_ASSERT(m_size > 0); return m_data[m_size-1]; }


    // Raw data access

    NS_INLINE ConstPointer ptr(void) const          { return m_data; }
    NS_INLINE Pointer      ptr(void)                { return m_data; }
    NS_INLINE bool         valid(void) const        { return m_data != 0;}


    // Size queries

    NS_INLINE NSsize capacity(void) const           { return m_capacity; }
    NS_INLINE NSsize size(void) const               { return m_size; }
    NS_INLINE bool empty(void) const                { return m_size == 0;}


    // Assignment operator (avoid where possible, pass by reference instead)
    nsArray<T> &operator= (const nsArray<T> &o)
    {
        clear();
        NSsize os = o.size();
        if (os > 0)
        {
            resize(os);
            copy(m_data, o.m_data, os);
        }
        return *this;
    }


    // Copy buffers
    NS_INLINE void copy(Pointer dst, ConstPointer src, NSsize size)
    {
        NS_ASSERT(size <= m_size);
        for (NSsize i = 0; i < size; i++) dst[i] = src[i];
    }

protected:
    NSsize      m_size;
    NSsize      m_capacity;
    Pointer     m_data;
    int         m_cacheLimit, m_curCache;
};



// Array iterator access, Similar to Ogre iterator wrappers
template <typename T>
class nsArrayIterator
{
public:

    typedef typename T::Pointer Iterator;

    // T::ValueType as reference
    typedef typename T::ValueType &ValueType;


protected:
    Iterator m_iterator;
    NSsize m_cur;
    NSsize m_capacity;

public:
    nsArrayIterator() {}

    // Construct with array pointer and size
    nsArrayIterator(Iterator begin, NSsize size) : m_iterator(begin), m_cur(0), m_capacity(size) {}

    // Construct with reference to the array
    nsArrayIterator(T &v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) { }
    ~nsArrayIterator() {}

    // Access queries
    NS_INLINE bool hasMoreElements(void)    { return (m_iterator && m_cur < m_capacity); }
    NS_INLINE ValueType getNext(void)       { NS_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur++]; }
    NS_INLINE void next(void)               { NS_ASSERT((m_iterator && m_cur < m_capacity)); ++m_cur;}
    NS_INLINE ValueType peekNext(void)      { NS_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur]; }
};


template <typename T>
class nsStack
{
public:
    typedef T              *Pointer;
    typedef const T        *ConstPointer;
    typedef T               ValueType;
    typedef T              &ReferenceType;
    typedef const T        &ConstReferenceType;


public:

    nsStack() : m_size(0), m_capacity(0), m_top(0), m_stack(0) {}
    nsStack(NSsize rsp) : m_size(0), m_capacity(0), m_top(0), m_stack(0) { reserve(rsp); }


    nsStack(const nsStack &o) : m_size(o.m_size), m_capacity(0), m_top(o.m_top), m_stack(0)
    {
        reserve(o.m_size);
        NS_ASSERT(m_stack && o.m_stack);
        copy(m_stack, o.m_stack, o.m_size);
    }
    ~nsStack() { clear(); }

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
        NS_ASSERT(m_top != NS_NPOS);
        if (m_size == m_capacity)
            reserve(m_size == 0 ? 8 : m_size * 2);
        m_stack[m_top] = v;
        ++m_top;
        ++m_size;
    }

    NS_INLINE ReferenceType pop(void)               { NS_ASSERT(m_top != 0); --m_size; return m_stack[--m_top]; }
    NS_INLINE ReferenceType top(void)               { NS_ASSERT(m_top != 0); return m_stack[(m_top - 1)]; }
    NS_INLINE ConstReferenceType top(void) const    { NS_ASSERT(m_top != 0); return m_stack[(m_top - 1)]; }

    NS_INLINE ReferenceType peek(NSsize offs)
    {
        NS_ASSERT(m_top != 0 && ((m_top - 1) - offs) != NS_NPOS);
        return m_stack[(m_top - 1)-offs];
    }

    NS_INLINE ConstReferenceType peek(NSsize offs) const
    {
        NS_ASSERT(m_top != 0 && ((m_top - 1) - offs) != NS_NPOS);
        return m_stack[(m_top - 1)-offs];
    }

    void reserve(NSsize nr)
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

    NS_INLINE NSsize capacity(void) const           { return m_capacity; }
    NS_INLINE NSsize size(void) const               { return m_size; }
    NS_INLINE NSsize itop(void) const               { return m_top; }
    NS_INLINE bool empty(void) const                { return m_size == 0; }
    NS_INLINE ConstPointer begin(void) const        { return m_stack; }
    NS_INLINE Pointer begin(void)                   { return m_stack; }

protected:
    NS_INLINE void copy(Pointer dest, const Pointer src, NSsize nr)
    {
        NSsize i;
        for (i = 0; i < nr; i++)
            dest[i] = src[i];
    }

    NSsize  m_size;
    NSsize  m_capacity;
    NSsize  m_top;
    Pointer m_stack;
};

template <typename T>
class nsStackIterator
{
public:
    typedef typename T::Pointer   Iterator;
    typedef typename T::ValueType ValueType;

public:

    nsStackIterator(Iterator begin, NSsize top) : m_iterator(begin), m_top(top), m_capacity(top) {}
    nsStackIterator(T &v) : m_iterator(v.begin()), m_top(v.itop()), m_capacity(v.itop()) {}

    ~nsStackIterator() {}

    NS_INLINE bool hasMoreElements(void) { return (m_iterator && m_top != 0 && m_top != NS_NPOS); }

    NS_INLINE ValueType getNext(void)
    {
        NS_ASSERT((m_iterator && (m_top - 1) != NS_NPOS));
        --m_top;
        return m_iterator[m_top];
    }

    NS_INLINE void next(void)
    {
        NS_ASSERT((m_iterator && (m_top - 1) != NS_NPOS));
        --m_top;
    }


    NS_INLINE ValueType peekNext(void)
    {
        NS_ASSERT((m_iterator && (m_top - 1) != NS_NPOS));
        return m_iterator[m_top-1];
    }

protected:
    Iterator        m_iterator;
    NSsize          m_top;
    const NSsize    m_capacity;
};


// Integer hash
class nsIntHashKey
{
protected:
    NSint32 m_key;
    mutable NShash m_hash;

public:
    nsIntHashKey() : m_key(0), m_hash(NS_NPOS) {}

    // Key Constructor
    nsIntHashKey(NSint32 k) : m_key(k), m_hash(NS_NPOS) {hash();}

    // Copy constructor
    nsIntHashKey(const nsIntHashKey &k) : m_key(k.m_key), m_hash(k.m_hash) {}

    NS_INLINE operator const int(void) const  { return m_key; }
    NS_INLINE operator int(void)              { return m_key; }

    NS_INLINE NShash hash(void) const
    {
        if (m_hash != NS_NPOS) return m_hash;
        NShash m_hash = static_cast<NShash>(m_key) * 2654435769U;
        return m_hash;
    }
    NS_INLINE bool operator== (const nsIntHashKey &v) const {return hash() == v.hash();}
    NS_INLINE bool operator!= (const nsIntHashKey &v) const {return hash() != v.hash();}
    NS_INLINE bool operator== (const NShash &v) const       {return hash() == v;}
    NS_INLINE bool operator!= (const NShash &v) const       {return hash() != v;}
};


// Pointer hash
class nsPointerHashKey
{
protected:
    void *m_key;
    mutable NShash m_hash;

public:
    nsPointerHashKey() : m_key(0), m_hash(NS_NPOS) {}

    // Key Constructor
    nsPointerHashKey(void *k) : m_key(k), m_hash(NS_NPOS) {hash();}

    // Copy constructor
    nsPointerHashKey(const nsPointerHashKey &k) : m_key(k.m_key), m_hash(k.m_hash) {}


    NS_INLINE operator const void*(void) const  { return m_key; }
    NS_INLINE operator void*(void)              { return m_key; }

    NS_INLINE NShash hash(void) const
    {
        if (m_hash != NS_NPOS) return m_hash;
        m_hash = reinterpret_cast<NShash>(m_key);
        return m_hash;
    }

    NS_INLINE bool operator== (const nsPointerHashKey &v) const {return hash() == v.hash();}
    NS_INLINE bool operator!= (const nsPointerHashKey &v) const {return hash() != v.hash();}
    NS_INLINE bool operator== (const NShash &v) const           {return hash() == v;}
    NS_INLINE bool operator!= (const NShash &v) const           {return hash() != v;}
};



// Char hash
class nsCharHashKey
{
protected:
    char *m_key;
    mutable NShash m_hash;

public:
    nsCharHashKey() : m_key(0), m_hash(NS_NPOS) {}

    // Key Constructor
    nsCharHashKey(char *k) : m_key(k), m_hash(NS_NPOS) {hash();}
    nsCharHashKey(const char *k) : m_key(const_cast<char *>(k)), m_hash(NS_NPOS) {}

    // Copy constructor
    nsCharHashKey(const nsCharHashKey &k) : m_key(k.m_key), m_hash(k.m_hash) {}

    NS_INLINE operator const char*(void) const { return m_key; }

    NShash hash(void) const
    {
        if (!m_key) return NS_NPOS;

        // use cached hash
        if (m_hash != NS_NPOS) return m_hash;

        // Lifted from btHashMap.h

        // magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
        static const unsigned int  InitialFNV = 2166136261u;
        static const unsigned int FNVMultiple = 16777619u;

        // Fowler / Noll / Vo (FNV) Hash
        m_hash = (NShash)InitialFNV;
        for (int i = 0; m_key[i]; i++)
        {
            m_hash = m_hash ^(m_key[i]);    // xor  the low 8 bits
            m_hash = m_hash * FNVMultiple;  // multiply by the magic number
        }
        return m_hash;
    }

    NS_INLINE bool operator== (const nsCharHashKey &v) const    {return hash() == v.hash();}
    NS_INLINE bool operator!= (const nsCharHashKey &v) const    {return hash() != v.hash();}
    NS_INLINE bool operator== (const NShash &v) const           {return hash() == v;}
    NS_INLINE bool operator!= (const NShash &v) const           {return hash() != v;}

};


// KeyValue pair
template<typename Key, typename Value>
struct nsHashEntry
{
    Key first;
    Value second;

    nsHashEntry() {}
    nsHashEntry(const Key &k, const Value &v) : first(k), second(v) {}

    // Assign hash entry
    NS_INLINE const nsHashEntry &operator=(const nsHashEntry &o)
    {
        first = o.first;
        second = o.second;
        return *this;
    }
};


// Simple hash map access, similar to btHashMap
// http://code.google.com/p/bullet/
template < typename Key, typename Value, NSsize table_size = 128 >
class nsHashTable
{
public:

    typedef nsHashEntry<Key, Value>     Entry;
    typedef nsArray<Entry>              EntryArray;
    typedef nsArray<NSsize>             IndexArray;


    typedef Key             KeyType;
    typedef Value           ValueType;
    typedef Value          &ReferenceValueType;
    typedef const Value    &ConstReferenceValueType;


    // Pointer access
    typedef typename EntryArray::Pointer Pointer;
    typedef typename EntryArray::ConstPointer ConstPointer;

public:

    // Empty constructor
    nsHashTable() : m_size(0), m_capacity(0), m_lastPos(NS_NPOS) {}

    // construct table with an initial size
    nsHashTable(NSsize capacity) : m_size(0), m_capacity(0), m_lastPos(NS_NPOS) { rehash(capacity ? capacity : 0); }

    // Table copy constructor (avoid where possible, pass by reference instead)
    nsHashTable(const nsHashTable &o): m_size(0), m_capacity(0), m_lastPos(NS_NPOS)
    {

        if (o.m_capacity != NS_NPOS && o.m_capacity != 0 && o.m_size > 0)
        {

            m_size = o.m_size;
            m_buckets.reserve(o.m_capacity);
            m_buckets.resize(m_size);

            Pointer dst = m_buckets.ptr();
            ConstPointer src = o.m_buckets.ptr();
            m_buckets.copy(dst, src, m_size);

            m_size += 1;
            rehash(o.m_capacity);
            m_size -= 1;
        }
    }


    ~nsHashTable() { clear(); }


    // Free used buffers
    void clear(void)
    {
        m_size = m_capacity = 0;
        m_lastPos = NS_NPOS;

        m_chainar.clear();
        m_indices.clear();
        m_buckets.clear();
    }

    // Safe data access

    ReferenceValueType at(NSsize i)                         { NS_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    ReferenceValueType operator [](NSsize i)                { NS_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    ConstReferenceValueType at(NSsize i)const               { NS_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    ConstReferenceValueType operator [](NSsize i) const     { NS_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }

    // Find and cache key
    ReferenceValueType get(const Key &key) const
    {

        NS_ASSERT(!m_buckets.empty());

        if (m_lastKey != key)
        {
            NSsize i = find(key);

            NS_ASSERT(i < m_size && i != NS_NPOS);

            m_lastKey = key;
            m_lastPos = i;
        }

        return (m_buckets.ptr()[m_lastPos]).second;
    }
   
    Entry& get(const Key &key)
    {
        NS_ASSERT(!m_buckets.empty());

        if (m_lastKey != key)
        {
            NSsize i = find(key);

            NS_ASSERT(i < m_size && i != NS_NPOS);

            m_lastKey = key;
            m_lastPos = i;
        }

        return (m_buckets.ptr()[m_lastPos]);
    }


    ReferenceValueType operator [](const Key &key) const { return get(key); }


    // Return array index to key
    NSsize find(const Key &key) const
    {

        if (m_capacity == 0 || m_capacity == NS_NPOS || m_buckets.empty())
            return NS_NPOS;

        if (m_lastPos != NS_NPOS && m_lastKey == key) return m_lastPos;
        NShash hr = key.hash() % m_capacity;

        // debug paranoia
        NS_ASSERT(m_indices.valid() && m_chainar.valid() && m_buckets.valid());

        NShash fh = m_indices[hr];
        while (fh != NS_NPOS && (key != m_buckets[fh].first))
            fh = m_chainar[fh];

        if (fh != NS_NPOS)
        {
            m_lastKey = key;
            m_lastPos = fh;
        }
        return fh;
    }

    // Removes element from the table
    void remove(const Key &key)
    {

        NSsize kp = find(key);
        if (kp == NS_NPOS || m_capacity == 0 || m_buckets.empty())
            return;

        typename IndexArray::Pointer ip = m_indices.ptr();
        typename IndexArray::Pointer cp = m_chainar.ptr();
        typename EntryArray::Pointer bp = m_buckets.ptr();

        // debug paranoia
        NS_ASSERT(m_indices.valid() && m_chainar.valid() && m_buckets.valid());


        NShash hr = key.hash() % m_capacity;
        NShash ch = ip[hr], ph = NS_NPOS;
        NS_ASSERT(ch != NS_NPOS);

        while (ch != kp)
        {
            ph = ch;
            ch = ip[ch];
        }

        if (ph != NS_NPOS)
        {
            NS_ASSERT(cp[ph] == kp);
            cp[ph] = cp[kp];
        }
        else
            ip[hr] = cp[kp];


        NSsize lp = m_size - 1;
        NS_ASSERT(lp != NS_NPOS);
        if (lp == kp)
        {
            --m_size;
            m_buckets.pop_back();
            return;
        }

        const Entry &en = bp[lp];
        hr = en.first.hash() % m_capacity;

        ch = ip[hr], ph = NS_NPOS;
        NS_ASSERT(ch != NS_NPOS);
        while (ch != lp)
        {
            ph = ch;
            ch = ip[ch];
        }

        if (ph != NS_NPOS)
        {
            NS_ASSERT(cp[ph] == lp);
            cp[ph] = cp[lp];
        }
        else
            ip[hr] = cp[lp];

        bp[kp] = bp[lp];
        cp[kp] = ip[hr];
        ip[hr] = kp;

        --m_size;
        m_buckets.pop_back();

    }

    // Appends an element to the table
    void insert(const Key &key, const Value &val)
    {

        if (m_capacity == NS_NPOS || find(key) != NS_NPOS)
            return;

        if (m_capacity == 0) rehash(table_size);

        NShash hr = key.hash() % m_capacity;
        NSsize sz = m_size++;
        if (m_size > m_capacity)
        {
            rehash(m_size * 2);
            hr = key.hash() % m_capacity;
        }

        // debug paranoia
        NS_ASSERT(m_indices.valid() && m_chainar.valid() && m_buckets.valid());

        m_buckets[sz].first  = key;
        m_buckets[sz].second = val;
        m_chainar[sz] = m_indices[hr];
        m_indices[hr] = sz;
    }

    // Raw data access

    NS_INLINE Pointer ptr(void)             { return m_buckets.ptr(); }
    NS_INLINE ConstPointer ptr(void) const  { return m_buckets.ptr(); }
    NS_INLINE bool valid(void) const        { return m_buckets.valid();}


    // Size queries

    NS_INLINE NSsize size(void) const       { return m_size; }
    NS_INLINE NSsize capacity(void) const   { return capacity; }
    NS_INLINE bool empty(void) const        { return m_size == 0; }


    // Allocate buffers
    void reserve(NSsize nr)
    {
        if (m_capacity < nr)
            rehash(nr);
    }


protected:


    // Allocate buffers and re-hash

    void rehash(NSsize nr)
    {

        if (nr < 16) nr = 16;

        m_chainar.resize(nr);
        m_indices.resize(nr);
        m_buckets.resize(nr);

        typename IndexArray::Pointer ip = m_indices.ptr();
        typename IndexArray::Pointer cp = m_chainar.ptr();

        memset(ip, NS_NPOS, sizeof(NSsize)*nr);
        memset(cp, NS_NPOS, sizeof(NSsize)*nr);

        m_capacity = nr;

        Pointer bp = m_buckets.ptr();
        ip = m_indices.ptr();
        cp = m_chainar.ptr();

        NSsize sz = (m_size - 1 != NS_NPOS) ? m_size - 1 : 0;
        for (NSsize ch = 0; ch < sz; ch++)
        {
            const Entry &he = bp[ch];
            NShash hr = he.first.hash() % m_capacity;

            cp[ch] = ip[hr];
            ip[hr] = ch;
        }
    }

    EntryArray m_buckets;
    IndexArray m_indices;
    IndexArray m_chainar;
    NSsize m_size, m_capacity;
    mutable NSsize m_lastPos;
    mutable Key m_lastKey;
};




// Table iterator access, Similar to Ogre iterator wrappers
template <typename T>
class nsHashTableIterator
{
public:
    typedef typename T::Pointer     Iterator;
    typedef typename T::Entry      &Pair;

    typedef typename T::KeyType    &KeyType;
    typedef typename T::ValueType  &ValueType;

protected:

    Iterator        m_iterator;
    NSsize          m_cur;
    const NSsize    m_capacity;

public:

    // Construct with table pointer and size
    nsHashTableIterator(Iterator begin, NSsize size) : m_iterator(begin), m_cur(0), m_capacity(size) { }

    // Construct with reference to the table
    nsHashTableIterator(T &v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) {}

    ~nsHashTableIterator() {}

    NS_INLINE bool hasMoreElements(void) const  { return (m_iterator && m_cur < m_capacity); }
    NS_INLINE Pair getNext(void)                { NS_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur++];}
    NS_INLINE void next(void)                   { NS_ASSERT((m_iterator && m_cur < m_capacity)); ++m_cur; }
    NS_INLINE Pair peekNext(void)               { NS_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur]; }
    NS_INLINE KeyType peekNextKey(void)         { NS_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur].first;}
    NS_INLINE ValueType peekNextValue(void)     { NS_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur].second; }
};

#endif//_nsTypes_h_
