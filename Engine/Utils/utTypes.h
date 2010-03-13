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

#include "Utils/utCommon.h"
#include <memory.h>

// List where each link is T. Not as flexible as utList, but requires less memory
template <typename T>
class utListClass
{
protected:
    T* m_first;
    T* m_last;
    UTsize m_size;

public:
    typedef const T* ConstValueType;
    typedef T* ValueType;
    typedef T* Pointer;
    typedef T* ReferenceType;


    // T Links
    class Link
    {
    protected:
        T* m_next;
        T* m_prev;
        friend class utListClass;

    public:

        Link() : m_next(0), m_prev(0) {}
        ~Link() {}

        UT_INLINE T* getNext(void) {return m_next;}
        UT_INLINE T* getPrev(void) {return m_prev;}
        UT_INLINE T* getLink(void) {return (T*)this;}
    };
public:

    // default constructor
    utListClass() : m_first(0), m_last(0), m_size(0) {}
    ~utListClass() { clear(); }

    // free links (no memory freed)
    void clear(void) {
        m_first = m_last = 0;
        m_size = 0;
    }

    // Pushes an element to the back of the list
    void push_back(ValueType v) {
        v->m_prev = m_last;
        if (m_last) m_last->m_next = v;
        if (!m_first) m_first = v;
        m_last = v;
        m_size ++;
    }

    // Pushes an element to the front of the list
    void push_front(ValueType v) {
        v->m_next = m_first;
        if (m_first) m_first->m_prev = v;
        if (!m_last) m_last = v;
        m_first = v;
        m_size ++;
    }

