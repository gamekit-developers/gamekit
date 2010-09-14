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
#ifndef _nsSingleton_h_
#define _nsSingleton_h_

#include <assert.h>


template <typename T>
class nsSingleton
{
protected:
	static T* m_singleton;


public:


	nsSingleton()
	{
		assert(!m_singleton);
		m_singleton = static_cast<T*>(this);
	}

	virtual ~nsSingleton()
	{
		assert(m_singleton);
		m_singleton = 0;
	}

	inline T& getSingleton(void)    {assert(m_singleton); return *m_singleton; }
	inline T& getSingletonPtr(void) { return  m_singleton; }

};



#define NS_DECLARE_SINGLETON(cls)       \
    public:                             \
    static cls& getSingleton(void);     \
    static cls* getSingletonPtr(void);

#define NS_IMPLEMENT_SINGLETON(cls)                     \
    template<> cls* nsSingleton<cls>::m_singleton= 0;   \
    cls& cls::getSingleton(void) {                      \
        assert(m_singleton);                            \
        return *m_singleton;                            \
    }                                                   \
    cls* cls::getSingletonPtr(void){                    \
        return m_singleton;                             \
    }

#endif//_nsSingleton_h_
