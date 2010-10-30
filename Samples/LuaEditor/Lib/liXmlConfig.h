/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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

#ifndef _liXmlConfig_h_
#define _liXmlConfig_h_

#include "liXmlUtils.h"


namespace xml
{

//-- section

void parseEntires(const TiXmlElement* XMLNode, liStrMap &entries);
void parseGroups(const TiXmlElement* XMLNode, liStrGroups &groups);
void parseSections(const TiXmlElement* XMLNode, liStrSections &sections);

bool openSections(const gkString& fileName, liStrSections &sections,
				  const gkString& rootElementName="", const gkString& resGroupName="General");


//[section].[group].[entry]=value
bool isValueExists(const liStrSections& sections, const gkString& key);
bool getValue(const liStrSections& sections, const gkString& key, gkString& value);
GK_INLINE gkString getValueStr(const liStrSections& sections, const gkString& key, const gkString& defValue=Empty_liStr)
{
	gkString val;
	return getValue(sections, key, val) ? val : defValue;
}


//-- config

class xmlConfig 
{
protected:
	bool mLoaded;
	liStrSections mSections;
	gkString mFileName;

public:

	xmlConfig();
	virtual ~xmlConfig();

	//--

	virtual bool load(const gkString& fileName);
	bool isLoaded() { return mLoaded; }
	gkString getFileName() { return mFileName; }

	//--
	
	bool isExists(const gkString& name) { return isValueExists(mSections, name); }

	liStrGroups &operator[](const gkString& name) { return mSections[name]; }

	//--

	bool getValue(const gkString& name, gkString& value);
	bool getValue(const gkString& name, int &value);
	bool getValue(const gkString& name, bool &value);
	bool getValue(const gkString& name, gkScalar &value);
	

	//--

	gkString	getString(const gkString& name, const gkString& defValue = "")	{ return getValueStr(mSections, name, defValue);  }
	int			getInt(const gkString& name, const int defValue = 0)			{ return parseInt(getString(name, getIntStr(defValue)));  }
	bool		getBool(const gkString& name, const bool defValue = false)		{ return parseBool(getString(name, getBoolStr(defValue))); }
	gkScalar	getScalar(const gkString& name, const gkScalar defValue = 0)	{ return parseReal(getString(name, getScalarStr(defValue))); }

	//--

	gkVector2		getVector2(const gkString& name)	   { return parseVector2(getString(name));      }
	gkVector3		getVector3(const gkString& name)       { return parseVector3(getString(name));      }
	gkVector4		getVector4(const gkString& name)       { return parseVector4(getString(name));      }
	gkQuaternion	getQuaternion(const gkString& name)    { return parseQuaternion(getString(name));   }
	gkColor			getColourValue(const gkString& name)   { return parseColourValue(getString(name));  }
	
	
	gkMatrix3		getMatrix3(const gkString& name)       { return parseMatrix3(getString(name));      }
	gkMatrix4		getMatrix4(const gkString& name)       { return parseMatrix4(getString(name));      }

	liStrVec		getStrVec(const gkString& name);
	liIntVec		getIntVec(const gkString& name);
	liScalarVec		getScalarVec(const gkString &name);
};


//-- GK_INLINE

GK_INLINE bool xmlConfig::getValue(const gkString& name, gkString& value) 
{ 
	return xml::getValue(mSections, name, value); 
}

GK_INLINE bool xmlConfig::getValue(const gkString& name, int &value)  
{ 
	gkString s; 
	if (!getValue(name, s)) return false;
	value = parseInt(s);
	return true;
}

GK_INLINE bool xmlConfig::getValue(const gkString& name, bool &value)
{ 
	gkString s; 
	if (!getValue(name, s)) return false;
	value = parseBool(s);
	return true;
}

GK_INLINE bool xmlConfig::getValue(const gkString& name, gkScalar &value) 
{ 
	gkString s; 
	if (!getValue(name, s)) return false;
	value = parseReal(s);
	return true;
}

}

#endif //_liXmlConfig_h_