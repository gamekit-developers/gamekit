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
#ifndef _utHashTable_h_
#define _utHashTable_h_

#include "utHash.h"
#include "utArray.h"

// -----------------------------------------------------------------------------
template<typename Key, typename Value>
struct utHashEntry
{
	Key first;
	Value second;

	utHashEntry() {}
	utHashEntry(const Key &k, const Value& v) : first(k), second(v) {}


	UT_INLINE const utHashEntry& operator=(const utHashEntry& o)
	{
		first = o.first;
		second= o.second;
		return *this;
	}
};

// -----------------------------------------------------------------------------
template < typename Key, typename Value, size_t table_size = 128 >
class utHashTable
{
public:

	typedef utHashEntry<Key, Value> Entry;
	typedef utArray<Entry> EntryArray;
	typedef utArray<size_t> IndexArray;

	typedef Key KeyType;
	typedef Value ValueType;

	/// Pointer access
	typedef typename EntryArray::Pointer Pointer;
	typedef typename EntryArray::ConstPointer ConstPointer;

public:

	utHashTable(size_t capacity = table_size) :
			m_size(0), m_capacity(0), m_lastPos(UT_NPOS)
	{
		if (capacity == UT_NPOS || capacity == 0)
			capacity = 256;
		rehash(capacity);
	}

	utHashTable(const utHashTable& o):
			m_size(0), m_capacity(0), m_lastPos(UT_NPOS)
	{
		if (o.m_capacity != UT_NPOS && o.m_capacity != 0 && o.m_size > 0)
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

	~utHashTable() { clear(); }

	void clear(void)
	{
		m_size = m_capacity = 0;
		m_lastPos = UT_NPOS;

		m_chainar.clear();
		m_indices.clear();
		m_buckets.clear();
	}

	ValueType at(size_t i)                      { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
	ValueType operator [](size_t i)             { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
	const ValueType at(size_t i)const           { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
	const ValueType operator [](size_t i) const { UT_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }


	ValueType get(const Key &key) const
	{
		UT_ASSERT(!m_buckets.empty());

		if (m_lastKey != key)
		{
			size_t i = find(key);
			UT_ASSERT(i < m_size && i != UT_NPOS);

			m_lastKey = key;
			m_lastPos = i;
		}

		return (m_buckets.ptr()[m_lastPos]).second;
	}

	ValueType operator [](const Key &key) const
	{
		UT_ASSERT(!m_buckets.empty());

		if (m_lastKey != key)
		{
			size_t i = find(key);
			UT_ASSERT(i < m_size && i != UT_NPOS);

			m_lastKey = key;
			m_lastPos = i;
		}

		return (m_buckets.ptr()[m_lastPos]).second;
	}

	size_t find(const Key &key) const
	{
		if (m_capacity == 0 || m_capacity == UT_NPOS || m_buckets.empty())
			return UT_NPOS;

		if (m_lastPos != UT_NPOS && m_lastKey == key)
			return m_lastPos;

		UThash hr = utTHash(key) % m_capacity;
		typename IndexArray::ConstPointer ip = m_indices.ptr();
		typename IndexArray::ConstPointer cp = m_chainar.ptr();
		typename EntryArray::ConstPointer bp = m_buckets.ptr();

		if (!ip || !cp || !bp)
			return UT_NPOS;

		UThash fh = ip[hr];
		while (fh != UT_NPOS && (key != bp[fh].first))
			fh = cp[fh];

		if (fh != UT_NPOS)
		{
			m_lastKey = key;
			m_lastPos = fh;
		}
		return fh;
	}

	void remove(const Key &key)
	{
		size_t kp = find(key);
		if (kp == UT_NPOS)
			return;

		typename IndexArray::Pointer ip = m_indices.ptr();
		typename IndexArray::Pointer cp = m_chainar.ptr();
		typename EntryArray::Pointer bp = m_buckets.ptr();

		if (!ip || !cp || !bp)
			return;

		UThash hr = utTHash(key) % m_capacity;
		UThash ch = ip[hr], ph = UT_NPOS;
		UT_ASSERT(ch != UT_NPOS);

		while (ch != kp)
		{
			ph = ch;
			ch = ip[ch];
		}

		if (ph != UT_NPOS)
		{
			UT_ASSERT(cp[ph] == kp);
			cp[ph] = cp[kp];
		}
		else
			ip[hr] = cp[kp];


		size_t lp = m_size - 1;
		UT_ASSERT(lp != UT_NPOS);
		if (lp == kp)
		{
			--m_size;
			m_buckets.pop_back();
			return;
		}

		const Entry& en = bp[lp];
		hr = utTHash(en.first) % m_capacity;

		ch = ip[hr], ph = UT_NPOS;
		UT_ASSERT(ch != UT_NPOS);
		while (ch != lp)
		{
			ph = ch;
			ch = ip[ch];
		}

		if (ph != UT_NPOS)
		{
			UT_ASSERT(cp[ph] == lp);
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
		if (m_capacity == UT_NPOS || find(key) != UT_NPOS)
			return;

		if (m_capacity == 0)
			rehash(table_size);

		UThash hr = utTHash(key) % m_capacity;
		size_t sz = m_size++;
		Entry ety(key, val);

		if (m_size > m_capacity)
		{
			rehash(m_size * 2);
			hr = utTHash(key) % m_capacity;
		}
		typename IndexArray::Pointer ip = m_indices.ptr();
		typename IndexArray::Pointer cp = m_chainar.ptr();
		typename EntryArray::Pointer bp = m_buckets.ptr();

		if (!ip || !cp || !bp)
			return;

		bp[sz] = ety;
		cp[sz] = ip[hr];
		ip[hr] = sz;
	}


	UT_INLINE Pointer ptr(void)             { return m_buckets.ptr(); }
	UT_INLINE size_t size(void) const       { return m_size; }
	UT_INLINE size_t capacity(void) const   { return capacity; }
	UT_INLINE bool empty(void) const        { return m_size == 0; }

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

		typename IndexArray::Pointer ip = m_indices.ptr();
		typename IndexArray::Pointer cp = m_chainar.ptr();

		utMemset(ip, UT_NPOS, sizeof(size_t)*nr);
		utMemset(cp, UT_NPOS, sizeof(size_t)*nr);
		if (m_capacity == 0)
			utMemset(m_buckets.ptr(), 0, sizeof(Entry)*nr);

		m_capacity = nr;

		Pointer bp = m_buckets.ptr();
		ip = m_indices.ptr();
		cp = m_chainar.ptr();

		size_t sz = (m_size - 1 != UT_NPOS) ? m_size - 1 : 0;
		for (size_t ch = 0; ch < sz; ch++)
		{
			const Entry &he = bp[ch];
			UThash hr = utTHash(he.first) % m_capacity;

			cp[ch] = ip[hr];
			ip[hr] = ch;
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

	UT_INLINE bool hasMoreElements(void)
	{
		return (m_iterator && m_cur < m_capacity);
	}


	UT_INLINE Pair getNext(void)
	{
		UT_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur++];
	}

	UT_INLINE void next(void)
	{
		UT_ASSERT((m_iterator && (m_cur) < m_capacity));
		++m_cur;
	}


	UT_INLINE Pair peekNext(void)
	{
		UT_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur+1];
	}

	UT_INLINE KeyType peekNextKey(void)
	{
		UT_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur+1].first;
	}

	UT_INLINE ValueType peekNextValue(void)
	{
		UT_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur+1].second;
	}


protected:
	Iterator    m_iterator;         // pointer access to stack
	size_t      m_cur;              // current index
	const size_t m_capacity;        // initial capacity
};

#endif//_utHashTable_h_
