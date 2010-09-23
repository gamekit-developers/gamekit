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
#ifndef _nsVariable_h_
#define _nsVariable_h_

#include "nsCommon.h"
#include "nsMath.h"
#include "nsString.h"
#include <typeinfo>


// Any value adapted from Poco
class nsValue
{
public:
	typedef std::type_info Info;


protected:

	// abstract holder
	class Content
	{
	public:
		virtual ~Content() {}


		virtual void            fromString(const nsString& v) = 0;
		virtual nsString        toString(void) const = 0;
		virtual const Info&      type(void) const = 0;
		virtual Content*         clone(void) const = 0;
	};


	// type holder
	template <typename T>
	class TypedContent : public Content
	{
	public:

		TypedContent(const T& v)
			:   m_value(v)
		{
		}

		virtual ~TypedContent() {}

		UT_INLINE const Info&   type(void) const    { return typeid(T); }
		UT_INLINE Content*       clone(void) const  { return new TypedContent<T>(m_value); }


		UT_INLINE void fromString(const nsString& v)
		{
			nsFromString(v, m_value);
		}

		UT_INLINE nsString  toString(void) const
		{
			return nsToString(m_value);
		}

		T m_value;
	};

	Content* m_content;


	UT_INLINE nsValue& swap(nsValue& rhs)
	{
		std::swap(m_content, rhs.m_content);
		return *this;
	}

public:

	nsValue()
		:   m_content(0)
	{
	}

	nsValue(const nsValue& v)
		:   m_content(v.m_content ? v.m_content->clone() : 0)
	{
	}

	template<typename T>
	nsValue(const T& v)
		:   m_content(new TypedContent<T>(v))
	{
	}

	~nsValue()
	{
		delete m_content;
	}

	template<typename T>
	UT_INLINE nsValue& operator = (const T& rhs)
	{
		nsValue(rhs).swap(*this);
		return *this;
	}

	UT_INLINE nsValue& operator = (const nsValue& rhs)
	{
		nsValue(rhs).swap(*this);
		return *this;
	}


	template<typename T>
	UT_INLINE operator T(void) const
	{
		if (m_content && m_content->type() == typeid(T))
			return static_cast<TypedContent<T>*>(m_content)->m_value;
		return T();
	}

	template<typename T>
	UT_INLINE T get(const T& def = T()) const
	{
		if (m_content && m_content->type() == typeid(T))
			return static_cast<TypedContent<T>*>(m_content)->m_value;
		return def;
	}


	UT_INLINE nsString toString(void) const
	{
		if (m_content)
			return m_content->toString();
		return "";
	}

	UT_INLINE void fromString(const nsString& v) const
	{
		if (m_content)
			m_content->fromString(v);
	}

	UT_INLINE bool isTypeOf(const nsValue& v) const
	{
		return m_content && v.m_content && m_content->type() ==  v.m_content->type();
	}
	UT_INLINE bool isTypeOf(const Info& v) const
	{
		return m_content && m_content->type() ==  v;
	}
};


#endif//_nsVariable_h_
