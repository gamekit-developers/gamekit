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

#ifndef _liXmlUtils_h_
#define _liXmlUtils_h_

#include "liUtils.h"
#include "tinyxml.h"

namespace xml
{

bool openXMLDocument(const gkString& fileName, TiXmlDocument &XMLDoc, const gkString& resGroupName="General");

TiXmlElement* openXMLDocumentRoot(const gkString& fileName, TiXmlDocument &XMLDoc, 
								  const gkString& rootElementName=Empty_liStr, const gkString& resGroupName="General");


bool openXMLDocumentOnFile(const gkString& fileName, TiXmlDocument &XMLDoc);
TiXmlElement* openXMLDocumentRootOnFile(const gkString& fileName, TiXmlDocument &XMLDoc, 
								  const gkString& rootElementName=Empty_liStr);

//--


GK_INLINE int parseInt(const gkString& val)
{
	if (val.find("0x") == 0 || val.find("0X") == 0)
		return strtol(val.substr(2, val.length() - 2).c_str (), 0, 16);
	else
		return atoi(val.c_str());
}

GK_INLINE UTuint32 parseUInt(const gkString& val)
{
	if (val.find("0x") == 0 || val.find("0X") == 0)
		return strtoul(val.substr(2, val.length() - 2).c_str (), 0, 16);
	else
		return strtoul(val.c_str(), 0, 10);
}

GK_INLINE gkScalar parseReal(const gkString& val)
{
	return Ogre::StringConverter::parseReal(val);
}

GK_INLINE bool parseBool(const gkString& val)
{
	return Ogre::StringConverter::parseBool(val); //if val in (true, 1, yes): true
}

GK_INLINE gkVector2 parseVector2(const gkString& val)
{
	return Ogre::StringConverter::parseVector2(val);
}

GK_INLINE gkVector3 parseVector3(const gkString& val)
{
	return Ogre::StringConverter::parseVector3(val);
}

GK_INLINE gkVector4 parseVector4(const gkString& val)
{
	return Ogre::StringConverter::parseVector4(val);
}

GK_INLINE gkQuaternion parseQuaternion(const gkString& val)
{
	return Ogre::StringConverter::parseQuaternion(val);
}

GK_INLINE gkColor parseColourValue(const gkString& val)
{
	return Ogre::StringConverter::parseColourValue(val);
}

GK_INLINE gkMatrix3 parseMatrix3(const gkString& val)
{
	return Ogre::StringConverter::parseMatrix3(val);
}

GK_INLINE gkMatrix4 parseMatrix4(const gkString& val)
{
	return Ogre::StringConverter::parseMatrix4(val);
}


GK_INLINE liStrVec parseStrVec(const gkString& val)
{
	//return Ogre::StringConverter::parseStringVector(val);
	return strSplit(val);
}

//-- tag


GK_INLINE gkString getTabLineStr(const gkString& s, int tabno, bool line)
	{ gkString tab(tabno, '\t'); return line ? (tab + s + "\n") : (tab + s); }

GK_INLINE gkString getVectorTagStr(const gkString& tag, const gkVector3 &v)
	{ return utStringFormat("<%s x='%f' y='%f' z='%f'/>", tag.c_str(), v.x, v.y, v.z); }

GK_INLINE gkString getQuatTagStr(const gkString& tag, const gkQuaternion &q)
	{ return utStringFormat("<%s qx='%f' qy='%f' qz='%f'  qw='%f'/>", tag.c_str(), q.x, q.y, q.z, q.w); }

GK_INLINE gkString getColourTagStr(const gkString& tag, const gkColor &c)
	{ return utStringFormat("<%s r='%f' g='%f' b='%f' a='%f'/>", tag.c_str(), c.r, c.g, c.b, c.a); }

GK_INLINE gkString getTagStr(const gkString& tag, const gkVector3 &v, int tabno=1, bool line=true)
	{ return getTabLineStr(getVectorTagStr(tag, v), tabno, line); }

GK_INLINE gkString getTagStr(const gkString& tag, const gkQuaternion &q, int tabno=1, bool line=true)
	{ return getTabLineStr(getQuatTagStr(tag, q), tabno, line); }

GK_INLINE gkString getTagStr(const gkString& tag, const gkColor &c, int tabno=1, bool line=true)
	{ return getTabLineStr(getColourTagStr(tag, c), tabno, line); }

GK_INLINE gkString getBoolStr(bool b) { return b ? "true" : "false"; }
GK_INLINE gkString getIntStr(int i) { return utStringFormat("%d", i); }
GK_INLINE gkString getScalarStr(gkScalar s) { return utStringFormat("%f", s); }

//--


GK_INLINE const TiXmlElement* getFirstChildElm(const TiXmlElement* XMLNode, const gkString& name)
	{ return XMLNode ? XMLNode->FirstChildElement(name.c_str()) : NULL; }
GK_INLINE const TiXmlElement* getFirstSubChildElm(const TiXmlElement* XMLNode, const gkString& name, const gkString& subName)
	{ const TiXmlElement* child = getFirstChildElm(XMLNode, name); return child ? getFirstChildElm(child, subName) : NULL; }
GK_INLINE const TiXmlElement* getNextSiblingElm(const TiXmlElement* XMLNode)
	{ return XMLNode ? XMLNode->NextSiblingElement(XMLNode->Value()) : NULL; }


GK_INLINE const TiXmlElement* getFirstChildElm2(const TiXmlElement* XMLNode)
	{ return XMLNode ? XMLNode->FirstChildElement() : NULL; }
GK_INLINE const TiXmlElement* getFirstSubChildElm2(const TiXmlElement* XMLNode)
	{ const TiXmlElement* child = getFirstChildElm2(XMLNode); return child ? getFirstChildElm2(child) : NULL; }
GK_INLINE const TiXmlElement* getNextSiblingElm2(const TiXmlElement* XMLNode)
	{ return XMLNode ? XMLNode->NextSiblingElement() : NULL; }


#define IF_XML_CHILD(PARENT_NODE, NODE, CHILD_NAME) \
	if (const TiXmlElement* NODE= getFirstChildElm(PARENT_NODE, CHILD_NAME))

#define IF_XML_SUBCHILD(PARENT_NODE, NODE, CHILD_NAME, SUBCHILD_NAME) \
	if (TiXmlElement* NODE= getFirstSubChildElm(PARENT_NODE, CHILD_NAME, SUBCHILD_NAME))

#define FOR_XML_CHILD(PARENT_NODE, NODE, CHILD_NAME) \
	for (const TiXmlElement* NODE= getFirstChildElm(PARENT_NODE, CHILD_NAME); NODE != NULL; NODE = getNextSiblingElm(NODE))

#define FOR_XML_SUBCHILD(PARENT_NODE, NODE, CHILD_NAME, SUBCHILD_NAME) \
	for (const TiXmlElement* NODE= getFirstSubChildElm(PARENT_NODE, CHILD_NAME, SUBCHILD_NAME); NODE != NULL; NODE = getNextSiblingElm(NODE))


#define IF_XML_ANY_CHILD(PARENT_NODE, NODE) \
	if (const TiXmlElement* NODE= getFirstChildElm2(PARENT_NODE))

#define IF_XML_ANY_SUBCHILD(PARENT_NODE, NODE) \
	if (TiXmlElement* NODE= getFirstSubChildElm2(PARENT_NODE))

#define FOR_XML_ALL_CHILD(PARENT_NODE, NODE) \
	for (const TiXmlElement* NODE= getFirstChildElm2(PARENT_NODE); NODE != NULL; NODE = getNextSiblingElm2(NODE))

#define FOR_XML_ALL_SUBCHILD(PARENT_NODE, NODE) \
	for (const TiXmlElement* NODE= getFirstSubChildElm2(PARENT_NODE); NODE != NULL; NODE = getNextSiblingElm2(NODE))


GK_INLINE gkString getTagStr(const TiXmlElement* XMLNode)
	{ return XMLNode ? XMLNode->ValueStr() : ""; }

bool getValue(const TiXmlElement* XMLNode, gkString& value);
bool getValue(const TiXmlElement* XMLNode, gkScalar &value);
bool getValue(const TiXmlElement* XMLNode, int& value);
bool getValue(const TiXmlElement* XMLNode, UTuint32& value);
bool getValue(const TiXmlElement* XMLNode, bool& value);

template <typename T>
T getValueT(const TiXmlElement* XMLNode, const T &defaultValue)
{
	T val(defaultValue);
	getValue(XMLNode, val);
	return val;
}

GK_INLINE gkString getValueStr(const TiXmlElement* XMLNode, const gkString& defaultValue = Empty_liStr)
	{ return getValueT(XMLNode, defaultValue); }

GK_INLINE int getValueInt(const TiXmlElement* XMLNode, const int defaultValue = 0)
	{ return getValueT(XMLNode, defaultValue); }

GK_INLINE UTuint32 getValueUint(const TiXmlElement* XMLNode, const UTuint32 defaultValue = 0)
	{ return getValueT(XMLNode, defaultValue); }

GK_INLINE gkScalar getValueReal(const TiXmlElement* XMLNode, const gkScalar defaultValue = 0)
	{ return getValueT(XMLNode, defaultValue); }

GK_INLINE bool getValueBool(const TiXmlElement* XMLNode, const bool defaultValue = false)
	{ return getValueT(XMLNode, defaultValue); }

GK_INLINE bool hasAttrib(const TiXmlElement* XMLNode, const gkString& attrib) 
	{ return XMLNode && XMLNode->Attribute(attrib.c_str()) != NULL; }

bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, gkString& value);
bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, gkScalar &value);
bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, int& value);
bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, UTuint32& value);
bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, bool& value);

