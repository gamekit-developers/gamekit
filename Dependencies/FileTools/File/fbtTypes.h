/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

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
#ifndef _fbtTypes_h_
#define _fbtTypes_h_

#ifndef FBT_HAVE_CONFIG
#include "fbtConfig.h"
#endif

#include <string.h> //memcmp


/** \addtogroup FBT
*  @{
*/

#if (defined(DEBUG) || defined(_DEBUG)) && FBT_USE_DEBUG_ASSERTS == 1
# include <assert.h> // Keep this the only std include in headers
# define FBT_DEBUG 1
# define FBT_ASSERT(x) assert(x)
#else
# define FBT_ASSERT(x) ((void)0)
#endif

#ifdef FBT_USE_COMPILER_CHECKS
#define FBT_ASSERTCOMP(x, n) typedef bool x[(n) ? 1 : -1];
#else
#define FBT_ASSERTCOMP(x, n)
#endif


#define FBT_PLATFORM_WIN32    0
#define FBT_PLATFORM_LINUX    2
#define FBT_PLATFORM_APPLE    3

#if defined (_WIN32)
# define FBT_PLATFORM FBT_PLATFORM_WIN32
#elif defined(__APPLE__)
# define FBT_PLATFORM FBT_PLATFORM_APPLE
#else
# define FBT_PLATFORM FBT_PLATFORM_LINUX
#endif

#define FBT_COMPILER_MSVC    0
#define FBT_COMPILER_GNU     1

#if defined(_MSC_VER)
# define FBT_COMPILER FBT_COMPILER_MSVC
#elif defined(__GNUC__)
# define FBT_COMPILER FBT_COMPILER_GNU
#else
# error unknown compiler
#endif

#define FBT_ENDIAN_LITTLE    0
#define FBT_ENDIAN_BIG       1

#if defined(__sgi)      ||  defined (__sparc)        || \
    defined (__sparc__) ||  defined (__PPC__)        || \
    defined (__ppc__)   ||  defined (__BIG_ENDIAN__)
#define FBT_ENDIAN FBT_ENDIAN_BIG
#else
#define FBT_ENDIAN FBT_ENDIAN_LITTLE
#endif

#if FBT_ENDIAN == FBT_ENDIAN_BIG
# define FBT_ID(a,b,c,d) ( (int)(a)<<24 | (int)(b)<<16 | (c)<<8 | (d) )
# define FBT_ID2(c, d)   ( (c)<<8 | (d) )
#else
# define FBT_ID(a,b,c,d) ( (int)(d)<<24 | (int)(c)<<16 | (b)<<8 | (a) )
# define FBT_ID2(c, d)   ( (d)<<8 | (c) )
#endif

#define FBT_ARCH_32 0
#define FBT_ARCH_64 1

#if defined(__x86_64__)     || defined(_M_X64)      || \
    defined(__powerpc64__)  || defined(__alpha__)   || \
    defined(__ia64__)       || defined(__s390__)    || \
    defined(__s390x__)
#define FBT_ARCH FBT_ARCH_64
#else
#define FBT_ARCH FBT_ARCH_32
#endif


#if FBT_PLATFORM == FBT_PLATFORM_WIN32
# if defined(__MINGW32__) || \
     defined(__CYGWIN__)  || \
     (defined (_MSC_VER) && _MSC_VER < 1300)
#  define FBT_INLINE inline
# else
#  define FBT_INLINE __forceinline
# endif
#else
#  define FBT_INLINE    inline
#endif

// Integer types
typedef long            FBTlong;
typedef unsigned long   FBTulong;
typedef int             FBTint32;
typedef unsigned int    FBTuint32;
typedef short           FBTint16;
typedef unsigned short  FBTuint16;
typedef signed char     FBTint8;
typedef unsigned char   FBTuint8;
typedef unsigned char   FBTubyte;
typedef signed char     FBTbyte;
typedef bool            FBTint1;


#if FBT_COMPILER == FBT_COMPILER_GNU
typedef long long          FBTint64;
typedef unsigned long long FBTuint64;
#else
typedef __int64          FBTint64;
typedef unsigned __int64 FBTuint64;
#endif

// Arch dependent types

#if FBT_ARCH == FBT_ARCH_64
typedef FBTuint64   FBTuintPtr;
typedef FBTint64    FBTintPtr;
#else
typedef FBTuint32   FBTuintPtr;
typedef FBTint32    FBTintPtr;
#endif

typedef FBTuintPtr  FBTsize;


// Type for arrays & tables (Always unsigned & 32bit)
typedef FBTuint32       FBTsizeType;
const   FBTuint32       FBT_NPOS = (FBTuint32) - 1;
typedef FBTuint32       FBThash;
typedef FBTuint16       FBTtype;


FBT_ASSERTCOMP(FBTsizeType_NPOS_VALIDATE, FBT_NPOS == ((FBTuint32) - 1));

#define _FBT_CACHE_LIMIT 999

template <typename T> FBT_INLINE void    fbtSwap(T& a, T& b)                              { T t(a); a = b; b = t; }
template <typename T> FBT_INLINE T       fbtMax(const T& a, const T& b)                   { return a < b ? b : a; }
template <typename T> FBT_INLINE T       fbtMin(const T& a, const T& b)                   { return a < b ? a : b; }
template <typename T> FBT_INLINE T       fbtClamp(const T& v, const T& a, const T& b)     { return v < a ? a : v > b ? b : v; }


#define fbtMalloc(size)             ::malloc(size)
#define fbtCalloc(size, len)        ::calloc(size, len)
#define fbtFree(ptr)                ::free(ptr)
#define fbtMemset                   ::memset
#define fbtMemcpy                   ::memcpy
#define fbtMemmove                  ::memmove
#define fbtMemcmp                   ::memcmp



