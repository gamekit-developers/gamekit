/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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
#ifndef _gkHashedString_h_
#define _gkHashedString_h_

#include "gkString.h"


class gkHashedString
{
protected:
	gkString m_key;
	mutable UThash m_hash;

public:
	gkHashedString() : m_key(""), m_hash(UT_NPOS) {}
	~gkHashedString() {}

	gkHashedString(char *k) : m_key(k), m_hash(UT_NPOS) {hash();}
	gkHashedString(const char *k) : m_key(const_cast<char *>(k)), m_hash(UT_NPOS) {}
	gkHashedString(const gkString &k) : m_key(k), m_hash(UT_NPOS) {}
	gkHashedString(const gkHashedString &k) : m_key(k.m_key), m_hash(k.m_hash) {}

	UT_INLINE const gkString &str(void) const {return m_key;}

	UThash hash(void) const
	{

		// use cached hash
		if (m_hash != UT_NPOS) return m_hash;

		const char *str = m_key.c_str();

		// magic numbers from http://www.isthe.com/chongo/tech/comp/fnv/
		static const unsigned int  InitialFNV = 2166136261u;
		static const unsigned int FNVMultiple = 16777619u;

		// Fowler / Noll / Vo (FNV) Hash
		m_hash = (UThash)InitialFNV;
		for (int i = 0; str[i]; i++)
		{
			m_hash = m_hash ^(str[i]);    // xor  the low 8 bits
			m_hash = m_hash * FNVMultiple;  // multiply by the magic number
		}
		return m_hash;
	}

	UT_INLINE bool operator== (const gkHashedString &v) const    {return hash() == v.hash();}
	UT_INLINE bool operator!= (const gkHashedString &v) const    {return hash() != v.hash();}
	UT_INLINE bool operator== (const UThash &v) const            {return hash() == v;}
	UT_INLINE bool operator!= (const UThash &v) const            {return hash() != v;}

};

#endif//_gkHashedString_h_