template <typename T>
T getAttribT(const TiXmlElement* XMLNode, const gkString& attrib, const T &defaultValue)
{
	T val(defaultValue);
	getAttrib(XMLNode, attrib,  val);
	return val;
}

GK_INLINE gkString getAttribStr(const TiXmlElement* XMLNode, const gkString& attrib, const gkString& defaultValue = Empty_liStr)
	{ return getAttribT(XMLNode, attrib, defaultValue); }

GK_INLINE gkScalar getAttribScalar(const TiXmlElement* XMLNode, const gkString& attrib, gkScalar defaultValue = 0)
	{ return getAttribT(XMLNode, attrib, defaultValue); }

GK_INLINE int getAttribInt(const TiXmlElement* XMLNode, const gkString& attrib, int defaultValue = 0)
	{ return getAttribT(XMLNode, attrib, defaultValue); }

GK_INLINE UTuint32 getAttribUInt(const TiXmlElement* XMLNode, const gkString& attrib, UTuint32 defaultValue = 0)
	{ return getAttribT(XMLNode, attrib, defaultValue); }

GK_INLINE bool getAttribBool(const TiXmlElement* XMLNode, const gkString& attrib, bool defaultValue = false)
	{ return getAttribT(XMLNode, attrib, defaultValue); }


template <typename T>
bool getElementT(const TiXmlElement* XMLNode, const gkString& name, T &val)
{
	const TiXmlElement* pElement = XMLNode->FirstChildElement(name.c_str());
	if (!pElement) return false;
	
	return getAttrib(pElement, name, val);	
}


