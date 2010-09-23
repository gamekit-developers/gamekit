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
#include "gkVariable.h"




gkVariable::gkVariable()
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
}



gkVariable::gkVariable(const gkString& n, bool dbg)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(n),
	     m_debug(dbg), m_lock(false)
{
}


gkVariable::gkVariable(bool v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}


gkVariable::gkVariable(int v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}


gkVariable::gkVariable(gkScalar v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}

gkVariable::gkVariable(const gkString& v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}

gkVariable::gkVariable(const gkVector2& v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}

gkVariable::gkVariable(const gkVector3& v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}

gkVariable::gkVariable(const gkVector4& v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}
gkVariable::gkVariable(const gkQuaternion& v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}

gkVariable::gkVariable(const gkMatrix3& v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}

gkVariable::gkVariable(const gkMatrix4& v, const gkString& name)
	:    m_value((int)0),
	     m_type(VAR_NULL),
	     m_name(""),
	     m_debug(false), m_lock(false)
{
	setValue(v);
}



gkVariable::~gkVariable()
{
}


void gkVariable::makeDefault(void)
{
	m_default = m_value;
}


void gkVariable::reset(void)
{
	m_value = m_default;
}


gkVariable* gkVariable::clone(void)
{
	gkVariable* v = new gkVariable(*this);
	v->m_value = gkValue(m_value);
	return v;
}


void gkVariable::setValue(int type, const gkString& v)
{
	if (!m_lock)
	{
		m_type = type;
		assign(v);
	}
}


void gkVariable::setValue(gkScalar v)
{
	if (!m_lock)
	{
		m_type = VAR_REAL;
		m_value = v;
	}
}



void gkVariable::setValue(bool v)
{
	if (!m_lock)
	{
		m_type = VAR_BOOL;
		m_value = v;
	}
}



void gkVariable::setValue(int v)
{
	if (!m_lock)
	{
		m_type = VAR_INT;
		m_value = v;
	}
}


void gkVariable::setValue(const gkString& v)
{
	if (!m_lock)
	{
		m_type = VAR_STRING;
		m_value = v;
	}
}


void gkVariable::setValue(const gkVector2& v)
{
	if (!m_lock)
	{
		m_type = VAR_VEC2;
		m_value = v;
	}
}


void gkVariable::setValue(const gkVector3& v)
{
	if (!m_lock)
	{
		m_type = VAR_VEC3;
		m_value = v;
	}
}


void gkVariable::setValue(const gkVector4& v)
{
	if (!m_lock)
	{
		m_type = VAR_VEC4;
		m_value = v;
	}
}


void gkVariable::setValue(const gkQuaternion& v)
{
	if (!m_lock)
	{
		m_type = VAR_QUAT;
		m_value = v;
	}
}


void gkVariable::setValue(const gkMatrix3& v)
{
	if (!m_lock)
	{
		m_type = VAR_MAT3;
		m_value = v;
	}
}


void gkVariable::setValue(const gkMatrix4& v)
{
	if (!m_lock)
	{
		m_type = VAR_MAT4;
		m_value = v;
	}
}


void gkVariable::setValue(const gkVariable& v)
{
	if (!m_lock)
	{
		m_type  = v.m_type;
		m_value = v.m_value;
		m_debug = v.m_debug;
		m_name  = v.m_name;
	}
}


bool gkVariable::getValueBool(void) const
{
	switch (m_type)
	{
	case VAR_INT:
		return m_value.get<int>() != 0;
	case VAR_REAL:
		return m_value.get<gkScalar>() != 0.f;
	case VAR_BOOL:
		return m_value.get<bool>();
	default:
		{
			bool v;
			gkFromString(m_value.toString(), v);
			return v;
		}
	}
	return false;
}


gkScalar gkVariable::getValueReal(void) const
{
	switch (m_type)
	{
	case VAR_INT:
		return (gkScalar)m_value.get<int>();
	case VAR_REAL:
		return m_value.get<gkScalar>();
	case VAR_BOOL:
		return m_value.get<bool>() ? 1.f : 0.f;
	default:
		{
			gkScalar v;
			gkFromString(m_value.toString(), v);
			return v;
		}
	}
	return 0.f;
}



int gkVariable::getValueInt(void) const
{
	switch (m_type)
	{
	case VAR_INT:
		return m_value.get<int>(0);
	case VAR_REAL:
		return (int)m_value.get<gkScalar>(0.f);
	case VAR_BOOL:
		return m_value.get<bool>(false) ? 1 : 0;
	default:
		{
			int v;
			gkFromString(m_value.toString(), v);
			return v;
		}
	}
	return 0;
}


gkString gkVariable::getValueString(void) const
{
	return m_value.toString();
}



gkVector2 gkVariable::getValueVector2(void) const
{
	return m_value.get<gkVector2>(gkVector2(0, 0));
}


gkVector3 gkVariable::getValueVector3(void) const
{
	return m_value.get<gkVector3>(gkVector3(0, 0, 0));
}



gkVector4 gkVariable::getValueVector4(void) const
{
	return m_value.get<gkVector4>(gkVector4(0, 0, 0, 1));
}


gkQuaternion gkVariable::getValueQuaternion(void) const
{
	return m_value.get<gkQuaternion>(gkQuaternion::IDENTITY);
}


gkMatrix3 gkVariable::getValueMatrix3(void) const
{
	return m_value.get<gkMatrix3>(gkMatrix3::IDENTITY);
}


gkMatrix4 gkVariable::getValueMatrix4(void) const
{
	return m_value.get<gkMatrix4>(gkMatrix4::IDENTITY);
}


