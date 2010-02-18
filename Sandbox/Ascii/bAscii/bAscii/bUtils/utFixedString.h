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
#ifndef _utFixedString_h_
#define _utFixedString_h_

#include "utCommon.h"
#include "utStringDefines.h"
#include "utHash.h"

// -----------------------------------------------------------------------------
template <const	unsigned short L>
class utFixedString
{
public:
	typedef	char Pointer[(L+1)];

public:
	utFixedString()	: m_size(0)	{ m_buffer[m_size] = 0;}
	utFixedString(const	utFixedString<L>& o) : m_size(0)
	{
		if (o.size())
		{
			unsigned short i;
			const char *cp = o.c_str();
			for	(i=0; i<L && i<o.size(); ++i, ++m_size)
				m_buffer[i]	= cp[i];
			m_buffer[m_size] = 0;
		}
		m_buffer[m_size] = 0;
	}

	utFixedString(const	char *o) : m_size(0)
	{
		if (o)
		{
			unsigned short i;
			for	(i=0; i<L && o[i]; ++i,	++m_size)
				m_buffer[i]	= o[i];
			m_buffer[m_size] = 0;
		}
		m_buffer[m_size] = 0;
	}

	UT_INLINE void push_back(char ch)
	{
		if (m_size >= L) return;
		m_buffer[m_size++] = ch;
		m_buffer[m_size] = 0;
	}

	const utFixedString& format(const char *fmt, ...)
	{
		va_list	lst;
		va_start(lst, fmt);
		int	size = ut_vsnprintf(m_buffer, L, fmt, lst);
		va_end(lst);
		if (size < 0)
			size = L;
		m_buffer[size] = 0;
		m_size = size;
		return *this;
	}

	void resize(unsigned short ns)				
	{
		if	(ns	< L) 
		{
			if (ns < m_size)
			{
				for	(unsigned short	i=ns; i<m_size;	i++)
					m_buffer[i]	= 0;
			}
			else
			{
				for	(unsigned short	i=m_size; i<ns;	i++)
					m_buffer[i]	= 0;
			}
			m_size = ns; 
			m_buffer[ns] = 0;
		}
	}

	UT_INLINE const	char* c_str(void) const			{ return m_buffer; }
	UT_INLINE char*	ptr(void)						{ return m_buffer; }
	UT_INLINE int empty(void) const					{ return m_size	== 0; }
	UT_INLINE int size(void) const					{ return m_size; }
	UT_INLINE int capacity(void) const				{ return L;	}
	UT_INLINE UThash hash(void)	const				{ return ::utHash(m_buffer); }
	UT_INLINE void clear(void)						{ m_buffer[0] =	0; m_size =	0; }

	UT_INLINE const	char operator [](unsigned short	i) const	{ UT_ASSERT(i <	m_size && i	< L); return m_buffer[i]; }
	UT_INLINE const	char at(unsigned short i) const				{ UT_ASSERT(i <	m_size && i	< L); return m_buffer[i]; }
	UT_INLINE bool operator	== (const utFixedString& str) const	{ return utCharEqL(m_buffer, str.m_buffer, UT_Min(L, str.m_size	+ 1)); }
	UT_INLINE bool operator	!= (const utFixedString& str) const	{ return utCharEqL(m_buffer, str.m_buffer, UT_Min(L, str.m_size	+ 1)); }

protected:
	Pointer			m_buffer;
	unsigned short	m_size;
};


#endif//_utFixedString_h_
