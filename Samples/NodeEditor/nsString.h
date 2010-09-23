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
#ifndef _nsString_h_
#define _nsString_h_

#include <string.h>
#include <string>
#include "utTypes.h"
#include "nsMath.h"
#include <sstream>
#include <stdarg.h>

#define nsStrlen            strlen
#define nsStrcpy            strncpy
#define nsCharNEq(a, b, n)  ((a && b) && !strncmp(a, b, n))
#define nsCharEq(a, b)      ((a && b) && (*a == *b) && !strcmp(a, b))
#define nsCharEqL(a, b, l)  ((a && b) && (*a == *b) && !strncmp(a, b, l))

#if NS_COMPILER == NS_COMPILER_MSVC
# define ns_snprintf    _snprintf
# define ns_vsnprintf    _vsnprintf
#else
# define ns_snprintf    snprintf
# define ns_vsnprintf    vsnprintf
#endif


// Some day I will write a reusable string class
typedef std::string         nsString;
typedef utArray<nsString>   nsStringArray;


// nsString nsility functions
class nsStringUtils
{
public:
	static void trim( nsString& in, const nsString& expr = " \t\r" );
	static void split(nsStringArray& arr, const nsString& spl, const nsString& expr = " \t\r" );
	static void lower( nsString& str );
	static nsString lower( const nsString& str );
	static void upper( nsString& str );
	static nsString upper( const nsString& str );
	static void replace( nsString& in, const nsString& from, const nsString& to );
	static void loadFileAsString( nsString& dest, const nsString& file);

};


class nsStringConverter
{
public:
	static char nsStringSplitter;


	static bool     toBool(const nsString& v)
	{
		return  v == "true" || (toInt(v) != 0);
	}

	static NSfloat  toFloat(const nsString& v)
	{
		nsString i = v;
		nsStringUtils::trim(i);
		return fromString<NSfloat>(i);
	}
	static int      toInt(const nsString& v)
	{
		nsString i = v;
		nsStringUtils::trim(i);
		return fromString<int>(i);
	}

	static NSvec2 toVec2(const nsString& v, char split = nsStringSplitter)
	{
		nsStringArray arr;
		toArray(v, arr, split);
		if (arr.size() == 2)
			return NSvec2(toFloat(arr[0]), toFloat(arr[1]));
		return NSvec2(0, 0);
	}

	static NSvec3 toVec3(const nsString& v, char split = nsStringSplitter)
	{
		nsStringArray arr;
		toArray(v, arr, split);
		if (arr.size() == 3)
			return NSvec3(toFloat(arr[0]), toFloat(arr[1]), toFloat(arr[2]));
		return NSvec3(0, 0, 0);
	}

	static NSvec4 toVec4(const nsString& v, char split = nsStringSplitter)
	{
		nsStringArray arr;
		toArray(v, arr, split);
		if (arr.size() == 4)
			return NSvec4(toFloat(arr[0]), toFloat(arr[1]), toFloat(arr[2]), toFloat(arr[3]));
		return NSvec4(0, 0, 0, 1);
	}

	static NSquat toQuat(const nsString& v, char split = nsStringSplitter)
	{
		nsStringArray arr;
		toArray(v, arr, split);
		if (arr.size() == 4)
			return NSquat(toFloat(arr[0]), toFloat(arr[1]), toFloat(arr[2]), toFloat(arr[3]));
		return NSquat(1, 0, 0, 0);
	}

	static NSrect toRect(const nsString& v, char split = nsStringSplitter)
	{
		nsStringArray arr;
		toArray(v, arr, split);
		if (arr.size() == 4)
			return NSrect(toFloat(arr[0]), toFloat(arr[1]), toFloat(arr[2]), toFloat(arr[3]));
		return NSrect(0, 0, 0, 0);
	}


	static unsigned int fromHexChar(const char* ch)
	{
		unsigned int rv = 0xFF;
		std::istringstream ss(ch);
		ss >> std::hex >> rv;
		return rv;
	}

	static NSfloat fromHexCharF(const char* ch)
	{
		NSfloat f = (NSfloat)fromHexChar(ch);
		return NScolorf(f);
	}

