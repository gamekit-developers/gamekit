/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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


/** \addtogroup Utils
	@{
*/



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

	/**
		\LuaMethod{ArrayIterator,constructor}



		Creates an empty iterator.

		\code
		function ArrayIterator:constructor()
		\endcode


		
	*/
	gsArrayIterator()    {}
	~gsArrayIterator()    {}

	/**
		\LuaMethod{ArrayIterator,hasMoreElements}


		Returns true if more iterations are present.

		\code
		function ArrayIterator:hasMoreElements()
		\endcode

		\returns bool

		
	*/
	inline bool hasMoreElements(void) {return m_iter.hasMoreElements();}
	/**
		\LuaMethod{ArrayIterator,moveNext}


		Advances to the next iteration.

		\code
		function ArrayIterator:moveNext()
		\endcode

		
	*/
	inline void moveNext(void) { m_iter.next();}

	/**
		\LuaMethod{ArrayIterator,peekNext}


		Returns the next iteration, without advancing.

		\code
		function ArrayIterator:peekNext()
		\endcode

		\returns T The data type contained in the array. 

		

	*/
	inline B* peekNext(void)
	{
		if (m_iter.hasMoreElements())
		{
			B* base = m_iter.peekNext();
			return base;
		}
		return 0;
	}

	/**
		\LuaMethod{ArrayIterator,getNext}


		Returns the current iteration, and advances to the next.

		\code
		function ArrayIterator:getNext()
		\endcode

		\returns T The data type contained in the array. 


		
	*/
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

	/**
		\LuaMethod{Array,constructor}



		Creates an empty array.


		\code
		function Array:constructor()
		\endcode


		
	*/
	gsArray() {}
	/**

	*/
	gsArray(const Array& a) : m_array(a) {}
	~gsArray() { clear(); }

	/**
		\LuaMethod{Array,clear}


		Removes all elements from the array.

		\code
		function Array:clear()
		\endcode


		
	*/
	inline void clear(void) { m_array.clear(); }
	/**
		\LuaMethod{Array,size}


		Returns the number of elements in this array.

		\code
		function Array:size()
		\endcode

		\returns number


		
	*/
	inline int  size(void)  const { return (int)m_array.size(); }
	/**
		\LuaMethod{Array,empty}


		True if the array has no elements.

		\code
		function Array:empty()
		\endcode

		\returns bool


		
	*/
	inline bool empty(void) const { return size() == 0; }
	/**
		\LuaMethod{Array,__index}


		Index meta method.

		\code
		function Array:__index(number)
		\endcode

		\param number array index equal or greater than zero. 
		\returns T or nil if the array index is greater than it's size

		
	*/
	inline B* __getitem__(int i) {return at(i);}
	/**
		\LuaMethod{Array,at}


		Returns the element at the array index specified.

		\code
		function Array:at(number)
		\endcode

		\param number array index equal or greater than zero. 
		\returns T or nil if the array index is greater than it's size

		
	*/
	inline B* at(int i)
	{
		if (i >= 0 && i < size())
		{
			B* base = m_array.at(i);
			return base;
		}
		return 0;
	}

	/**
		\LuaMethod{Array,push}


		Adds an element to the back of the array.

		\code
		function Array:push(T)
		\endcode


		\param T Generic type depending on the type contained in the array.

		
	*/
	inline void push(W* val)
	{
		if (val)
		{
			B* intern = OGRE_KIT_OBJECT(B, val);
			if (intern)
				m_array.push_back(intern);
		}
	}
	/**
		\LuaMethod{Array,erase}


		Removes an element from the array.


		\code
		function Array:erase(T)
		\endcode


		\param T Generic type depending on the type contained in the array.
		\returns bool True if the object was removed, false if T is not in the array.

		
	*/
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
	/**
		\LuaMethod{Array,iterator}


		Returns an iterator for all elements in this array.


		\code
		function Array:iterator()
		\endcode

		\returns \LuaClassRef{ArrayIterator}


		
	*/
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


/** @} */

#endif//_gsUtils_h_
