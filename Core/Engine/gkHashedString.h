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
#ifndef _gkHashedString_h_
#define _gkHashedString_h_

#include "gkHash.h"
#include "OgreString.h"



// ----------------------------------------------------------------------------
class gkHashedString : public Ogre::String
{
public:
	gkHashedString() :
			Ogre::String(),
			m_hash(GK_NPOS),
			m_hashed(false)
	{
	}

	gkHashedString(char c, size_t nr= 1) :
			Ogre::String(c, nr),
			m_hash(GK_NPOS),
			m_hashed(false)
	{
		hash_string();
	}

	gkHashedString(const char *buf) :
			Ogre::String(buf),
			m_hash(GK_NPOS),
			m_hashed(false)
	{
		hash_string();
	}

	gkHashedString(const Ogre::String &oth) :
			Ogre::String(oth),
			m_hash(GK_NPOS),
			m_hashed(false)
	{
		hash_string();
	}

	gkHashedString(const Ogre::String& v, size_t nr) :
			Ogre::String(v, nr),
			m_hash(GK_NPOS),
			m_hashed(false)
	{
		hash_string();
	}

	gkHashedString(const char *buf, size_t nr) :
			Ogre::String(buf, nr),
			m_hash(GK_NPOS),
			m_hashed(false)
	{
		hash_string();
	}

	gkHashedString(GKhash h) :
			Ogre::String(),
			m_hash(h),
			m_hashed(true)
	{
		/// cannot reshash
	}

	GKhash rehash(void) const;
	GKhash gkHash(void) const;
	operator GKhash(void) const;

	const gkHashedString& operator= (const gkHashedString& o);

	bool operator== (const GKhash& v) const;
	bool operator!= (const GKhash& v) const;

	bool operator== (const gkHashedString& v) const;
	bool operator!= (const gkHashedString& v) const;

	bool hashed(void) const;

protected:

	GKhash hash_string(void) const;

	mutable GKhash  m_hash;
	mutable bool	m_hashed;
};


// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHash(const gkHashedString& h)
{
	return h.gkHash();
}

// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHashedString::hash_string(void) const
{
	if (m_hashed)
		return GK_NPOS;

	/// http://www.cse.yorku.ca/~oz/gkHash.html
	/// djb2

	char *cp= (char*)Ogre::String::c_str();

	m_hash= 5381;
	int c;
	while (c= *cp++)
		m_hash= ((m_hash << 5) + m_hash) + c;

	m_hashed= true;
	return m_hash;
}

// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHashedString::gkHash(void) const
{
	if (!m_hashed)
		hash_string();
	return m_hash;
}

// ----------------------------------------------------------------------------
GK_INLINE GKhash gkHashedString::rehash(void) const
{
	m_hashed= false;
	hash_string();
	return m_hash;
}

// ----------------------------------------------------------------------------
GK_INLINE gkHashedString::operator GKhash(void) const
{
	if (!m_hashed)
		hash_string();
	return m_hash;
}

// ----------------------------------------------------------------------------
GK_INLINE const gkHashedString& gkHashedString::operator= (const gkHashedString & o)
{
	Ogre::String::operator= ((const Ogre::String&)o);
	m_hashed= o.m_hashed;
	m_hash= o.m_hash;

	if (!m_hashed)
		hash_string();
	return *this;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkHashedString::operator== (const GKhash& v) const
{
	return m_hash == v;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkHashedString::operator!= (const GKhash& v) const
{
	return m_hash != v;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkHashedString::operator== (const gkHashedString& v) const
{
	return m_hash == v.m_hash;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkHashedString::operator!= (const gkHashedString& v) const
{
	return m_hash != v.m_hash;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkHashedString::hashed(void) const
{
	return m_hashed;
}






#endif//_gkHashedString_h_