	static NScolor toColorHex(const nsString& v)
	{
		const char* cptr = v.c_str();
		bool rgba   = v.size()  == 8;
		bool rgb    = !rgba && v.size() == 6;
		if (!rgba && !rgb) return NScolor(1, 1, 1, 1);

		NScolor ret;
		char r[3] = {*cptr++, *cptr++, 0};
		char g[3] = {*cptr++, *cptr++, 0};
		char b[3] = {*cptr++, *cptr++, 0};

		ret.r = fromHexCharF(r);
		ret.g = fromHexCharF(g);
		ret.b = fromHexCharF(b);
		ret.a = 1.f;

		if (rgba)
		{
			char a[3] = {*cptr++, *cptr++, 0};
			ret.a = fromHexCharF(a);
		}
		return ret;
	}

	template<typename T>
	static nsString toString(const T& v)
	{
		std::ostringstream stream;
		stream << v;
		return stream.str();
	}

	template<typename T, int size>
	static nsString toStringArray(const T* v, char split = nsStringSplitter)
	{
		std::ostringstream stream;
		char pad = split;
		for (int i = 0; i < size; ++i)
		{
			stream << v[i];
			if (i + 1 < size)
				stream << pad;
		}
		return stream.str();
	}

	template<typename T>
	static T fromString(const nsString& v)
	{
		T val = T();
		std::istringstream ss(v);
		ss >> val;
		return val;
	}


	template<typename T, int size>
	static void fromStringArray(const nsString& v, T* out)
	{
		nsString local;
		formatString(v, local);

		std::istringstream ss(local);
		char pad;
		for (int i = 0; i < size; ++i)
		{
			ss >> out[i];
			if ((i + 1) < size)
				ss >> pad;
		}
	}

	static void formatString(const nsString& in, nsString& out)
	{
		out = in;
		nsStringUtils::replace(out, ",",        "");
		nsStringUtils::replace(out, ";",        "");
	}

	static void toArray(const nsString& in, nsStringArray& out, char split = nsStringSplitter)
	{
		nsString local;
		formatString(in, local);
		out.reserve(16);
		char spl[2] = {split, '\0'};
		nsStringUtils::split(out, local, spl);
	}
};

#include "nsDefaultStringConverters.inl"


class nsHashedString
{
protected:
	nsString m_key;
	mutable UThash m_hash;

public:
	nsHashedString() : m_key(""), m_hash(UT_NPOS) {}
	~nsHashedString() {}

	// Key Constructor
	nsHashedString(char* k) : m_key(k), m_hash(UT_NPOS) {hash();}
	nsHashedString(const char* k) : m_key(const_cast<char*>(k)), m_hash(UT_NPOS) {}
	nsHashedString(const nsString& k) : m_key(k), m_hash(UT_NPOS) {}

	// Copy constructor
	nsHashedString(const nsHashedString& k) : m_key(k.m_key), m_hash(k.m_hash) {}

	UT_INLINE const nsString& str(void) const {return m_key;}

	UThash hash(void) const
	{

		// use cached hash
		if (m_hash != UT_NPOS) return m_hash;

		const char* str = m_key.c_str();

		// magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
		static const unsigned int  InitialFNV = 2166136261u;
		static const unsigned int FNVMultiple = 16777619u;

		// Fowler / Noll / Vo (FNV) Hash
		m_hash = (UThash)InitialFNV;
		for (int i = 0; str[i]; i++)
		{
			m_hash = m_hash ^ (str[i]);   // xor  the low 8 bits
			m_hash = m_hash * FNVMultiple;  // multiply by the magic number
		}
		return m_hash;
	}

	UT_INLINE bool operator== (const nsHashedString& v) const    {return hash() == v.hash();}
	UT_INLINE bool operator!= (const nsHashedString& v) const    {return hash() != v.hash();}
	UT_INLINE bool operator== (const UThash& v) const            {return hash() == v;}
	UT_INLINE bool operator!= (const UThash& v) const            {return hash() != v;}

};

