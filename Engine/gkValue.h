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
#ifndef _gkValue_h_
#define _gkValue_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "gkString.h"
#include "OgreStringConverter.h"
#include <typeinfo>



GK_INLINE void gkFromString(const gkString& s, bool& v)         { v = Ogre::StringConverter::parseBool(s); }
GK_INLINE void gkFromString(const gkString& s, int& v)          { v = Ogre::StringConverter::parseInt(s); }
GK_INLINE void gkFromString(const gkString& s, gkScalar& v)     { v = Ogre::StringConverter::parseReal(s); }
GK_INLINE void gkFromString(const gkString& s, gkString& v)     { v = s; }
GK_INLINE void gkFromString(const gkString& s, gkVector2& v)    { v = Ogre::StringConverter::parseVector2(s); }
GK_INLINE void gkFromString(const gkString& s, gkVector3& v)    { v = Ogre::StringConverter::parseVector3(s); }
GK_INLINE void gkFromString(const gkString& s, gkVector4& v)    { v = Ogre::StringConverter::parseVector4(s); }
GK_INLINE void gkFromString(const gkString& s, gkQuaternion& v) { v = Ogre::StringConverter::parseQuaternion(s); }
GK_INLINE void gkFromString(const gkString& s, gkMatrix3& v)    { v = Ogre::StringConverter::parseMatrix3(s); }
GK_INLINE void gkFromString(const gkString& s, gkMatrix4& v)    { v = Ogre::StringConverter::parseMatrix4(s); }


GK_INLINE gkString gkToString(bool v)                { return v ? "true" : "false"; }
GK_INLINE gkString gkToString(int v)                 { return Ogre::StringConverter::toString(v); }
GK_INLINE gkString gkToString(gkScalar v)            { return Ogre::StringConverter::toString(v); }
GK_INLINE gkString gkToString(const gkString& v)     { return v; }
GK_INLINE gkString gkToString(const gkVector2& v)    { return Ogre::StringConverter::toString(v); }
GK_INLINE gkString gkToString(const gkVector3& v)    { return Ogre::StringConverter::toString(v); }
GK_INLINE gkString gkToString(const gkVector4& v)    { return Ogre::StringConverter::toString(v); }
GK_INLINE gkString gkToString(const gkQuaternion& v) { return Ogre::StringConverter::toString(v); }
GK_INLINE gkString gkToString(const gkMatrix3& v)    { return Ogre::StringConverter::toString(v); }
GK_INLINE gkString gkToString(const gkMatrix4& v)    { return Ogre::StringConverter::toString(v); }


class gkValue
{
public:
	typedef std::type_info Info;


protected:

	class Value
	{
	public:
		virtual ~Value() {}


		virtual void          fromString(const gkString& v) = 0;
		virtual gkString      toString(void) const = 0;
		virtual const Info&    type(void) const = 0;
		virtual Value*         clone(void) const = 0;
	};

	template <typename T>
	class ValueType : public Value
	{
	public:

		ValueType(const T& v)
			:   m_value(v)
		{
		}

		virtual ~ValueType() {}

		GK_INLINE const Info&  type(void) const    { return typeid(T); }
		GK_INLINE Value*        clone(void) const  { return new ValueType<T>(m_value); }

		GK_INLINE void fromString(const gkString& v)
		{
			gkFromString(v, m_value);
		}

		GK_INLINE gkString toString(void) const
		{
			return gkToString(m_value);
		}
		T m_value;
	};

	Value* m_data;


	GK_INLINE gkValue& swap(gkValue& rhs)
	{
		utSwap(m_data, rhs.m_data);
		return *this;
	}

public:

	gkValue()
		:   m_data(0)
	{
	}

	gkValue(const gkValue& v)
		:   m_data(v.m_data ? v.m_data->clone() : 0)
	{
	}

	template<typename T>
	gkValue(const T& v)
		:   m_data(new ValueType<T>(v))
	{
	}

	~gkValue()
	{
		delete m_data;
	}

	template<typename T>
	GK_INLINE gkValue& operator = (const T& rhs)
	{
		gkValue(rhs).swap(*this);
		return *this;
	}

	GK_INLINE gkValue& operator = (const gkValue& rhs)
	{
		gkValue(rhs).swap(*this);
		return *this;
	}


	template<typename T>
	GK_INLINE operator T(void) const
	{
		if (m_data && m_data->type() == typeid(T))
			return static_cast<ValueType<T>*>(m_data)->m_value;
		return T();
	}

	template<typename T>
	GK_INLINE T get(const T& def = T()) const
	{
		if (m_data && m_data->type() == typeid(T))
			return static_cast<ValueType<T>*>(m_data)->m_value;
		return def;
	}


	GK_INLINE gkString toString(void) const
	{
		if (m_data)
			return m_data->toString();
		return "";
	}

	GK_INLINE void fromString(const gkString& v) const
	{
		if (m_data)
			m_data->fromString(v);
	}

	GK_INLINE bool isTypeOf(const gkValue& v) const
	{
		return m_data && v.m_data && m_data->type() ==  v.m_data->type();
	}

	GK_INLINE bool isTypeOf(const Info& v) const
	{
		return m_data && m_data->type() ==  v;
	}
};


#endif//_gkValue_h_