class fbtDebugger
{
public:
	typedef void (*ReportHook) (FBTuintPtr client, const char* buffer);

	struct Reporter
	{
		FBTuintPtr m_client;
		ReportHook m_hook;
	};

	static bool isDebugger(void);
	static void reportIDE(const char* src, long line, const char* msg, ...);
	static void errorIDE(const char* src, long line, const char* msg, ...);
	static void report(const char* msg, ...);
	static void breakProcess(void);

	static void setReportHook(Reporter& hook);

private:
	static Reporter m_report;

};

#define fbtPrintf fbtDebugger::report
#define fbtTRACE  fbtDebugger::reportIDE
#define fbtERROR  fbtDebugger::errorIDE

typedef enum fbtEndian
{
	FBT_ENDIAN_IS_BIG       = 0,
	FBT_ENDIAN_IS_LITTLE    = 1,
	FBT_ENDIAN_NATIVE,
} fbtEndian;

typedef union fbtEndianTest
{
	FBTbyte  bo[4];
	FBTint32 test;
} fbtEndianTest;



FBT_INLINE fbtEndian fbtGetEndian(void)
{
	fbtEndianTest e;
	e.test = FBT_ENDIAN_IS_LITTLE;
	return static_cast<fbtEndian>(e.bo[0]);
}


FBT_INLINE bool fbtIsEndian(const fbtEndian& endian)
{
	fbtEndianTest e;
	e.test = endian;
	return static_cast<fbtEndian>(e.bo[0]) == endian;
}

FBT_INLINE FBTuint16 fbtSwap16(FBTuint16 in)
{
	return static_cast<FBTuint16>(((in & 0xFF00) >> 8) | ((in & 0x00FF) << 8));
}

FBT_INLINE FBTuint32 fbtSwap32(const FBTuint32& in)
{
	return (((in & 0xFF000000) >> 24) | ((in & 0x00FF0000) >> 8) | ((in & 0x0000FF00) << 8)  | ((in & 0x000000FF) << 24));
}

FBT_INLINE FBTint16 fbtSwap16(FBTint16 in)
{
	return fbtSwap16(static_cast<FBTuint16>(in));
}

FBT_INLINE FBTint32 fbtSwap32(const FBTint32& in)
{
	return fbtSwap32(static_cast<FBTuint32>(in));
}


FBT_INLINE FBTuint64 fbtSwap64(const FBTuint64& in)
{
	FBTuint64 r = 0;
	const FBTubyte* src = reinterpret_cast<const FBTubyte*>(&in);
	FBTubyte* dst = reinterpret_cast<FBTubyte*>(&r);

	dst[0] = src[7];
	dst[1] = src[6];
	dst[2] = src[5];
	dst[3] = src[4];
	dst[4] = src[3];
	dst[5] = src[2];
	dst[6] = src[1];
	dst[7] = src[0];
	return r;
}


FBT_INLINE void fbtSwap16(FBTuint16* sp, FBTsize len)
{
	FBTsizeType i;
	for (i = 0; i < len; ++i)
	{
		*sp = fbtSwap16(*sp);
		++sp;	
	}	
}

FBT_INLINE void fbtSwap32(FBTuint32* ip, FBTsize len)
{	 
	FBTsizeType i;
	for (i = 0; i < len; ++i)
	{
		*ip = fbtSwap32(*ip);
		++ip;
	}
}


FBT_INLINE void fbtSwap64(FBTuint64* dp, FBTsize len)
{	 
	FBTsizeType i;
	for (i = 0; i < len; ++i)
	{
		*dp = fbtSwap64(*dp);
		++dp;
	}
}


FBT_INLINE FBTint64 fbtSwap64(const FBTint64& in)
{
	return fbtSwap64(static_cast<FBTuint64>(in));
}



enum fbtPointerLen
{
	FBT_VOID  = sizeof(void*),
	FBT_VOID4 = FBT_VOID == 4,
	FBT_VOID8 = FBT_VOID == 8,
};


#if FBT_ARCH == FBT_ARCH_64
FBT_ASSERTCOMP(FBT_VOID8_ASSERT, FBT_VOID8);
#else
FBT_ASSERTCOMP(FBT_VOID4_ASSERT, FBT_VOID4);
#endif




class fbtList
{
public:

	struct Link
	{
		Link*   next;
		Link*   prev;
	};


	Link*   first;
	Link*   last;

public:

	fbtList() : first(0), last(0) {}
	~fbtList() { clear(); }

	void clear(void) { first = last = 0; }

	void push_back(void* v)
	{
		Link* link = ((Link*)v);
		if (!link)
			return;

		link->prev = last;
		if (last)
			last->next = link;

		if (!first)
			first = link;

		last = link;
	}
};





template <typename T>
class fbtArrayIterator
{
public:
	typedef typename T::Pointer             Iterator;
	typedef typename T::ReferenceType       ValueType;
	typedef typename T::ConstReferenceType  ConstValueType;

protected:

	mutable Iterator        m_iterator;
	mutable FBTsizeType     m_cur;
	mutable FBTsizeType     m_capacity;

public:


	fbtArrayIterator() : m_iterator(0), m_cur(0), m_capacity(0) {}
	fbtArrayIterator(Iterator begin, FBTsizeType size) : m_iterator(begin), m_cur(0), m_capacity(size) {}
	fbtArrayIterator(T& v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) { }

	~fbtArrayIterator() {}