// For operations on a fixed size character array
template <const UTuint16 L>
class nsFixedString
{
public:
	UT_ASSERTCOMP((L < 0xFFFF), Limit);

	typedef char Pointer[(L+1)];


public:
	// Empty constructor
	nsFixedString() : m_size(0) { m_buffer[m_size] = 0;}

	// Copy constructor
	nsFixedString(const nsFixedString& o) : m_size(0)
	{
		if (o.size())
		{
			UTuint16 i;
			const char* cp = o.c_str();
			for (i = 0; i < L && i < o.size(); ++i, ++m_size)
				m_buffer[i] = cp[i];
			m_buffer[m_size] = 0;
		}
		m_buffer[m_size] = 0;
	}

	// Copy character pointer
	nsFixedString(const char* o) : m_size(0)
	{
		if (o)
		{
			UTuint16 i;
			for (i = 0; i < L && o[i]; ++i, ++m_size)
				m_buffer[i]    = o[i];
			m_buffer[m_size] = 0;
		}
		m_buffer[m_size] = 0;
	}
	// Appends charcters upto max (L)
	UT_INLINE void push_back(char ch)
	{
		if (m_size >= L) return;
		m_buffer[m_size++] = ch;
		m_buffer[m_size] = 0;
	}

	const nsFixedString& format(const char* fmt, ...)
	{
		va_list    lst;
		va_start(lst, fmt);
		int    size = ns_vsnprintf(m_buffer, L, fmt, lst);
		va_end(lst);
		if (size < 0)
			size = L;
		m_buffer[size] = 0;
		m_size = size;
		return *this;
	}

	// Nulify size
	void resize(UTuint16 ns)
	{
		if (ns < L)
		{
			if (ns < m_size)
			{
				for (UTuint16 i = ns; i < m_size; i++)
					m_buffer[i] = 0;
			}
			else
			{
				for (UTuint16 i = m_size; i < ns; i++)
					m_buffer[i]    = 0;
			}
			m_size = ns;
			m_buffer[m_size] = 0;
		}
	}



	nsFixedString<L>& operator = (const nsFixedString<L>& o)
	{
		if (o.m_size > 0)
		{
			if (!(nsCharEqL(m_buffer, o.m_buffer, o.m_size)))
			{
				UTuint16 i;
				m_size = 0;
				for (i = 0; i < L && i < o.m_size; ++i, ++m_size)
					m_buffer[i] = o.m_buffer[i];
				m_buffer[m_size] = 0;
			}
		}
		return *this;
	}

	// Raw data access

	UT_INLINE const char* c_str(void) const                 { return m_buffer; }
	UT_INLINE char* ptr(void)                               { return m_buffer; }
	UT_INLINE const char* ptr(void) const                   { return m_buffer; }
	UT_INLINE const char operator [](UTuint16 i) const      { UT_ASSERT(i < m_size && i < L); return m_buffer[i]; }
	UT_INLINE const char at(UTuint16 i) const               { UT_ASSERT(i < m_size && i < L); return m_buffer[i]; }
	UT_INLINE void clear(void)                              { m_buffer[0] = 0; m_size = 0; }


	// Size queries

	UT_INLINE int empty(void) const                 { return m_size == 0; }
	UT_INLINE int size(void) const                  { return m_size; }
	UT_INLINE int capacity(void) const              { return L; }


	UT_INLINE bool operator == (const nsFixedString& str) const { return nsCharEqL(m_buffer, str.m_buffer, bufMin(L, str.m_size + 1)); }
	UT_INLINE bool operator != (const nsFixedString& str) const { return !nsCharEqL(m_buffer, str.m_buffer, bufMin(L, str.m_size + 1)); }

	// hashing
	UT_INLINE UThash hash(void) const
	{
		utCharHashKey ch(m_buffer);
		return ch.hash();
	}


protected:
	UT_INLINE UTuint16 bufMin(UTuint16 a, UTuint16 b) const { return a > b ? b : a; }
	Pointer m_buffer;
	UTuint16 m_size;
};

#endif//_nsString_h_
