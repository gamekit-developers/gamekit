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

#include "StdAfx.h"
#include "liXmlConfig.h"

namespace xml
{

#define TAG_ENTRY   "entry"
#define TAG_GROUP   "group"
#define TAG_SECTION "section"

#define STR_DELIM ";"
#define NUM_DELIM " ,\t"

//-- config

xmlConfig::xmlConfig()
{
	mLoaded = false;
}

xmlConfig::~xmlConfig()
{
}

bool xmlConfig::load(const gkString &fileName)
{
	if (!openSections(fileName, mSections, "config")) {
		gkPrintf("[XML] Can't open file: '%s'", fileName.c_str());
		return false;
	}
	
	mLoaded = true;
	mFileName = fileName;

	return mLoaded;
}


//-- sections

void parseEntires(const TiXmlElement* XMLNode, liStrMap &entries)
{
	if (!XMLNode) return;

	FOR_XML_CHILD(XMLNode, pElement, TAG_ENTRY)
	{
		gkString name = getAttribStr(pElement, "name");
		gkString value = getAttribStr(pElement, "value");
		
		entries[name] = value;
	}
}

void parseGroups(const TiXmlElement* XMLNode, liStrGroups &groups)
{
	if (!XMLNode) return;

	FOR_XML_CHILD(XMLNode, pElement, TAG_GROUP)
	{
		gkString name = getAttribStr(pElement, "name");
		groups[name] = liStrMap();
		liStrMap &entries = groups[name];

		parseEntires(pElement, entries);
	}
}

void parseSections(const TiXmlElement* XMLNode, liStrSections &sections)
{
	if (!XMLNode) return;

	FOR_XML_CHILD(XMLNode, pElement, TAG_SECTION)
	{
		gkString name = getAttribStr(pElement, "name");
		sections[name] = liStrGroups();
		liStrGroups &groups = sections[name];

		parseGroups(pElement, groups);
	}
}

bool openSections(const gkString &fileName, liStrSections &sections,
				  const gkString &rootElementName, const gkString &resGroupName)
{
	TiXmlDocument  XMLDoc;
	TiXmlElement  *XMLRoot = openXMLDocumentRoot(fileName, XMLDoc, rootElementName, resGroupName);

	if (!XMLRoot) return false;

	parseSections(XMLRoot, sections);

	return true;
} 

bool isValueExists(const liStrSections& sections, const gkString &key)
{	
	gkString value;
	
	return getValue(sections, key, value);
}

//[section].[group].[entry]
bool getValue(const liStrSections& sections, const gkString &key, gkString &value)
{
	liStrVec skeys = strSplit(key, ".");
	if (skeys.size() != 3) return false;

	liStrSections::const_iterator i = sections.find(skeys[0]);
	if (i == sections.end()) return false;

	const liStrGroups &groups = i->second;
	liStrGroups::const_iterator t = groups.find(skeys[1]);
	if (t == groups.end()) return false;

	const liStrMap &entries = t->second;
	liStrMap::const_iterator u = entries.find(skeys[2]);
	if (u == entries.end()) return false;
	
	value = u->second;
	return true;
}

liStrVec xmlConfig::getStrVec(const gkString &name)
{
	liStrVec v;
	parseStrArray(v, getString(name), STR_DELIM);
	return v;
}

liIntVec xmlConfig::getIntVec(const gkString &name)
{
	liIntVec v;
	parseIntArray(v, getString(name), NUM_DELIM);
	return v;
}

liScalarVec xmlConfig::getScalarVec(const gkString &name)
{
	liScalarVec v;
	parseScalarArray(v, getString(name), NUM_DELIM);
	return v;
}

} //xml