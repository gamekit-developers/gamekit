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
#ifndef _gsUtils_h_
#define _gsUtils_h_

#include "gsCommon.h"






template <typename B>
class gsArrayIterator
{
public:
	typedef utArray<B*> Array;

	// FIXME: ut*Iterator needs const as well.
	typedef utArrayIterator<Array> Iterator;


private:

	Iterator m_iter;

public:

#ifndef SWIG
	gsArrayIterator(Array& a) : m_iter(a) {}
#endif

	gsArrayIterator()    {}
	~gsArrayIterator()    {}


	inline bool hasMoreElements(void) {return m_iter.hasMoreElements();}
	inline void moveNext(void) { m_iter.next();}

	inline B* peekNext(void)
	{
		if (m_iter.hasMoreElements())
		{
			B* base = m_iter.peekNext();
			return base;
		}
		return 0;
	}

	inline B* getNext(void)
	{
		if (m_iter.hasMoreElements())
		{
			B* base = m_iter.getNext();
			return base;
		}
		return 0;
	}
};




template <typename W, typename B>
class gsArray
{
public:
	typedef utArray<B*>            Array;

private:

	Array m_array;

public:


	gsArray() {}
	gsArray(const Array& a) : m_array(a) {}
	~gsArray() { clear(); }


	inline void clear(void) { m_array.clear(); }

	inline int    size(void)  const { return (int)m_array.size(); }
	inline bool empty(void) const { return size() == 0; }

	inline B* __getitem__(int i) {return at(i);}

	inline B* at(int i)
	{
		if (i >= 0 && i < size())
		{
			B* base = m_array.at(i);
			return base;
		}
		return 0;
	}


	inline void push(W* val)
	{
		if (val)
		{
			B* intern = OGRE_KIT_OBJECT(B, val);
			if (intern)
				m_array.push_back(intern);
		}
	}

	bool erase(W* val)
	{
		if (val)
		{
			B* intern = OGRE_KIT_OBJECT(B, val);
			if (intern)
			{
				UTsize pos = m_array.find(intern);
				if (pos != UT_NPOS)
				{
					m_array.erase(pos);
					return true;
				}
			}
		}
		return false;
	}

	gsArrayIterator<B> iterator()
	{
		return gsArrayIterator<B>(m_array);
	}


#ifndef SWIG
	inline void push(B* val)
	{
		m_array.push_back(val);
	}

	inline B* iat(int i)
	{
		if (i >= 0 && i < size())
			return m_array.at(i);
		return 0;
	}

	Array& getIterAccess(void) {return m_array;}
#endif
};




#endif//_gsUtils_h_
