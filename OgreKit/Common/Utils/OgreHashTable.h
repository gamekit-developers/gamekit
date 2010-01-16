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
#ifndef _OgreHashTable_h_
#define _OgreHashTable_h_

#include "OgreHash.h"
#include "OgreArray.h"


namespace Ogre {


template<typename Key, typename Value>
struct HashEntry
{
    Key first;
    Value second;


	HashEntry() {}
	HashEntry(const Key &k, const Value& v) : first(k), second(v) {}
	HashEntry(const HashEntry& o) : first(o.first), second(o.second)  {}

	HashEntry& operator=(const HashEntry& op)
	{
		first = op.first;
		second = op.second;
		return *this;
	}
};


template < typename Key, typename Value, size_t table_size = 128 >
class HashTable
{
public:

    typedef HashEntry<Key, Value>   Entry;
    typedef Array<Entry>            EntryArray;
    typedef Array<size_t>           IndexArray;

    typedef Key KeyType;
    typedef Value ValueType;

    /// Pointer access
    typedef typename EntryArray::Pointer Pointer;
    typedef typename EntryArray::ConstPointer ConstPointer;

public:

    HashTable(size_t capacity = table_size) :
            m_size(0), m_capacity(0), m_lastPos(OGRE_NPOS)
    {
        if (capacity == OGRE_NPOS || capacity == 0)
            capacity = 256;
        rehash(capacity);
    }

