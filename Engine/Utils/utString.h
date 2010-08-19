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
#ifndef _utString_h_
#define _utString_h_

#include <string.h>
#include <string>

#include "Utils/utCommon.h"
#include "Utils/utTypes.h"

#define utStrlen            strlen
#define utStrcpy            strncpy
#define utCharNEq(a, b, n)  ((a && b) && !strncmp(a, b, n))
#define utCharEq(a, b)      ((a && b) && (*a == *b) && !strcmp(a, b))
#define utCharEqL(a, b, l)  ((a && b) && (*a == *b) && !strncmp(a, b, l))


// Some day I will write a reusable string class
typedef std::string utString;
typedef utArray<utString> utStringArray;

// utString utility functions
class utStringUtils
{
public:
	static void trim( utString &in, const utString &expr= " \t\r" );
	static void split(utStringArray &arr, const utString &spl, const utString &expr= " \t\r" );
	static void lower( utString &str );
	static utString lower( const utString &str );
	static void upper( utString &str );
	static utString upper( const utString &str );
	static void replace( utString &in, const utString &from, const utString &to );
};


// For operations on a fixed size character array
template <const UTuint16 L>
class utFixedString
{
public:
	UT_ASSERTCOMP((L < 0xFFFF), Limit);

	typedef char Pointer[(L+1)];


public:
	// Empty constructor
	utFixedString() : m_size(0) { m_buffer[m_size] = 0;}

	// Copy constructor
	utFixedString(const utFixedString &o) : m_size(0)
	{
		if (o.size())
		{
			UTuint16 i;
			const char *cp = o.c_str();
			for (i = 0; i < L && i < o.size(); ++i, ++m_size)
				m_buffer[i] = cp[i];
			m_buffer[m_size] = 0;
		}
		m_buffer[m_size] = 0;
	}

	// Copy character pointer
	utFixedString(const char *o) : m_size(0)
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



	utFixedString<L>& operator = (const utFixedString<L>& o)
	{
		if (o.m_size > 0)
		{
			if (!(utCharEqL(m_buffer, o.m_buffer, o.m_size)))
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

	UT_INLINE const char *c_str(void) const                 { return m_buffer; }
	UT_INLINE char *ptr(void)                               { return m_buffer; }
	UT_INLINE const char *ptr(void) const                   { return m_buffer; }
	UT_INLINE const char operator [](UTuint16 i) const      { UT_ASSERT(i < m_size && i < L); return m_buffer[i]; }
	UT_INLINE const char at(UTuint16 i) const               { UT_ASSERT(i < m_size && i < L); return m_buffer[i]; }
	UT_INLINE void clear(void)                              { m_buffer[0] = 0; m_size = 0; }


	// Size queries

	UT_INLINE int empty(void) const                 { return m_size == 0; }
	UT_INLINE int size(void) const                  { return m_size; }
	UT_INLINE int capacity(void) const              { return L; }


	UT_INLINE bool operator == (const utFixedString &str) const { return utCharEqL(m_buffer, str.m_buffer, bufMin(L, str.m_size + 1)); }
	UT_INLINE bool operator != (const utFixedString &str) const { return !utCharEqL(m_buffer, str.m_buffer, bufMin(L, str.m_size + 1)); }

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

#endif//_utString_h_