    // Inserts and element
    void insert(ValueType from, ValueType v) {
        m_size ++;
        if (!m_first) {
            m_first = m_last = v;
            return;
        }

        if (from == 0) {
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
    ValueType find(ValueType v) {
        ValueType node = m_first;
        while (node) {
            if (node == v) return node;
            node = node->m_next;
        }
        return 0;
    }


    // Gets indexed value (using a linear search (avoid if possible))
    ValueType at(UTsize index) {
        UT_ASSERT(index < m_size);
        UTsize i = 0;
        ValueType node = m_first;
        while (node) {
            if (i == index) break;
            node = node->m_next;
            i++;
        }
        UT_ASSERT(node);
        return node;
    }

    // Gets indexed value (using a linear search (avoid if possible))
    ConstValueType at(UTsize index) const {
        UT_ASSERT(index < m_size);
        UTsize i = 0;
        ValueType node = m_first;
        while (node) {
            if (i == index) break;
            node = node->m_next;
            i++;
        }
        UT_ASSERT(node);
        return node;
    }


    // Removes the element (no memory freed)
    ValueType erase(ValueType v) {
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

    // Size queries

    UT_INLINE bool empty(void) const            { return m_size == 0; }
    UT_INLINE UTsize size(void)const            { return m_size; }


    // Safe data access

    UT_INLINE ValueType begin(void)             { UT_ASSERT(m_first); return m_first; }
    UT_INLINE ValueType end(void)               { UT_ASSERT(m_last); return m_last; }
    UT_INLINE ConstValueType begin(void) const  { UT_ASSERT(m_first); return m_first; }
    UT_INLINE ConstValueType end(void) const    { UT_ASSERT(m_last); return m_last; }
};


// List where each link is a wrapper of T. More flexible than utListClass, but requires more memory
template <typename T>
class utList
{
public:

    class Link
    {
    protected:
        Link*     next;
        Link*     prev;
        friend class utList;

    public:

        Link() : next(0), prev(0) {}
        Link(const T& v) : next(0), prev(0), link(v) {}


        UT_INLINE Link* getNext(void) {return next;}
        UT_INLINE Link* getPrev(void) {return prev;}
        UT_INLINE T& getLink(void)      {return link;}
        T link;
    };

    typedef Link* Pointer;
    typedef const Link* ConstPointer;
    typedef T ValueType;
    typedef T& ReferenceType;
    typedef const T& ConstReferenceType;

    void swap(Link *a, Link *b)
    {
        ValueType v = a->link;
        a->link = b->link;
        b->link = v;
    }


public:

    // default constructor
    utList() : m_first(0), m_last(0), m_size(0) {}
    ~utList() { clear(); }



    // free links
    void clear(void) {
        Link *node = m_first;
        while (node) {
            Link *temp = node;
            node = node->next;
            delete temp;
        }
        m_first = m_last = 0;
        m_size = 0;
    }

    // Pushes an element to the back of the list
    UT_INLINE void push_back(const T& v) {
        Link *link = new Link(v);
        link->prev = m_last;
        if (m_last) m_last->next = link;
        if (!m_first) m_first = link;
        m_last = link;
        m_size ++;
    }

    // Pushes an element to the front of the list
    UT_INLINE void push_front(const T& v) {
        Link *link = new Link(v);
        link->next = m_first;
        if (m_first) m_first->prev = link;
        if (!m_last) m_last = link;
        m_first = link;
        m_size ++;
    }

    // Inserts and element
    void insert(Pointer from, const T& v) {
        Link *link = new Link(v);
        m_size ++;
        if (!m_first) {
            m_first = m_last = link;
            return;
        }
        if (from == 0) {
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
    Pointer find(const T& v) {
        Link *node = m_first;
        while (node) {
            if (node->link == v) return node;
            node = node->next;
        }
        return 0;
    }

    // Gets indexed value (using a linear search (avoid if possible))
    ReferenceType at(UTsize index) {
        UT_ASSERT(index < m_size);
        UTsize i = 0;
        Link *node = m_first;
        while (node) {
            if (i == index) break;
            node = node->next;
            i++;
        }
        UT_ASSERT(node);
        return node->link;
    }

    // Gets indexed value (using a linear search (avoid if possible))
    ConstReferenceType at(UTsize index) const {
        UT_ASSERT(index < m_size);
        UTsize i = 0;
        Link *node = m_first;
        while (node) {
            if (i == index) break;
            node = node->next;
            i++;
        }
        UT_ASSERT(node);
        return node->link;
    }

    // Gets indexed link (using a linear search (avoid if possible))
    Pointer link_at(UTsize index) {
        UTsize i = 0;
        Link *node = m_first;
        while (node) {
            if (i == index) return node;
            node = node->next;
            i++;
        }
        return 0;
    }

    // Removes the element
    void erase(Pointer link) {
        if (!link || m_size == 0)
            return;
        if (link->next) link->next->prev = link->prev;
        if (link->prev) link->prev->next = link->next;
        if (m_last == link) m_last = link->prev;
        if (m_first == link) m_first = link->next;

        m_size -= 1;
        delete link;
    }

    // Bubble sort 
    void sort(bool (*cmp)(const T& a, const T& b))
    {
        UTsize i, n=m_size;
        Link *a;
        bool swapped = false;
        if (n < 2 || !cmp)
            return;
        do {
            a = m_first;
            for (i=0; a && a->next && i<n-1; i++, a = a->next) {
                if (cmp(a->link, a->next->link)) {
                    swap(a, a->next);
                    swapped = true;
                }
            }
            n-=1;
        } while (swapped && n != UT_NPOS);
    }

    UT_INLINE void pop_back(void)       { erase(end()); }
    UT_INLINE void pop_front(void)      { erase(begin()); }

    // Size queries

    UT_INLINE bool empty(void) const    { return m_size == 0; }
    UT_INLINE UTsize size(void)const    { return m_size; }

    // Safe data access

    UT_INLINE Pointer begin(void)               { return m_first; }
    UT_INLINE Pointer end(void)                 { return m_last; }
    UT_INLINE ConstPointer begin(void)const     { return m_first; }
    UT_INLINE ConstPointer end(void)const       { return m_last; }

    UT_INLINE ReferenceType front(void) { UT_ASSERT(m_first); return m_first->link; }
    UT_INLINE ReferenceType back(void)  { UT_ASSERT(m_last); return m_last->link; }

protected:
    Link*     m_first;
    Link*     m_last;
    UTsize      m_size;
};

// List iterator access, Similar to Ogre iterator wrappers

template <typename T>
class utListIterator
{
public:

    typedef typename T::Pointer         Iterator;
    typedef typename T::ReferenceType   ValueType;

protected:
    Iterator    m_iterator;
    Iterator    m_cur;


public:

    // Construct with list front
    utListIterator(Iterator first) : m_iterator(first), m_cur(first) {}

    // Construct with reference to the list
    utListIterator(T& v) : m_iterator(v.begin()), m_cur(v.begin()) { }
    ~utListIterator() {}


    UT_INLINE bool hasMoreElements(void)    { return (m_cur != 0); }
    UT_INLINE void next(void)               { UT_ASSERT(m_cur != 0); m_cur = m_cur->getNext(); }


    UT_INLINE ValueType getNext(void) {
        UT_ASSERT(m_cur != 0);
        ValueType ret = m_cur->getLink();
        m_cur = m_cur->getNext();
        return ret;
    }
    UT_INLINE ValueType peekNext(void) {
        UT_ASSERT(m_cur != 0);
        return m_cur->getLink();
    }
};


// List reverse iterator access, Similar to Ogre iterator wrappers
template <typename T>
class utListReverseIterator
{
public:

    typedef typename T::Pointer         Iterator;
    typedef typename T::ReferenceType   ValueType;

protected:
    Iterator    m_iterator;
    Iterator    m_cur;

public:
    // Construct with list back
    utListReverseIterator(Iterator last) : m_iterator(last), m_cur(last) {}

    // Construct with reference to the list
    utListReverseIterator(T& v) : m_iterator(v.end()), m_cur(v.end()) {}
    ~utListReverseIterator() {}


    UT_INLINE bool hasMoreElements(void)    { return (m_cur != 0); }
    UT_INLINE void next(void)               { UT_ASSERT(m_cur != 0); m_cur = m_cur->getPrev(); }

    UT_INLINE ValueType getNext(void) {
        UT_ASSERT(m_cur != 0);
        ValueType ret = m_cur->getLink();
        m_cur = m_cur->getPrev();
        return ret;
    }

    UT_INLINE ValueType peekNext(void) {
        UT_ASSERT(m_cur != 0);
        return m_cur->getLink();
    }
};


// Simple array access, similar to std::vector
template <typename T>
class utArray
{
public:
    typedef    T*          Pointer;
    typedef    const T*    ConstPointer;
    typedef    T           ValueType;

public:

    // Empty constructor
    utArray() : m_size(0), m_capacity(0), m_data(0) {}

    // Array copy constructor (avoid where possible, pass by reference instead)
    utArray(const utArray<T>& o) :
            m_size(o.size()), m_capacity(0), m_data(0) {
        reserve(m_size);
        copy(m_data, o.m_data, m_size);
    }

    ~utArray() { clear(); }

    // Free used buffers
    void clear(void) {
        if (m_data)
            delete []m_data;
        m_data = 0;
        m_capacity = 0;
        m_size = 0;
    }

    // Find using linear search
    UTsize find(const T& v) {
        for (UTsize i = 0; i < m_size; i++) {
            if (m_data[i] == v)
                return i;
        }
        return UT_NPOS;
    }


    // Appends an element to the back of the array
    UT_INLINE void push_back(const T& v) {
        if (m_size == m_capacity)
            reserve(m_size == 0 ? 8 : m_size * 2);

        m_data[m_size] = v;
        m_size++;
    }


    // Removes the last element
    UT_INLINE void pop_back(void) {
        m_size--;
        m_data[m_size].~T();
    }

    // Expands the buffer
    void resize(UTsize nr) {
        if (nr < m_size) {
            for (UTsize i = m_size; i < nr; i++)
                m_data[i].~T();
        } else {
            if (nr > m_size)
                reserve(nr);
        }
        m_size = nr;
    }


    // Expands the buffer and fills empty data
    void resize(UTsize nr, const T& fill) {
        if (nr < m_size) {
            for (UTsize i = m_size; i < nr; i++)
                m_data[i].~T();
        } else {
            if (nr > m_size)
                reserve(nr);
            for (UTsize i = m_size; i < nr; i++)
                m_data[i] = fill;

        }
        m_size = nr;
    }


    // Allocate buffers
    void reserve(UTsize nr) {

        if (m_capacity < nr) {
            T* p = new T[nr];
            if (m_data != 0) {
                copy(p, m_data, m_size);
                delete []m_data;
            }
            m_data = p;
            m_capacity = nr;
        }
    }


    // Safe data access

    UT_INLINE T& operator[](UTsize idx)             { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    UT_INLINE const T& operator[](UTsize idx) const { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    UT_INLINE T& at(UTsize idx)                     { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    UT_INLINE const T& at(UTsize idx) const         { UT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
    UT_INLINE T& front(void)                        { UT_ASSERT(m_size > 0); return m_data[0]; }
    UT_INLINE T& back(void)                         { UT_ASSERT(m_size > 0); return m_data[m_size-1]; }


    // Raw data access

    UT_INLINE ConstPointer ptr(void) const          { return m_data; }
    UT_INLINE Pointer      ptr(void)                { return m_data; }
    UT_INLINE bool         valid(void) const        { return m_data != 0;}


    // Size queries

    UT_INLINE UTsize capacity(void) const           { return m_capacity; }
    UT_INLINE UTsize size(void) const               { return m_size; }
    UT_INLINE bool empty(void) const                { return m_size == 0;}


    // Assignment operator (avoid where possible, pass by reference instead)
    utArray<T> &operator= (const utArray<T> &o) {
        clear();
        UTsize os = o.size();
        if (os > 0) {
            resize(os);
            copy(m_data, o.m_data, os);
        }
        return *this;
    }


    // Copy buffers
    UT_INLINE void copy(Pointer dst, ConstPointer src, UTsize size) {
        UT_ASSERT(size <= m_size);
        for (UTsize i = 0; i < size; i++) dst[i] = src[i];
    }

protected:
    UTsize      m_size;
    UTsize      m_capacity;
    Pointer     m_data;
};



// Array iterator access, Similar to Ogre iterator wrappers
template <typename T>
class utArrayIterator
{
public:

    typedef typename T::Pointer Iterator;

    // T::ValueType as reference
    typedef typename T::ValueType& ValueType;


protected:
    Iterator m_iterator;
    UTsize m_cur;
    UTsize m_capacity;

public:
    utArrayIterator() {}

    // Construct with array pointer and size
    utArrayIterator(Iterator begin, UTsize size) : m_iterator(begin), m_cur(0), m_capacity(size) {}

    // Construct with reference to the array
    utArrayIterator(T& v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) { UT_ASSERT(v.valid()); }
    ~utArrayIterator() {}

    // Access queries
    UT_INLINE bool hasMoreElements(void)    { return (m_iterator && m_cur < m_capacity); }
    UT_INLINE ValueType getNext(void)       { UT_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur++]; }
    UT_INLINE void next(void)               { UT_ASSERT((m_iterator && m_cur < m_capacity)); ++m_cur;}
    UT_INLINE ValueType peekNext(void)      { UT_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur]; }
};


template <typename T>
class utStack
{
public:
    typedef T*              Pointer;
    typedef const T*        ConstPointer;
    typedef T               ValueType;
    typedef T&              ReferenceType;
    typedef const T&        ConstReferenceType;


public:

    utStack() : m_size(0), m_capacity(0), m_top(0), m_stack(0) {}
    utStack(UTsize rsp) : m_size(0), m_capacity(0), m_top(0), m_stack(0) { reserve(rsp); }


    utStack(const utStack& o) : m_size(o.m_size), m_capacity(0), m_top(o.m_top), m_stack(0) {
        reserve(o.m_size);
        UT_ASSERT(m_stack && o.m_stack);
        copy(m_stack, o.m_stack, o.m_size);
    }
    ~utStack() { clear(); }

    void clear(void) {
        if (m_stack != 0) {
            delete []m_stack;
            m_stack = 0;
        }

        m_size = 0;
        m_capacity = 0;
        m_top = 0;
    }


    void push(const T& v) {
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

    UT_INLINE ReferenceType peek(UTsize offs) {
        UT_ASSERT(m_top != 0 && ((m_top - 1) - offs) != UT_NPOS);
        return m_stack[(m_top - 1)-offs];
    }

    UT_INLINE ConstReferenceType peek(UTsize offs) const {
        UT_ASSERT(m_top != 0 && ((m_top - 1) - offs) != UT_NPOS);
        return m_stack[(m_top - 1)-offs];
    }

    void reserve(UTsize nr) {
        if (m_capacity < nr) {
            T* temp = new T[nr];
            if (m_stack) {
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
    UT_INLINE ConstPointer begin(void) const        { return m_stack; }
    UT_INLINE Pointer begin(void)                   { return m_stack; }

protected:
    UT_INLINE void copy(Pointer dest, const Pointer src, UTsize nr) {
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
class utStackIterator
{
public:
    typedef typename T::Pointer   Iterator;
    typedef typename T::ValueType ValueType;

public:

    utStackIterator(Iterator begin, UTsize top) : m_iterator(begin), m_top(top), m_capacity(top) {}
    utStackIterator(T& v) : m_iterator(v.begin()), m_top(v.itop()), m_capacity(v.itop()) {}

    ~utStackIterator() {}

    UT_INLINE bool hasMoreElements(void) { return (m_iterator && m_top != 0 && m_top != UT_NPOS); }

    UT_INLINE ValueType getNext(void) {
        UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
        --m_top;
        return m_iterator[m_top];
    }

    UT_INLINE void next(void) {
        UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
        --m_top;
    }


    UT_INLINE ValueType peekNext(void) {
         UT_ASSERT((m_iterator && (m_top - 1) != UT_NPOS));
        return m_iterator[m_top-1];
    }

protected:
    Iterator        m_iterator;
    UTsize          m_top;
    const UTsize    m_capacity;
};


// Integer hash
class utIntHashKey
{
protected:
    UTint32 m_key;
    mutable UThash m_hash;

public:
    utIntHashKey() : m_key(0), m_hash(UT_NPOS) {}

    // Key Constructor
    utIntHashKey(UTint32 k) : m_key(k), m_hash(UT_NPOS) {hash();}

    // Copy constructor
    utIntHashKey(const utIntHashKey& k) : m_key(k.m_key), m_hash(k.m_hash) {}


    UT_INLINE UThash hash(void) const {
        if (m_hash != UT_NPOS) return m_hash;
        UThash m_hash = static_cast<UThash>(m_key) * 2654435769U;
        return m_hash;
    }
    UT_INLINE bool operator== (const utIntHashKey& v) const {return hash() == v.hash();}
    UT_INLINE bool operator!= (const utIntHashKey& v) const {return hash() != v.hash();}
    UT_INLINE bool operator== (const UThash& v) const       {return hash() == v;}
    UT_INLINE bool operator!= (const UThash& v) const       {return hash() != v;}
};


// Pointer hash
class utPointerHashKey
{
protected:
    void* m_key;
    mutable UThash m_hash;

public:
    utPointerHashKey() : m_key(0), m_hash(UT_NPOS) {}

    // Key Constructor
    utPointerHashKey(void* k) : m_key(k), m_hash(UT_NPOS) {hash();}

    // Copy constructor
    utPointerHashKey(const utPointerHashKey& k) : m_key(k.m_key), m_hash(k.m_hash) {}

    UT_INLINE void          *key(void)       {return m_key;}
    UT_INLINE const void    *key(void) const {return m_key;}

    UT_INLINE UThash hash(void) const {
        if (m_hash != UT_NPOS) return m_hash;
        m_hash = static_cast<UThash>(reinterpret_cast<UTuintPtr>(m_key));
        return m_hash;
    }

    UT_INLINE bool operator== (const utPointerHashKey& v) const {return hash() == v.hash();}
    UT_INLINE bool operator!= (const utPointerHashKey& v) const {return hash() != v.hash();}
    UT_INLINE bool operator== (const UThash& v) const           {return hash() == v;}
    UT_INLINE bool operator!= (const UThash& v) const           {return hash() != v;}
};



// Char hash
class utCharHashKey
{
protected:
    char* m_key;
    mutable UThash m_hash;

public:
    utCharHashKey() : m_key(0), m_hash(UT_NPOS) {}

    // Key Constructor
    utCharHashKey(char* k) : m_key(k), m_hash(UT_NPOS) {hash();}
    utCharHashKey(const char* k) : m_key(const_cast<char*>(k)), m_hash(UT_NPOS) {}

    // Copy constructor
    utCharHashKey(const utCharHashKey& k) : m_key(k.m_key), m_hash(k.m_hash) {}


    UThash hash(void) const {
        if (!m_key) return UT_NPOS;

        // use cached hash
        if (m_hash != UT_NPOS) return m_hash;

        // Lifted from btHashMap.h

        // magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
        static const unsigned int  InitialFNV = 2166136261u;
        static const unsigned int FNVMultiple = 16777619u;

        // Fowler / Noll / Vo (FNV) Hash
        m_hash = (UThash)InitialFNV;
        for (int i = 0; m_key[i]; i++) {
            m_hash = m_hash ^(m_key[i]);    // xor  the low 8 bits
            m_hash = m_hash * FNVMultiple;  // multiply by the magic number
        }
        return m_hash;
    }

    UT_INLINE bool operator== (const utCharHashKey& v) const    {return hash() == v.hash();}
    UT_INLINE bool operator!= (const utCharHashKey& v) const    {return hash() != v.hash();}
    UT_INLINE bool operator== (const UThash& v) const           {return hash() == v;}
    UT_INLINE bool operator!= (const UThash& v) const           {return hash() != v;}

};


// KeyValue pair
template<typename Key, typename Value>
struct utHashEntry {
    Key first;
    Value second;

    utHashEntry() {}
    utHashEntry(const Key &k, const Value& v) : first(k), second(v) {}

    // Assign hash entry
    UT_INLINE const utHashEntry& operator=(const utHashEntry& o) {
        first = o.first;
        second = o.second;
        return *this;
    }
};


// Simple hash map access, similar to btHashMap
// http://code.google.com/p/bullet/
template < typename Key, typename Value, UTsize table_size = 128 >
class utHashTable
{
public:

    typedef utHashEntry<Key, Value>     Entry;
    typedef utArray<Entry>              EntryArray;
    typedef utArray<UTsize>             IndexArray;


    typedef Key             KeyType;
    typedef Value           ValueType;
    typedef Value&          ReferenceValueType;
    typedef const Value&    ConstReferenceValueType;


    // Pointer access
    typedef typename EntryArray::Pointer Pointer;
    typedef typename EntryArray::ConstPointer ConstPointer;

public:

    // Empty constructor
    utHashTable() : m_size(0), m_capacity(0), m_lastPos(UT_NPOS) {}

    // construct table with an initial size
    utHashTable(UTsize capacity) : m_size(0), m_capacity(0), m_lastPos(UT_NPOS) { rehash(capacity ? capacity : 0); }

    // Table copy constructor (avoid where possible, pass by reference instead)
    utHashTable(const utHashTable& o): m_size(0), m_capacity(0), m_lastPos(UT_NPOS) {

        if (o.m_capacity != UT_NPOS && o.m_capacity != 0 && o.m_size > 0) {

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


    ~utHashTable() { clear(); }


    // Free used buffers
    void clear(void) {
        m_size = m_capacity = 0;
        m_lastPos = UT_NPOS;

        m_chainar.clear();
        m_indices.clear();
        m_buckets.clear();
    }

    // Safe data access

    ReferenceValueType at(UTsize i)                         { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    ReferenceValueType operator [](UTsize i)                { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    ConstReferenceValueType at(UTsize i)const               { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    ConstReferenceValueType operator [](UTsize i) const     { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }

    // Find and cache key
    ReferenceValueType get(const Key &key) const {

        UT_ASSERT(!m_buckets.empty());

        if (m_lastKey != key) {
            UTsize i = find(key);

            UT_ASSERT(i < m_size && i != UT_NPOS);

            m_lastKey = key;
            m_lastPos = i;
        }

        return (m_buckets.ptr()[m_lastPos]).second;
    }

    ReferenceValueType operator [](const Key &key) const { return get(key); }


    // Return array index to key
    UTsize find(const Key &key) const {

        if (m_capacity == 0 || m_capacity == UT_NPOS || m_buckets.empty())
            return UT_NPOS;

        if (m_lastPos != UT_NPOS && m_lastKey == key) return m_lastPos;
        UThash hr = key.hash() % m_capacity;

        // debug paranoia
        UT_ASSERT(m_indices.valid() && m_chainar.valid() && m_buckets.valid());

        UThash fh = m_indices[hr];
        while (fh != UT_NPOS && (key != m_buckets[fh].first))
            fh = m_chainar[fh];

        if (fh != UT_NPOS) {
            m_lastKey = key;
            m_lastPos = fh;
        }
        return fh;
    }

    // Removes element from the table
    void remove(const Key &key) {

        UTsize kp = find(key);
        if (kp == UT_NPOS || m_capacity == 0 || m_buckets.empty())
            return;

        typename IndexArray::Pointer ip = m_indices.ptr();
        typename IndexArray::Pointer cp = m_chainar.ptr();
        typename EntryArray::Pointer bp = m_buckets.ptr();

        // debug paranoia
        UT_ASSERT(m_indices.valid() && m_chainar.valid() && m_buckets.valid());


        UThash hr = key.hash() % m_capacity;
        UThash ch = ip[hr], ph = UT_NPOS;
        UT_ASSERT(ch != UT_NPOS);

        while (ch != kp) {
            ph = ch;
            ch = ip[ch];
        }

        if (ph != UT_NPOS) {
            UT_ASSERT(cp[ph] == kp);
            cp[ph] = cp[kp];
        } else
            ip[hr] = cp[kp];


        UTsize lp = m_size - 1;
        UT_ASSERT(lp != UT_NPOS);
        if (lp == kp) {
            --m_size;
            m_buckets.pop_back();
            return;
        }

        const Entry& en = bp[lp];
        hr = en.first.hash() % m_capacity;

        ch = ip[hr], ph = UT_NPOS;
        UT_ASSERT(ch != UT_NPOS);
        while (ch != lp) {
            ph = ch;
            ch = ip[ch];
        }

        if (ph != UT_NPOS) {
            UT_ASSERT(cp[ph] == lp);
            cp[ph] = cp[lp];
        } else
            ip[hr] = cp[lp];

        bp[kp] = bp[lp];
        cp[kp] = ip[hr];
        ip[hr] = kp;

        --m_size;
        m_buckets.pop_back();

    }

    // Appends an element to the table
    void insert(const Key& key, const Value &val) {

        if (m_capacity == UT_NPOS || find(key) != UT_NPOS)
            return;

        if (m_capacity == 0) rehash(table_size);

        UThash hr = key.hash() % m_capacity;
        UTsize sz = m_size++;
        if (m_size > m_capacity) {
            rehash(m_size * 2);
            hr = key.hash() % m_capacity;
        }

        // debug paranoia
        UT_ASSERT(m_indices.valid() && m_chainar.valid() && m_buckets.valid());

        m_buckets[sz].first  = key;
        m_buckets[sz].second = val;
        m_chainar[sz] = m_indices[hr];
        m_indices[hr] = sz;
    }

    // Raw data access

    UT_INLINE Pointer ptr(void)             { return m_buckets.ptr(); }
    UT_INLINE ConstPointer ptr(void) const  { return m_buckets.ptr(); }
    UT_INLINE bool valid(void) const        { return m_buckets.valid();}


    // Size queries

    UT_INLINE UTsize size(void) const       { return m_size; }
    UT_INLINE UTsize capacity(void) const   { return capacity; }
    UT_INLINE bool empty(void) const        { return m_size == 0; }


    // Allocate buffers
    void reserve(UTsize nr) {
        if (m_capacity < nr)
            rehash(nr);
    }


protected:


    // Allocate buffers and re-hash

    void rehash(UTsize nr) {

        if (nr < 16) nr = 16;

        m_chainar.resize(nr);
        m_indices.resize(nr);
        m_buckets.resize(nr);

        typename IndexArray::Pointer ip = m_indices.ptr();
        typename IndexArray::Pointer cp = m_chainar.ptr();

        memset(ip, UT_NPOS, sizeof(UTsize)*nr);
        memset(cp, UT_NPOS, sizeof(UTsize)*nr);

        m_capacity = nr;

        Pointer bp = m_buckets.ptr();
        ip = m_indices.ptr();
        cp = m_chainar.ptr();

        UTsize sz = (m_size - 1 != UT_NPOS) ? m_size - 1 : 0;
        for (UTsize ch = 0; ch < sz; ch++) {
            const Entry &he = bp[ch];
            UThash hr = he.first.hash() % m_capacity;

            cp[ch] = ip[hr];
            ip[hr] = ch;
        }
    }

    EntryArray m_buckets;
    IndexArray m_indices;
    IndexArray m_chainar;
    UTsize m_size, m_capacity;
    mutable UTsize m_lastPos;
    mutable Key m_lastKey;
};




// Table iterator access, Similar to Ogre iterator wrappers
template <typename T>
class utHashTableIterator
{
public:
    typedef typename T::Pointer     Iterator;
    typedef typename T::Entry&      Pair;

    typedef typename T::KeyType&    KeyType;
    typedef typename T::ValueType&  ValueType;

protected:

    Iterator        m_iterator;
    UTsize          m_cur;
    const UTsize    m_capacity;

public:

    // Construct with table pointer and size
    utHashTableIterator(Iterator begin, UTsize size) : m_iterator(begin), m_cur(0), m_capacity(size) { }

    // Construct with reference to the table
    utHashTableIterator(T& v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) {}

    ~utHashTableIterator() {}

    UT_INLINE bool hasMoreElements(void) const  { return (m_iterator && m_cur < m_capacity); }
    UT_INLINE Pair getNext(void)                { UT_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur++];}
    UT_INLINE void next(void)                   { UT_ASSERT((m_iterator && m_cur < m_capacity)); ++m_cur; }
    UT_INLINE Pair peekNext(void)               { UT_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur]; }
    UT_INLINE KeyType peekNextKey(void)         { UT_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur].first;}
    UT_INLINE ValueType peekNextValue(void)     { UT_ASSERT((m_iterator && m_cur < m_capacity)); return m_iterator[m_cur].second; }
};

#endif//_utTypes_h_