bool gkVariable::operator < (const gkVariable& o) const
{
	switch (m_type)
	{
	case VAR_BOOL: return (int)getValueBool()  < (int)o.getValueBool();
	case VAR_INT:  return getValueInt()        < o.getValueInt();
	case VAR_REAL: return getValueReal()       < o.getValueReal();
	case VAR_VEC2: return getValueVector2()    < o.getValueVector2();
	case VAR_VEC3: return getValueVector3()    < o.getValueVector3();
	default:
		return getValueString() < o.getValueString();
	}
	return false;
}


bool gkVariable::operator > (const gkVariable& o) const
{
	switch (m_type)
	{
	case VAR_BOOL: return (int)getValueBool()  > (int)o.getValueBool();
	case VAR_INT:  return getValueInt()        > o.getValueInt();
	case VAR_REAL: return getValueReal()       > o.getValueReal();
	case VAR_VEC2: return getValueVector2()    > o.getValueVector2();
	case VAR_VEC3: return getValueVector3()    > o.getValueVector3();
	default:
		return getValueString() > o.getValueString();
	}
	return false;
}


bool gkVariable::operator <= (const gkVariable& o) const
{
	return (*this).operator < (o) || (*this).operator == (o);
}


bool gkVariable::operator >= (const gkVariable& o) const
{
	return (*this).operator > (o) || (*this).operator == (o);
}



bool gkVariable::operator == (const gkVariable& o) const
{
	switch (m_type)
	{
	case VAR_BOOL: return (int)getValueBool()  == (int)o.getValueBool();
	case VAR_INT:  return getValueInt()        == o.getValueInt();
	case VAR_REAL: return getValueReal()       == o.getValueReal();
	case VAR_VEC2: return getValueVector2()    == o.getValueVector2();
	case VAR_VEC3: return getValueVector3()    == o.getValueVector3();
	case VAR_VEC4: return getValueVector4()    == o.getValueVector4();
	case VAR_QUAT: return getValueQuaternion() == o.getValueQuaternion();
	case VAR_MAT3: return getValueMatrix3()    == o.getValueMatrix3();
	case VAR_MAT4: return getValueMatrix4()    == o.getValueMatrix4();
	default:
		return getValueString() == o.getValueString();
	}
	return false;
}


bool gkVariable::operator != (const gkVariable& o) const
{
	return !(*this).operator == (o);
}



void gkVariable::assign(const gkString& o)
{
	if (!m_lock)
	{
		m_type  = VAR_STRING;
		m_value = o;
	}
}



void gkVariable::add(const gkString& o)
{
	if (!m_lock)
	{
		gkVariable nv;
		nv.setValue(o);
		add(nv);
	}
}



void gkVariable::inverse(const gkString& o)
{
	if (!m_lock)
	{
		gkVariable nv;
		nv.setValue(o);
		inverse(nv);
	}
}




void gkVariable::toggle(const gkString& o)
{
	if (!m_lock)
	{
		inverse(o);
	}
}


void gkVariable::assign(const gkVariable& nv)
{
	if (!m_lock)
	{
		m_type  = nv.m_type;
		m_value = nv.m_value;
	}
}


void gkVariable::add(const gkVariable& nv)
{
	if (!m_lock)
	{
		switch (m_type)
		{
		case VAR_BOOL:  setValue(getValueBool()      != nv.getValueBool());       break;
		case VAR_INT:   setValue(getValueInt()        + nv.getValueInt());        break;
		case VAR_REAL:  setValue(getValueReal()       + nv.getValueReal());       break;
		case VAR_VEC2:  setValue(getValueVector2()    + nv.getValueVector2());    break;
		case VAR_VEC3:  setValue(getValueVector3()    + nv.getValueVector3());    break;
		case VAR_VEC4:  setValue(getValueVector4()    + nv.getValueVector4());    break;
		case VAR_QUAT:  setValue(getValueQuaternion() * nv.getValueQuaternion()); break;
		case VAR_MAT3:  setValue(getValueMatrix3()    * nv.getValueMatrix3());    break;
		case VAR_MAT4:  setValue(getValueMatrix4()    * nv.getValueMatrix4());    break;
		default:
			setValue(getValueString() + nv.getValueString());
			break;
		}
	}
}



bool gkVariable::hasInverse(void)
{
	switch (m_type)
	{
	case VAR_BOOL:
	case VAR_INT:
	case VAR_REAL:
	case VAR_VEC2:
	case VAR_VEC3:
	case VAR_VEC4:
		return true;
	case VAR_QUAT:
	case VAR_MAT3:
	case VAR_MAT4:
	default:
		break;
	}
	return false;
}


void gkVariable::inverse(const gkVariable& nv)
{
	if (!m_lock)
	{
		switch (m_type)
		{
		case VAR_BOOL:  setValue(!nv.getValueBool());            break;
		case VAR_INT:   setValue(nv.getValueInt()  ? 0   : 1);   break;
		case VAR_REAL:  setValue(nv.getValueReal() ? 0.f : 1.f); break;
		case VAR_VEC2:  setValue(-(nv.getValueVector2()));       break;
		case VAR_VEC3:  setValue(-(nv.getValueVector3()));       break;
		case VAR_VEC4:  setValue(-(nv.getValueVector4()));       break;
		case VAR_QUAT:
		case VAR_MAT3:
		case VAR_MAT4:
		default:
			break;
		}
	}
}



void gkVariable::toggle(const gkVariable& o)
{
	if (!m_lock)
	{
		inverse(o);
	}
}
