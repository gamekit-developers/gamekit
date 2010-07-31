/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#include "gksProperty.h"

// ----------------------------------------------------------------------------
gksProperty::gksProperty(gkVariable *oth)
	:   m_var(oth)
{

}

// ----------------------------------------------------------------------------
int gksProperty::getType(void)
{
	if (m_var)
	{
		switch (m_var->getType())
		{
		case gkVariable::VAR_BOOL:      return BOOL;
		case gkVariable::VAR_INT:       return INT;
		case gkVariable::VAR_REAL:      return FLOAT;
		case gkVariable::VAR_STRING:    return STRING;
		}
	}

	return -1;
}


// ----------------------------------------------------------------------------
bool gksProperty::getValueBool(void)
{
	if (m_var)
		return m_var->getValueBool();
	return false;
}


// ----------------------------------------------------------------------------
int gksProperty::getValueInt(void)
{
	if (m_var)
		return m_var->getValueInt();
	return -1;
}


// ----------------------------------------------------------------------------
float gksProperty::getValueReal(void)
{
	if (m_var)
		return m_var->getValueReal();
	return 0.f;
}


// ----------------------------------------------------------------------------
const char *gksProperty::getValueString(void)
{
	static char buffer[128];

	if (m_var)
	{
		sprintf(buffer, "%s", m_var->getValueString().c_str());
	}
	else
		buffer[0] = 0;
	return buffer;
}

// ----------------------------------------------------------------------------
void gksProperty::setBool(bool v)
{
	if (m_var)
		m_var->setValue(v);
}


// ----------------------------------------------------------------------------
void gksProperty::setInt(int v)
{
	if (m_var)
		m_var->setValue(v);
}


// ----------------------------------------------------------------------------
void gksProperty::setFloat(float v)
{
	if (m_var)
		m_var->setValue(v);
}


// ----------------------------------------------------------------------------
void gksProperty::setString(const char *v)
{
	if (m_var)
		m_var->setValue(gkString(v));
}