	FBT_INLINE bool           hasMoreElements(void) const { return m_iterator && m_cur < m_capacity; }
	FBT_INLINE ValueType      getNext(void)               { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];  }
	FBT_INLINE ConstValueType getNext(void) const         { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];  }
	FBT_INLINE void           next(void) const            { FBT_ASSERT(hasMoreElements()); ++m_cur; }
	FBT_INLINE ValueType      peekNext(void)              { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }
	FBT_INLINE ConstValueType peekNext(void) const        { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }

};

template <typename T>
class fbtArray
{
public:
	typedef T*           Pointer;
	typedef const T*     ConstPointer;

	typedef T            ValueType;
	typedef const T      ConstValueType;

	typedef T&           ReferenceType;
	typedef const T&     ConstReferenceType;

	typedef fbtArrayIterator<fbtArray<T> >       Iterator;
	typedef const fbtArrayIterator<fbtArray<T> > ConstIterator;

public:
	fbtArray() : m_size(0), m_capacity(0), m_data(0), m_cache(0)  {}

	fbtArray(const fbtArray<T>& o)
		: m_size(o.size()), m_capacity(0), m_data(0), m_cache(0)
	{
		reserve(m_size);
		copy(m_data, o.m_data, m_size);
	}

	~fbtArray() { clear(); }

	void clear(bool useCache = false)
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
			if (m_cache > _FBT_CACHE_LIMIT )
				clear(false);
			else m_size = 0;
		}
	}

	FBTsizeType find(const T& v)
	{
		for (FBTsizeType i = 0; i < m_size; i++)
		{
			if (m_data[i] == v)
				return i;
		}
		return FBT_NPOS;
	}

	FBT_INLINE void push_back(const T& v)
	{
		if (m_size == m_capacity)
			reserve(m_size == 0 ? 8 : m_size * 2);

		m_data[m_size] = v;
		m_size++;
	}

	FBT_INLINE void pop_back(void)
	{
		m_size--;
		m_data[m_size].~T();
	}


	void erase(const T& v)
	{
		erase(find(v));
	}

	void erase(FBTsizeType pos)
	{
		if (m_size > 0)
		{
			if (pos != FBT_NPOS)
			{
				swap(pos, m_size - 1);
				m_size--;
				m_data[m_size].~T();
			}
		}
	}

	void resize(FBTsizeType nr)
	{
		if (nr < m_size)
		{
			for (FBTsizeType i = m_size; i < nr; i++)
				m_data[i].~T();
		}
		else
		{
			if (nr > m_size)
				reserve(nr);
		}
		m_size = nr;
	}

	void resize(FBTsizeType nr, const T& fill)
	{
		if (nr < m_size)
		{
			for (FBTsizeType i = m_size; i < nr; i++)
				m_data[i].~T();
		}
		else
		{
			if (nr > m_size)
				reserve(nr);
			for (FBTsizeType i = m_size; i < nr; i++)
				m_data[i] = fill;

		}
		m_size = nr;
	}

	void reserve(FBTsizeType nr)
	{

		if (m_capacity < nr)
		{
			T* p = new T[nr];
			if (m_data != 0)
			{
				copy(p, m_data, m_size);
				delete []m_data;
			}
			m_data = p;
			m_capacity = nr;
		}
	}

	void sort(bool (*cmp)(const T& a, const T& b))
	{
		if (m_size > 1 && cmp)
			_sort(cmp, 0, m_size - 1);
	}

	FBT_INLINE T& operator[](FBTsizeType idx)               { FBT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	FBT_INLINE const T& operator[](FBTsizeType idx) const   { FBT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	FBT_INLINE T& at(FBTsizeType idx)                       { FBT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	FBT_INLINE const T& at(FBTsizeType idx) const           { FBT_ASSERT(idx >= 0 && idx < m_capacity); return m_data[idx]; }
	FBT_INLINE T& front(void)                               { FBT_ASSERT(m_size > 0); return m_data[0]; }
	FBT_INLINE T& back(void)                                { FBT_ASSERT(m_size > 0); return m_data[m_size-1]; }

	FBT_INLINE ConstPointer ptr(void) const             { return m_data; }
	FBT_INLINE Pointer      ptr(void)                   { return m_data; }
	FBT_INLINE bool         valid(void) const           { return m_data != 0;}

	FBT_INLINE FBTsizeType capacity(void) const         { return m_capacity; }
	FBT_INLINE FBTsizeType size(void) const             { return m_size; }
	FBT_INLINE bool empty(void) const                   { return m_size == 0;}

	FBT_INLINE Iterator       iterator(void)       { return m_data && m_size > 0 ? Iterator(m_data, m_size) : Iterator(); }
	FBT_INLINE ConstIterator  iterator(void) const { return m_data && m_size > 0 ? ConstIterator(m_data, m_size) : ConstIterator(); }

	fbtArray<T> &operator= (const fbtArray<T> &rhs)
	{
		if (this != &rhs)
		{
			clear();
			FBTsizeType os = rhs.size();
			if (os > 0)
			{
				resize(os);
				copy(m_data, rhs.m_data, os);
			}
		}

		return *this;
	}

	FBT_INLINE void copy(Pointer dst, ConstPointer src, FBTsizeType size)
	{
		FBT_ASSERT(size <= m_size);
		for (FBTsizeType i = 0; i < size; i++) dst[i] = src[i];
	}

	FBT_INLINE bool equal(const fbtArray<T> &rhs)
	{
		if (rhs.size() != size()) return false;
		if (empty()) return true;
		return fbtMemcmp(m_data, rhs.m_data, sizeof(T)*m_size) == 0;
	}

protected:

	void _sort(bool (*cmp)(const T& a, const T& b), int lo, int hi)
	{
		// btAlignedObjectArray.h

		int i = lo, j = hi;
		T x = m_data[(lo+hi)/2];

		//  partition
		do
		{
			while (cmp(m_data[i], x))
				i++;
			while (cmp(x, m_data[j]))
				j--;
			if (i <= j)
			{
				swap(i, j);
				i++; j--;
			}
		}
		while (i <= j);

		//  recursion
		if (lo < j)
			_sort( cmp, lo, j);
		if (i < hi)
			_sort( cmp, i, hi);
	}


	void swap(FBTsizeType a, FBTsizeType b)
	{
		ValueType t = m_data[a];
		m_data[a] = m_data[b];
		m_data[b] = t;
	}

	FBTsizeType     m_size;
	FBTsizeType     m_capacity;
	Pointer         m_data;
	int             m_cache;
};



template <typename T>
class fbtHashTableIterator
{
public:

	typedef typename T::Pointer        Iterator;
	typedef typename T::Entry&          Pair;
	typedef typename T::ConstEntry&     ConstPair;

	typedef typename T::ReferenceKeyType         KeyType;
	typedef typename T::ReferenceValueType       ValueType;
	typedef typename T::ConstReferenceKeyType    ConstKeyType;
	typedef typename T::ConstReferenceValueType  ConstValueType;

protected:

	mutable Iterator m_iterator;
	mutable FBTsizeType   m_cur;
	const FBTsizeType     m_capacity;


public:
	fbtHashTableIterator() : m_iterator(0), m_cur(0), m_capacity(0)  {}
	fbtHashTableIterator(Iterator begin, FBTsizeType size) : m_iterator(begin), m_cur(0), m_capacity(size) { }
	fbtHashTableIterator(T& v) : m_iterator(v.ptr()), m_cur(0), m_capacity(v.size()) {}

	~fbtHashTableIterator() {}

	FBT_INLINE bool      hasMoreElements(void) const  { return (m_iterator && m_cur < m_capacity); }
	FBT_INLINE Pair      getNext(void)                { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];}
	FBT_INLINE ConstPair getNext(void) const          { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur++];}
	FBT_INLINE void      next(void) const             { FBT_ASSERT(hasMoreElements()); ++m_cur; }


	FBT_INLINE Pair      peekNext(void)               { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }
	FBT_INLINE KeyType   peekNextKey(void)            { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur].first;}
	FBT_INLINE ValueType peekNextValue(void)          { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur].second; }

	FBT_INLINE ConstPair      peekNext(void)  const     { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur]; }
	FBT_INLINE ConstKeyType   peekNextKey(void) const   { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur].first;}
	FBT_INLINE ConstValueType peekNextValue(void) const { FBT_ASSERT(hasMoreElements()); return m_iterator[m_cur].second; }
};



// magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
#define _FBT_INITIAL_FNV  0x9E3779B1
#define _FBT_INITIAL_FNV2 0x9E3779B9
#define _FBT_MULTIPLE_FNV 0x1000193
#define _FBT_TWHASH(key)         \
        key += ~(key << 15);    \
        key ^=  (key >> 10);    \
        key +=  (key << 3);     \
        key ^=  (key >> 6);     \
        key += ~(key << 11);    \
        key ^=  (key >> 16);


class fbtCharHashKey
{
protected:
	char* m_key;
	mutable FBThash m_hash;

public:
	fbtCharHashKey() : m_key(0), m_hash(FBT_NPOS) {}
	fbtCharHashKey(char* k) : m_key(k), m_hash(FBT_NPOS) {hash();}
	fbtCharHashKey(const char* k) : m_key(const_cast<char*>(k)), m_hash(FBT_NPOS) {}
	fbtCharHashKey(const fbtCharHashKey& k) : m_key(k.m_key), m_hash(k.m_hash) { if (m_hash == FBT_NPOS) hash(); }


	FBThash hash(void) const
	{
		if (!m_key) return FBT_NPOS;
		if (m_hash != FBT_NPOS) return m_hash;

		// Fowler / Noll / Vo (FNV) Hash
		m_hash = (FBThash)_FBT_INITIAL_FNV;
		for (int i = 0; m_key[i]; i++)
		{
			m_hash = m_hash ^ (m_key[i]);   // xor  the low 8 bits
			m_hash = m_hash * _FBT_MULTIPLE_FNV;  // multiply by the magic number
		}
		return m_hash;
	}

	FBT_INLINE bool operator== (const fbtCharHashKey& v) const    {return hash() == v.hash();}
	FBT_INLINE bool operator!= (const fbtCharHashKey& v) const    {return hash() != v.hash();}
	FBT_INLINE bool operator== (const FBThash& v) const           {return hash() == v;}
	FBT_INLINE bool operator!= (const FBThash& v) const           {return hash() != v;}
};

class fbtIntHashKey
{
protected:
	FBTint32 m_key;
public:
	fbtIntHashKey() : m_key(0) {}
	fbtIntHashKey(FBTint32 k) : m_key(k) {}
	fbtIntHashKey(const fbtIntHashKey& k) : m_key(k.m_key) { }

	FBT_INLINE FBThash hash(void) const  { return static_cast<FBThash>(m_key) * _FBT_INITIAL_FNV; }

	FBT_INLINE bool operator== (const fbtIntHashKey& v) const {return hash() == v.hash();}
	FBT_INLINE bool operator!= (const fbtIntHashKey& v) const {return hash() != v.hash();}
	FBT_INLINE bool operator== (const FBThash& v) const       {return hash() == v;}
	FBT_INLINE bool operator!= (const FBThash& v) const       {return hash() != v;}
};



//For handing invalid pointers
class fbtSizeHashKey
{
protected:

	FBTsize m_key;
	mutable FBThash m_hash;

public:
	fbtSizeHashKey()
		: m_hash(FBT_NPOS), m_key(0)
	{
	}


