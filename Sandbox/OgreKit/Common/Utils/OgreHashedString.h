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
#ifndef _OgreHashedString_h_
#define _OgreHashedString_h_

#include "OgreHash.h"
#include "OgreString.h"
#include "OgreIntTypes.h"

namespace Ogre
{


class HashedString
{
public:
    HashedString() : m_string(""), m_hash(OGRE_NPOS), m_hashed(false) {}

	HashedString(const char *buf) :
            m_string(buf), m_hash(OGRE_NPOS), m_hashed(false)
    {
		hash_string();
    }

	HashedString(const HashedString &oth) :
		m_string(oth.m_string), m_hash(OGRE_NPOS), m_hashed(false)
    {
		hash_string();
    }

	HashedString(const String &oth) :
            m_string(oth), m_hash(OGRE_NPOS), m_hashed(false)
    {
		hash_string();
    }

	virtual ~HashedString() {}

	OGRE_INLINE const String& str(void)const					{ return m_string; }
    OGRE_INLINE hash_t hash(void) const                         { if (!m_hashed) hash_string(); return m_hash; }
    OGRE_INLINE hash_t rehash(void) const                       { m_hashed = false; hash_string(); return m_hash; }
    OGRE_INLINE operator hash_t(void) const                     { if (!m_hashed) hash_string(); return m_hash; }
    OGRE_INLINE bool operator== (const hash_t& v) const         { return m_hash == v; }
    OGRE_INLINE bool operator!= (const hash_t& v) const         { return m_hash != v; }
    OGRE_INLINE bool operator== (const HashedString& v) const   { return m_hash == v.m_hash; }
    OGRE_INLINE bool operator!= (const HashedString& v) const   { return m_hash != v.m_hash; }
    OGRE_INLINE bool hashed(void) const                         { return m_hashed; }

	HashedString &operator=(const HashedString& o)
	{
		if (*this != o)
		{
			m_string = o.m_string;
			m_hash = OGRE_NPOS;
			m_hashed = false;
			hash_string();
		}

		return *this;
	}

protected:
    mutable hash_t  m_hash;
    mutable bool    m_hashed;
	String			m_string;

    hash_t hash_string(void) const
    {
        if (m_hashed)
            return OGRE_NPOS;

		m_hash = OgreHash(m_string.c_str());
        m_hashed = true;
        return m_hash;
    }

};


OGRE_INLINE hash_t OgreHash(const HashedString& h) { return h.hash(); }

}

#endif//_OgreHashedString_h_