GK_INLINE bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkString& val)
	{ return getElementT(XMLNode, name, val);  }

GK_INLINE bool getElement(const TiXmlElement* XMLNode, const gkString& name, bool& val)
	{ return getElementT(XMLNode, name, val);  }

GK_INLINE bool getElement(const TiXmlElement* XMLNode, const gkString& name, int& val)
	{ return getElementT(XMLNode, name, val);  }

GK_INLINE bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkScalar &val)
	{ return getElementT(XMLNode, name, val);  }

bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkVector3 &v, bool normalize = false);
bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkQuaternion &q);
bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkColor &c);



GK_INLINE gkVector3 getElmVector3(const TiXmlElement* XMLNode, const gkString& name, gkVector3 defaultValue=gkVector3::ZERO, bool normalize = false)
	{ getElement(XMLNode, name, defaultValue, normalize); return defaultValue; }

GK_INLINE gkQuaternion getElmQuaternion(const TiXmlElement* XMLNode, const gkString& name, gkQuaternion defaultValue = gkQuaternion::IDENTITY)
	{ getElement(XMLNode, name, defaultValue); return defaultValue; }

GK_INLINE gkColor getElmColour(const TiXmlElement* XMLNode, const gkString& name, gkColor defaultValue = gkColor::ZERO)
	{ getElement(XMLNode, name, defaultValue); return defaultValue; }


gkVector3     parseVector3(const TiXmlElement* XMLNode, bool normalize = false);
gkQuaternion  parseQuaternion(const TiXmlElement* XMLNode);
gkColor parseColour(const TiXmlElement* XMLNode);


//-- param

GK_INLINE int  getParamInt(liStrMap &props, const gkString& key, int defValue = 0)
	{ return props.find(key) != props.end() ? atoi(props[key].c_str()) : defValue; }

GK_INLINE gkScalar getParamReal(liStrMap &props, const gkString& key, gkScalar defValue = 0)
	{ return props.find(key) != props.end() ? atof(props[key].c_str()) : defValue; }

} //xml

#endif //_liXmlUtils_h_