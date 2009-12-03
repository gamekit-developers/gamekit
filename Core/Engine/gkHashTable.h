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
#ifndef _gkHashTable_h_
#define _gkHashTable_h_

#include <memory.h>
#include "gkHash.h"
#include "gkArray.h"




// ----------------------------------------------------------------------------
template<typename Key, typename Value>
struct gkHashEntry
{
	Key			first;
	Value		second;
};


// ----------------------------------------------------------------------------
template<typename Key, typename Value, size_t table_size=128>
class gkHashTable
{
public:
	typedef gkHashEntry<Key, Value>		Entry;		/// Current data type
	typedef gkArray<Entry>				EntryArray; /// current entries
	typedef gkArray<size_t>				IndexArray; /// indexed
	typedef Key							KeyType;
	typedef Value						ValueType;

	/// Pointer access
	typedef typename EntryArray::Pointer		Pointer;
	typedef typename EntryArray::ConstPointer   ConstPointer;

public:

	gkHashTable(size_t capacity= table_size) :
			m_size(0), m_capacity(0), m_lastPos(GK_NPOS)
	{
		if (capacity == GK_NPOS || capacity == 0)
			capacity= 256;
		rehash(capacity);
	}


	gkHashTable(const gkHashTable& o):
			m_size(0), m_capacity(0), m_lastPos(GK_NPOS)
	{
		/// Avoid if possible...
		if (o.m_capacity != GK_NPOS && o.m_capacity != 0 && o.m_size > 0)
		{
			m_size= o.m_size;

			/// size is not to be trusted
			/// in the bucket array so copy manually

			m_buckets.reserve(o.m_capacity);
			m_buckets.resize(m_size);

			Pointer dst= m_buckets.ptr();
			ConstPointer src= o.m_buckets.ptr();

			m_buckets.copy(dst, src, m_size);

			// rehash uses size - 1
			// so temporarily add one
			m_size += 1;

			/// gkHash buffers
			rehash(o.m_capacity);
			m_size -= 1;
		}
	}

	~gkHashTable()
	{
		// tidy
		clear();
	}

	void clear(void)
	{
		m_size= 0;
		m_chainar.clear();
		m_indices.clear();
		m_buckets.clear();
	}