	fbtSizeHashKey(const FBTsize& k)
		:   m_hash(FBT_NPOS), m_key(k)
	{
		hash();
	}


	fbtSizeHashKey(const fbtSizeHashKey& k)
		:   m_hash(FBT_NPOS), m_key(k.m_key)
	{
		hash();
	}


	FBT_INLINE FBThash hash(void) const
	{
		if (m_hash != FBT_NPOS)
			return m_hash;

		m_hash = (FBThash)m_key;
		_FBT_TWHASH(m_hash);
		return m_hash;
	}

	FBT_INLINE bool operator== (const fbtSizeHashKey& v) const  { return hash() == v.hash();}
	FBT_INLINE bool operator!= (const fbtSizeHashKey& v) const  { return hash() != v.hash();}
	FBT_INLINE bool operator== (const FBThash& v) const         { return hash() == v;}
	FBT_INLINE bool operator!= (const FBThash& v) const         { return hash() != v;}
};


template<typename T>
class fbtTHashKey
{
protected:
	T* m_key;
	mutable FBThash m_hash;

public:
	fbtTHashKey() : m_key(0), m_hash(FBT_NPOS) { hash(); }
	fbtTHashKey(T* k) : m_key(k), m_hash(FBT_NPOS) { hash(); }
	fbtTHashKey(const fbtTHashKey& k) : m_key(k.m_key), m_hash(k.m_hash) { if (m_hash == FBT_NPOS) hash(); }

	FBT_INLINE T*          key(void)       {return m_key;}
	FBT_INLINE const T*    key(void) const {return m_key;}


	FBT_INLINE FBThash hash(void) const
	{
		if (m_hash != FBT_NPOS)
			return m_hash;

		// Yields the least collisions.
		m_hash = static_cast<FBThash>(reinterpret_cast<FBTuintPtr>(m_key));
		_FBT_TWHASH(m_hash);
		return m_hash;
	}


	FBT_INLINE bool operator== (const fbtTHashKey& v) const  { return hash() == v.hash();}
	FBT_INLINE bool operator!= (const fbtTHashKey& v) const  { return hash() != v.hash();}
	FBT_INLINE bool operator== (const FBThash& v) const      { return hash() == v;}
	FBT_INLINE bool operator!= (const FBThash& v) const      { return hash() != v;}
};

typedef fbtTHashKey<void> fbtPointerHashKey;


template<typename Key, typename Value>
struct fbtHashEntry
{
	Key    first;
	Value  second;

	fbtHashEntry() {}
	fbtHashEntry(const Key& k, const Value& v) : first(k), second(v) {}

	FBT_INLINE bool operator==(const fbtHashEntry& rhs) const
	{
		return first == rhs.first && second == rhs.second;
	}
};

#define _FBT_UTHASHTABLE_HASH(key)      ((key.hash() & (m_capacity - 1)))
#define _FBT_UTHASHTABLE_HKHASH(key)    ((hk & (m_capacity - 1)))
#define _FBT_UTHASHTABLE_FORCE_POW2     1
#define _FBT_UTHASHTABLE_INIT           32
#define _FBT_UTHASHTABLE_EXPANSE  (m_size * 2)


#define _FBT_UTHASHTABLE_STAT       FBT_HASHTABLE_STAT
#define _FBT_UTHASHTABLE_STAT_ALLOC 0


#if _FBT_UTHASHTABLE_FORCE_POW2 == 1
#define _FBT_UTHASHTABLE_POW2(x) \
    --x; x |= x >> 16; x |= x >> 8; x |= x >> 4; \
    x |= x >> 2; x |= x >> 1; ++x;

#define _FBT_UTHASHTABLE_IS_POW2(x) (x && !((x-1) & x))
#endif


#if _FBT_UTHASHTABLE_STAT == 1
#include <typeinfo>
#endif



template < typename Key, typename Value >
class fbtHashTable
{
public:
	typedef fbtHashEntry<Key, Value>        Entry;
	typedef const fbtHashEntry<Key, Value>  ConstEntry;

	typedef Entry*  EntryArray;
	typedef FBTsizeType* IndexArray;


	typedef Key            KeyType;
	typedef Value          ValueType;

	typedef const Key      ConstKeyType;
	typedef const Value    ConstValueType;

	typedef Value&          ReferenceValueType;
	typedef const Value&    ConstReferenceValueType;

	typedef Key&            ReferenceKeyType;
	typedef const Key&      ConstReferenceKeyType;

	typedef EntryArray Pointer;
	typedef const Entry* ConstPointer;


	typedef fbtHashTableIterator<fbtHashTable<Key, Value> > Iterator;
	typedef const fbtHashTableIterator<fbtHashTable<Key, Value> > ConstIterator;


public:

	fbtHashTable()
		:    m_size(0), m_capacity(0), m_lastPos(FBT_NPOS),
		     m_iptr(0), m_nptr(0), m_bptr(0), m_cache(0)
	{
	}

	fbtHashTable(FBTsizeType capacity)
		:    m_size(0), m_capacity(0), m_lastPos(FBT_NPOS),
		     m_iptr(0), m_nptr(0), m_bptr(0), m_cache(0)
	{
	}

	fbtHashTable(const fbtHashTable& rhs)
		:    m_size(0), m_capacity(0), m_lastPos(FBT_NPOS),
		     m_iptr(0), m_nptr(0), m_bptr(0), m_cache(0)
	{
		doCopy(rhs);
	}

	~fbtHashTable() { clear(); }

	fbtHashTable<Key, Value> &operator = (const fbtHashTable<Key, Value> &rhs)
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
			m_lastKey = FBT_NPOS;
			m_lastPos = FBT_NPOS;
			m_cache = 0;