    HashTable(const HashTable& o):
            m_size(0), m_capacity(0), m_lastPos(OGRE_NPOS)
    {
        if (o.m_capacity != OGRE_NPOS && o.m_capacity != 0 && o.m_size > 0)
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

    ~HashTable() { clear(); }

    void clear(void)
    {
        m_size = m_capacity = 0;
        m_lastPos = OGRE_NPOS;

        m_chainar.clear();
        m_indices.clear();
        m_buckets.clear();
    }

    ValueType& at(size_t i)                      { OGRE_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    ValueType& operator [](size_t i)             { OGRE_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    const ValueType& at(size_t i)const           { OGRE_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
    const ValueType& operator [](size_t i) const { OGRE_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }


    ValueType& get(const Key &key) const
    {
        OGRE_ASSERT(!m_buckets.empty());

        if (m_lastKey != key)
        {
            size_t i = find(key);
            OGRE_ASSERT(i < m_size && i != OGRE_NPOS);

            m_lastKey = key;
            m_lastPos = i;
        }

        return (m_buckets.ptr()[m_lastPos]).second;
    }

    ValueType& operator [](const Key &key) const
    {
        OGRE_ASSERT(!m_buckets.empty());

        if (m_lastKey != key)
        {
            size_t i = find(key);
            OGRE_ASSERT(i < m_size && i != OGRE_NPOS);

            m_lastKey = key;
            m_lastPos = i;
        }

        return (m_buckets.ptr()[m_lastPos]).second;
    }

    size_t find(const Key &key) const
    {
        if (m_capacity == 0 || m_capacity == OGRE_NPOS || m_buckets.empty())
            return OGRE_NPOS;

        if (m_lastPos != OGRE_NPOS && m_lastKey == key)
            return m_lastPos;

        hash_t hr = OgreTHash(key) % m_capacity;

		hash_t fh = m_indices[hr];
        while (fh != OGRE_NPOS && (key != m_buckets[fh].first))
            fh = m_chainar[fh];

        if (fh != OGRE_NPOS)
        {
            m_lastKey = key;
            m_lastPos = fh;
        }
        return fh;
    }

    void remove(const Key &key)
    {
        size_t kp = find(key);
        if (kp == OGRE_NPOS)
            return;

        typename IndexArray::Pointer ip = m_indices.ptr();
        typename IndexArray::Pointer cp = m_chainar.ptr();
        typename EntryArray::Pointer bp = m_buckets.ptr();

        if (!ip || !cp || !bp)
            return;

        hash_t hr = OgreTHash(key) % m_capacity;
        hash_t ch = ip[hr], ph = OGRE_NPOS;
        OGRE_ASSERT(ch != OGRE_NPOS);

        while (ch != kp)
        {
            ph = ch;
            ch = ip[ch];
        }

        if (ph != OGRE_NPOS)
        {
            OGRE_ASSERT(cp[ph] == kp);
            cp[ph] = cp[kp];
        }
        else
            ip[hr] = cp[kp];


        size_t lp = m_size - 1;
        OGRE_ASSERT(lp != OGRE_NPOS);
        if (lp == kp)
        {
            --m_size;
            m_buckets.pop_back();
            return;
        }

        const Entry& en = bp[lp];
        hr = OgreTHash(en.first) % m_capacity;

        ch = ip[hr], ph = OGRE_NPOS;
        OGRE_ASSERT(ch != OGRE_NPOS);
        while (ch != lp)
        {
            ph = ch;
            ch = ip[ch];
        }

        if (ph != OGRE_NPOS)
        {
            OGRE_ASSERT(cp[ph] == lp);
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

    void insert(const Key& key, const Value &val)
    {
        if (m_capacity == OGRE_NPOS || find(key) != OGRE_NPOS)
            return;

        if (m_capacity == 0)
            rehash(table_size);

        hash_t hr = OgreTHash(key) % m_capacity;
        size_t sz = m_size++;

        if (m_size > m_capacity)
        {
            rehash(m_size * 2);
            hr = OgreTHash(key) % m_capacity;
        }

        m_buckets[sz] = Entry(key, val);
        m_chainar[sz] = m_indices[hr];
        m_indices[hr] = sz;
    }


    OGRE_INLINE Pointer ptr(void)             { return m_buckets.ptr(); }
    OGRE_INLINE size_t size(void) const       { return m_size; }
    OGRE_INLINE size_t capacity(void) const   { return capacity; }
    OGRE_INLINE bool empty(void) const        { return m_size == 0; }

    void reserve(size_t nr)
    {
        if (m_capacity < nr)
            rehash(nr);
    }


protected:
    void rehash(size_t nr)
    {
        m_chainar.resize(nr);
        m_indices.resize(nr);
        m_buckets.resize(nr);

		size_t i=0; 
		while (i < nr)
		{
			m_chainar[i] = OGRE_NPOS;
			m_indices[i] = OGRE_NPOS;
			++i;
		}
		m_capacity = nr;

		size_t sz = (m_size - 1 != OGRE_NPOS) ? m_size - 1 : 0;
        for (size_t ch = 0; ch < sz; ch++)
        {
            const Entry &he = m_buckets[ch];
            hash_t hr = OgreTHash(he.first) % m_capacity;

            m_chainar[ch] = m_indices[hr];
            m_indices[hr] = ch;
        }
    }

    EntryArray m_buckets;
    IndexArray m_indices;
    IndexArray m_chainar;
    size_t m_size, m_capacity;
    mutable size_t m_lastPos;
    mutable Key m_lastKey;
};


template <typename T>
class HashTableIterator
{
public:

    typedef typename T::Pointer     Iterator;
    typedef typename T::Entry&      Pair;

    typedef typename T::KeyType&    KeyType;
    typedef typename T::ValueType&  ValueType;

public:

    HashTableIterator(Iterator begin, size_t size) :
            m_iterator(begin), m_cur(0), m_capacity(size)
    {
    }

    HashTableIterator(T& v) :
            m_iterator(v.ptr()), m_cur(0), m_capacity(v.size())
    {
    }

    ~HashTableIterator() {}

    OGRE_INLINE bool hasMoreElements(void)      { return (m_iterator && m_cur < m_capacity); }
    OGRE_INLINE Pair getNext(void)              { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); return m_iterator[m_cur++];}
    OGRE_INLINE void next(void)                 { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); ++m_cur;}
    OGRE_INLINE Pair peekNext(void)             { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); return m_iterator[m_cur+1]; }
    OGRE_INLINE KeyType peekNextKey(void)       { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); return m_iterator[m_cur+1].first; }
    OGRE_INLINE ValueType peekNextValue(void)   { OGRE_ASSERT((m_iterator && (m_cur) < m_capacity)); return m_iterator[m_cur+1].second; }


protected:
    Iterator    m_iterator;
    size_t      m_cur;
	const size_t m_capacity;
};

}
#endif//_OgreHashTable_h_