	GK_INLINE ValueType at(size_t i)						{ GK_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
	GK_INLINE ValueType operator [](size_t i)				{ GK_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
	GK_INLINE const ValueType at(size_t i)const				{ GK_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }
	GK_INLINE const ValueType operator [](size_t i) const	{ GK_ASSERT(i < m_size); return (m_buckets.ptr()[i]).second; }


	ValueType get(const Key &key) const
	{
		/// key lookup
		if (m_lastKey != key)
		{
			size_t i= find(key);
			GK_ASSERT(i < m_size && i != GK_NPOS);

			m_lastKey= key;
			m_lastPos= i;
		}

		return (m_buckets.ptr()[m_lastPos]).second;
	}

	ValueType operator [](const Key &key) const
	{
		/// key lookup
		if (m_lastKey != key)
		{
			size_t i= find(key);
			GK_ASSERT(i < m_size && i != GK_NPOS);

			m_lastKey= key;
			m_lastPos= i;
		}

		return (m_buckets.ptr()[m_lastPos]).second;
	}

	size_t find(const Key &key) const
	{
		if (m_capacity == 0 || m_capacity== GK_NPOS)
			return GK_NPOS;

		if (m_lastPos != GK_NPOS && m_lastKey == key)
			return m_lastPos;


		GKhash hr= gkHashT(key) % m_capacity;
		typename IndexArray::ConstPointer ip= m_indices.ptr();
		typename IndexArray::ConstPointer cp= m_chainar.ptr();
		typename EntryArray::ConstPointer bp= m_buckets.ptr();

		if (!ip || !cp || !bp)
			return GK_NPOS;

		GKhash fh= ip[hr];
		while (fh != GK_NPOS && (key != bp[fh].first))
			fh= cp[fh];

		if (fh != GK_NPOS)
		{
			m_lastKey= key;
			m_lastPos= fh;
		}
		return fh;
	}

	void remove(const Key &key)
	{
		size_t kp= find(key);
		if (kp == GK_NPOS)
			return;

		typename IndexArray::Pointer ip= m_indices.ptr();
		typename IndexArray::Pointer cp= m_chainar.ptr();
		typename EntryArray::Pointer bp= m_buckets.ptr();

		if (!ip || !cp || !bp)
			return;

		GKhash hr= gkHashT(key) % m_capacity;
		GKhash ch= ip[hr], ph=GK_NPOS;

		GK_ASSERT(ch != GK_NPOS);

		while (ch != kp)
		{
			ph= ch;
			ch= ip[ch];
		}

		if (ph != GK_NPOS)
		{
			GK_ASSERT(cp[ph] == kp);
			cp[ph]= cp[kp];
		}
		else
			ip[hr]= cp[kp];


		size_t lp= m_size - 1;

		GK_ASSERT(lp != GK_NPOS);
		if (lp == kp)
		{
			--m_size;
			m_buckets.pop_back();
			return;
		}

		const Entry& en= bp[lp];
		hr= gkHashT(en.first) % m_capacity;

		ch= ip[hr], ph= GK_NPOS;

		GK_ASSERT(ch != GK_NPOS);
		while (ch != lp)
		{
			ph= ch;
			ch= ip[ch];
		}

		if (ph != GK_NPOS)
		{
			GK_ASSERT(cp[ph] == lp);
			cp[ph]= cp[lp];
		}
		else
			ip[hr]= cp[lp];

		bp[kp]=bp[lp];
		cp[kp]=ip[hr];
		ip[hr]=kp;

		--m_size;
		m_buckets.pop_back();

	}

	void insert(const Key& key, const Value &val)
	{
		if (m_capacity == 0 || m_capacity== GK_NPOS || find(key) != GK_NPOS)
			return;

		GKhash hr= gkHashT(key) % m_capacity;

		size_t sz= m_size++;
		Entry ety= {key, val};

		if (m_size > m_capacity || sz == 0)
		{
			rehash(sz == 0 ? 8 : m_size * 2);
			hr= gkHashT(key) % m_capacity;
		}

		typename IndexArray::Pointer ip= m_indices.ptr();
		typename IndexArray::Pointer cp= m_chainar.ptr();
		typename EntryArray::Pointer bp= m_buckets.ptr();

		if (!ip || !cp || !bp)
			return;

		bp[sz]= ety;
		cp[sz]= ip[hr];
		ip[hr]= sz;
	}


	GK_INLINE Pointer ptr(void)				{ return m_buckets.ptr(); }
	GK_INLINE size_t size(void) const		{ return m_size; }
	GK_INLINE size_t capacity(void) const   { return capacity; }
	GK_INLINE bool empty(void) const		{ return m_size == 0; }

	void reserve(size_t nr)
	{
		if (m_capacity < nr)
			rehash(nr);
	}


protected:



	void rehash(size_t nr)
	{
		/// grow tables to match nr

		m_chainar.resize(nr);
		m_indices.resize(nr);
		m_buckets.resize(nr);

		m_capacity= nr;

		typename IndexArray::Pointer ip= m_indices.ptr();
		typename IndexArray::Pointer cp= m_chainar.ptr();

		memset(ip, GK_NPOS, sizeof(size_t)*nr);
		memset(cp, GK_NPOS, sizeof(size_t)*nr);

		Pointer bp= m_buckets.ptr();
		ip= m_indices.ptr();
		cp= m_chainar.ptr();

		size_t sz= (m_size - 1 != GK_NPOS) ? m_size - 1 : 0;
		for (size_t ch=0; ch<sz; ch++)
		{
			const Entry &he= bp[ch];
			GKhash hr= gkHashT(he.first) % m_capacity;

			cp[ch]= ip[hr];
			ip[hr]= ch;
		}
	}

	EntryArray m_buckets;
	IndexArray m_indices;
	IndexArray m_chainar;
	size_t  m_size, m_capacity;
	mutable size_t  m_lastPos;
	mutable Key m_lastKey;
};




// ----------------------------------------------------------------------------
template <typename T>
class gkHashTableIterator
{
public:

	typedef typename T::Pointer	 Iterator;
	typedef typename T::Entry&	  Pair;

	typedef typename T::KeyType&	KeyType;
	typedef typename T::ValueType&  ValueType;

public:

	gkHashTableIterator(Iterator begin, size_t size) :
			m_iterator(begin), m_cur(0), m_capacity(size)
	{
	}

	gkHashTableIterator(T& v) :
			m_iterator(v.ptr()), m_cur(0), m_capacity(v.size())
	{
	}

	~gkHashTableIterator() {}

	GK_INLINE bool hasMoreElements(void)
	{
		return (m_iterator && m_cur < m_capacity);
	}


	GK_INLINE Pair getNext(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur++];
	}

	GK_INLINE void next(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		++m_cur;
	}


	GK_INLINE Pair peekNext(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur];
	}

	GK_INLINE KeyType peekNextKey(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur].first;
	}


	GK_INLINE ValueType peekNextValue(void)
	{
		GK_ASSERT((m_iterator && (m_cur) < m_capacity));
		return m_iterator[m_cur].second;
	}

protected:
	Iterator	m_iterator;		 // pointer access to stack
	size_t	  m_cur;			  // current index
	const size_t m_capacity;		// initial capacity
};




#endif//_gkHashTable_h_