			delete [] m_bptr;
			delete [] m_iptr;
			delete [] m_nptr;
			m_bptr = 0; m_iptr = 0; m_nptr = 0;
		}
		else
		{
			++m_cache;
			if (m_cache > _FBT_CACHE_LIMIT)
				clear(false);
			else
			{
				m_size = 0;
				m_lastKey = FBT_NPOS;
				m_lastPos = FBT_NPOS;


				FBTsizeType i;
				for (i = 0; i < m_capacity; ++i)
				{
					m_iptr[i] = FBT_NPOS;
					m_nptr[i] = FBT_NPOS;
				}
			}
		}

	}
	Value&              at(FBTsizeType i)                    { FBT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	Value&              operator [](FBTsizeType i)           { FBT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	const Value&        at(FBTsizeType i)const               { FBT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	const Value&        operator [](FBTsizeType i) const     { FBT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].second; }
	Key&                keyAt(FBTsizeType i)                 { FBT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].first; }
	const Key&          keyAt(FBTsizeType i)const            { FBT_ASSERT(m_bptr && i >= 0 && i < m_size); return m_bptr[i].first; }

	Value* get(const Key& key) const
	{
		if (!m_bptr || m_size == 0)
			return (Value*)0;


		FBThash hr = key.hash();

		if (m_lastKey != hr)
		{
			FBTsizeType i = find(key);
			if (i == FBT_NPOS) return (Value*)0;


			FBT_ASSERT(i >= 0 && i < m_size);

			m_lastKey = hr;
			m_lastPos = i;
		}

		return &m_bptr[m_lastPos].second;
	}


	Value*         operator [](const Key& key)       { return get(key); }
	const Value*   operator [](const Key& key) const { return get(key); }

	FBTsizeType find(const Key& key) const
	{
		if (m_capacity == 0 || m_capacity == FBT_NPOS || m_size == 0)
			return FBT_NPOS;

		FBTsizeType hk = key.hash();
		if (m_lastPos != FBT_NPOS && m_lastKey == hk)
			return m_lastPos;

		FBThash hr = _FBT_UTHASHTABLE_HKHASH(hk);

		FBT_ASSERT(m_bptr && m_iptr && m_nptr);

		FBTsizeType fh = m_iptr[hr];
		while (fh != FBT_NPOS && (key != m_bptr[fh].first))
			fh = m_nptr[fh];


		if (fh != FBT_NPOS)
		{
			m_lastKey = hk;
			m_lastPos = fh;

			FBT_ASSERT(fh >= 0  && fh < m_size);
		}
		return fh;
	}



	void erase(const Key& key) {remove(key);}

	void remove(const Key& key)
	{
		FBThash hash, lhash;
		FBTsizeType index, pindex, findex;

		findex = find(key);
		if (findex == FBT_NPOS || m_capacity == 0 || m_size == 0)
			return;

		m_lastKey = FBT_NPOS;
		m_lastPos = FBT_NPOS;
		FBT_ASSERT(m_bptr && m_iptr && m_nptr);

		hash = _FBT_UTHASHTABLE_HASH(key);

		index  = m_iptr[hash];
		pindex = FBT_NPOS;
		while (index != findex)
		{
			pindex = index;
			index = m_nptr[index];
		}

		if (pindex != FBT_NPOS)
		{
			FBT_ASSERT(m_nptr[pindex] == findex);
			m_nptr[pindex] = m_nptr[findex];
		}
		else
			m_iptr[hash] = m_nptr[findex];

		FBTsizeType lindex = m_size - 1;
		if (lindex == findex)
		{
			--m_size;
			m_bptr[m_size].~Entry();
			return;
		}

		lhash = _FBT_UTHASHTABLE_HASH(m_bptr[lindex].first);
		index  = m_iptr[lhash];
		pindex = FBT_NPOS;
		while (index != lindex)
		{
			pindex = index;
			index = m_nptr[index];
		}

		if (pindex != FBT_NPOS)
		{
			FBT_ASSERT(m_nptr[pindex] == lindex);
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

	bool insert(const Key& key, const Value& val)
	{
		if (find(key) != FBT_NPOS)
			return false;

		if (m_size == m_capacity)
			reserve(m_size == 0 ? _FBT_UTHASHTABLE_INIT : _FBT_UTHASHTABLE_EXPANSE);

		const FBThash hr = _FBT_UTHASHTABLE_HASH(key);

		FBT_ASSERT(m_bptr && m_iptr && m_nptr);
		m_bptr[m_size] = Entry(key, val);
		m_nptr[m_size] = m_iptr[hr];
		m_iptr[hr] = m_size;
		++m_size;
		return true;
	}

	FBT_INLINE Pointer ptr(void)                { return m_bptr; }
	FBT_INLINE ConstPointer ptr(void) const     { return m_bptr; }
	FBT_INLINE bool valid(void) const           { return m_bptr != 0;}


	FBT_INLINE FBTsizeType size(void) const         { return m_size; }
	FBT_INLINE FBTsizeType capacity(void) const     { return capacity; }
	FBT_INLINE bool empty(void) const               { return m_size == 0; }


	Iterator        iterator(void)       { return m_bptr && m_size > 0 ? Iterator(m_bptr, m_size) : Iterator(); }
	ConstIterator   iterator(void) const { return m_bptr && m_size > 0 ? ConstIterator(m_bptr, m_size) : ConstIterator(); }


	void reserve(FBTsizeType nr)
	{
		if (m_capacity < nr && nr != FBT_NPOS)
			rehash(nr);
	}

#if _FBT_UTHASHTABLE_STAT == 1

	void report(void) const
	{
		if (m_capacity == 0 || m_capacity == FBT_NPOS || m_size == 0)
			return;

		FBT_ASSERT(m_bptr && m_iptr && m_nptr);


		FBTsizeType min_col = m_size, max_col = 0;
		FBTsizeType i, tot = 0, avg = 0;
		for (i = 0; i < m_size; ++i)
		{
			Key& key = m_bptr[i].first;

			FBThash hr = _FBT_UTHASHTABLE_HASH(key);

			FBTsizeType nr = 0;

			FBTsizeType fh = m_iptr[hr];
			while (fh != FBT_NPOS && (key != m_bptr[fh].first))
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

#if _FBT_UTHASHTABLE_FORCE_POW2 == 1
		fbtPrintf("Results using forced power of 2 expansion.\n\n");
#else
		fbtPrintf("Results using unaltered expansion.\n\n");
#endif
		fbtPrintf("\tTotal number of collisions %i for a table of size %i.\n\t\tusing (%s)\n", tot, m_size, typeid(Key).name());
		fbtPrintf("\tThe minimum number of collisions per key: %i\n", min_col);
		fbtPrintf("\tThe maximum number of collisions per key: %i\n", max_col);

		int favr = (int)(100.f * ((float)avg / (float)m_size));
		fbtPrintf("\tThe average number of key collisions: %i\n\n", favr);

		if (tot == 0)
			fbtPrintf("\nCongratulations lookup is 100%% linear!\n\n");
		else if (favr >  35)
			fbtPrintf("\nImprove your hash function!\n\n");
	}
#endif



private:

	void doCopy(const fbtHashTable<Key, Value> &rhs)
	{
		if (rhs.valid() && !rhs.empty())
		{
			reserve(rhs.m_capacity);

			FBTsizeType i, b;
			m_size     = rhs.m_size;
			m_capacity = rhs.m_capacity;

			b = m_size > 0 ? m_size - 1 : 0;
			for (i = b; i < m_capacity; ++i) m_nptr[i] = m_iptr[i] = FBT_NPOS;

			for (i = 0; i < m_size; ++i)
			{
				m_bptr[i] = rhs.m_bptr[i];
				m_iptr[i] = rhs.m_iptr[i];
				m_nptr[i] = rhs.m_nptr[i];
			}
		}

	}

	template<typename ArrayType>
	void reserveType(ArrayType** old, FBTsizeType nr, bool cpy = false)
	{
		FBTsizeType i;
		ArrayType* nar = new ArrayType[nr];
		if ((*old) != 0)
		{
			if (cpy)
			{
				const ArrayType* oar = (*old);
				for (i = 0; i < m_size; i++) nar[i] = oar[i];
			}
			delete [](*old);
		}
		(*old) = nar;
	}


	void rehash(FBTsizeType nr)
	{
#if _FBT_UTHASHTABLE_FORCE_POW2

		if (!_FBT_UTHASHTABLE_IS_POW2(nr))
		{
			_FBT_UTHASHTABLE_POW2(nr);
		}

#if _FBT_UTHASHTABLE_STAT_ALLOC == 1
		fbtPrintf("Expanding tables: %i\n", nr);
#endif
		FBT_ASSERT(_FBT_UTHASHTABLE_IS_POW2(nr));


#else

#if _FBT_UTHASHTABLE_STAT_ALLOC == 1
		fbtPrintf("Expanding tables: %i\n", nr);
#endif

#endif

		reserveType<Entry>(&m_bptr, nr, true);
		reserveType<FBTsizeType>(&m_iptr, nr);
		reserveType<FBTsizeType>(&m_nptr, nr);

		m_capacity = nr;
		FBT_ASSERT(m_bptr && m_iptr && m_nptr);


		FBTsizeType i, h;
		for (i = 0; i < m_capacity; ++i) { m_iptr[i] = m_nptr[i] = FBT_NPOS; }
		for (i = 0; i < m_size; i++)     { h = _FBT_UTHASHTABLE_HASH(m_bptr[i].first); m_nptr[i] = m_iptr[h]; m_iptr[h] = i;}
	}



	FBTsizeType m_size, m_capacity;
	mutable FBTsizeType m_lastPos;
	mutable FBTsizeType m_lastKey;

	IndexArray m_iptr;
	IndexArray m_nptr;
	EntryArray m_bptr;
	FBTsizeType m_cache;
};




#define fbtCharNEq(a, b, n)  ((a && b) && !strncmp(a, b, n))
#define fbtCharEq(a, b)      ((a && b) && (*a == *b) && !strcmp(a, b))
#define fbtCharEqL(a, b, l)  ((a && b) && (*a == *b) && !memcmp(a, b, l))

FBT_INLINE int fbtStrLen(const char* cp)
{
	int i = 0;
	while (cp[i]) cp[i++];
	return i;
}


// For operations on a fixed size character array
template <const FBTuint16 L>
class fbtFixedString
{
public:
	typedef char Pointer[L+1];


public:

	fbtFixedString()
		: m_size(0), m_hash(FBT_NPOS)
	{
		m_buffer[m_size] = 0;
	}

	fbtFixedString(const fbtFixedString& rhs)
		:   m_size(0), m_hash(FBT_NPOS)
	{
		if (rhs.size())
		{
			FBTuint16 i, os = rhs.size();
			const char* cp = rhs.c_str();

			for (i = 0; i < L && i < os; ++i, ++m_size) m_buffer[i] = cp[i];
		}
		m_buffer[m_size] = 0;
	}


	fbtFixedString(const char* rhs)
		:   m_size(0), m_hash(FBT_NPOS)
	{
		if (rhs)
		{
			FBTuint16 i;
			for (i = 0; i < L && rhs[i]; ++i, ++m_size) m_buffer[i] = rhs[i];
		}
		m_buffer[m_size] = 0;
	}



	FBT_INLINE void push_back(char ch)
	{
		if (m_size >= L)
			return;
		m_buffer[m_size++] = ch;
		m_buffer[m_size] = 0;
	}

	void append(const char* str)
	{
		int len = fbtStrLen(str);
		int a = 0;
		while (a < len)
			push_back(str[a++]);
	}


	void append(const fbtFixedString& str)
	{
		int len = str.m_size;
		int a = 0;
		while (a < len)
			push_back(str.m_buffer[a++]);
	}


	fbtFixedString operator +(const fbtFixedString& rhs)
	{
		fbtFixedString lhs = *this;
		lhs.append(rhs);
		return lhs;
	}


	fbtFixedString operator +=(const fbtFixedString& rhs)
	{
		append(rhs);
		return *this;
	}

	fbtFixedString operator +(const char* str)
	{
		fbtFixedString lhs = *this;
		lhs.append(str);
		return lhs;
	}

	fbtFixedString operator +=(const char* str)
	{
		append(str);
		return *this;
	}


	void split(fbtArray<fbtFixedString<L> >& dest, char c, char e = '\0') const
	{
		FBTuint16 i, p = 0, t;
		for (i = 0; i < L && i < m_size; ++i)
		{
			if (m_buffer[i] == c || m_buffer[i] == e)
			{
				fbtFixedString<L> cpy;
				for (t = p; t < i; ++t) cpy.push_back(m_buffer[t]);
				dest.push_back(cpy);
				p = i + 1;
			}
		}

		if (p != i)
		{
			fbtFixedString<L> cpy;
			for (t = p; t < i; ++t) cpy.push_back(m_buffer[t]);
			dest.push_back(cpy);
		}
	}


	void resize(FBTuint16 ns)
	{
		if (ns <= L)
		{
			if (ns < m_size)
				for (FBTuint16 i = ns; i < m_size; i++) m_buffer[i] = 0;
			else
				for (FBTuint16 i = m_size; i < ns; i++) m_buffer[i] = 0;
			m_size = ns;
			m_buffer[m_size] = 0;
		}
	}



	template<const FBTuint16 OL>
	fbtFixedString<L>& operator = (const fbtFixedString<OL>& o)
	{
		if (o.m_size > 0)
		{
			if (m_hash == FBT_NPOS || m_hash != o.m_hash)
			{
				FBTuint16 i;
				m_size = 0;
				m_hash = o.m_hash;
				for (i = 0; (i < L && i < OL) && i < o.m_size; ++i, ++m_size) m_buffer[i] = o.m_buffer[i];
				m_buffer[m_size] = 0;
			}
		}
		return *this;
	}

	FBT_INLINE const char* c_str(void) const                 { return m_buffer; }
	FBT_INLINE char* ptr(void)                               { return m_buffer; }
	FBT_INLINE const char* ptr(void) const                   { return m_buffer; }
	FBT_INLINE const char operator [](FBTuint16 i) const     { FBT_ASSERT(i < L); return m_buffer[i]; }
	FBT_INLINE const char at(FBTuint16 i) const              { FBT_ASSERT(i < L); return m_buffer[i]; }
	FBT_INLINE void clear(void)                              { m_buffer[0] = 0; m_size = 0; }
	FBT_INLINE int empty(void) const                         { return m_size == 0; }
	FBT_INLINE int size(void) const                          { return m_size; }
	FBT_INLINE int capacity(void) const                      { return L; }



	FBTsize hash(void) const
	{
		if (m_hash != FBT_NPOS)
			return m_hash;

		if (m_size == 0 || !m_buffer)
			return FBT_NPOS;
		fbtCharHashKey chk(m_buffer);
		m_hash = chk.hash();
		return m_hash;
	}

	FBT_INLINE bool operator == (const fbtFixedString& str) const { return hash() == str.hash(); }
	FBT_INLINE bool operator != (const fbtFixedString& str) const { return !(this->operator ==(str));}

protected:

	Pointer             m_buffer;
	FBTuint16           m_size;
	mutable FBThash     m_hash;
};


enum FBT_PRIM_TYPE
{
	FBT_PRIM_CHAR,		// 0
	FBT_PRIM_UCHAR,		// 1
	FBT_PRIM_SHORT,		// 2
	FBT_PRIM_USHORT,	// 3
	FBT_PRIM_INT,		// 4
	FBT_PRIM_LONG,		// 5
	FBT_PRIM_ULONG,		// 6
	FBT_PRIM_FLOAT,		// 7
	FBT_PRIM_DOUBLE,	// 8
	FBT_PRIM_VOID,		// 9
	FBT_PRIM_UNKNOWN	// 10
};

FBT_PRIM_TYPE fbtGetPrimType(FBTuint32 typeKey);
FBT_INLINE FBT_PRIM_TYPE fbtGetPrimType(const char* typeName)
{
	return fbtGetPrimType(fbtCharHashKey(typeName).hash());
}
FBT_INLINE bool fbtIsIntType(FBTuint32 typeKey)
{
	FBT_PRIM_TYPE tp = fbtGetPrimType(typeKey);
	return tp < FBT_PRIM_FLOAT;
}
FBT_INLINE bool fbtIsFloatType(FBTuint32 typeKey)
{
	FBT_PRIM_TYPE tp = fbtGetPrimType(typeKey);
	return tp == FBT_PRIM_FLOAT || tp == FBT_PRIM_DOUBLE;
}
FBT_INLINE bool fbtIsNumberType(FBTuint32 typeKey)
{
	FBT_PRIM_TYPE tp = fbtGetPrimType(typeKey);
	return tp != FBT_PRIM_VOID && tp != FBT_PRIM_UNKNOWN;
}



/** @}*/
#endif//_fbtTypes_h_
