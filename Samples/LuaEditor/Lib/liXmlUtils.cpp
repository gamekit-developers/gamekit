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
#include "liUtils.h"
#include "liXmlUtils.h"

namespace xml
{

bool openXMLDocumentOnFile(const gkString& fileName, TiXmlDocument &XMLDoc)
{
	if (!XMLDoc.LoadFile(fileName)) 
	{
		gkPrintf("[XML] Error - The TiXmlDocument reported an error: %s",XMLDoc.ErrorDesc());
		return false;
	}

	return true;
}

TiXmlElement* openXMLDocumentRootOnFile(const gkString& fileName, TiXmlDocument &XMLDoc, const gkString& rootElementName)
{
	if (!openXMLDocumentOnFile(fileName, XMLDoc))
		return NULL;
	
	TiXmlElement* XMLRoot = XMLDoc.RootElement();
	if (!rootElementName.empty() && rootElementName !=  XMLRoot->Value()) 
	{
		gkPrintf( "[XML] Error - Invalid .xml File. Missing %s", rootElementName.c_str());
		return NULL;
	}

	return XMLRoot;
}

bool openXMLDocument(const gkString& fileName, TiXmlDocument &XMLDoc, const gkString& resGroupName)
{
	try
	{
		//open from file
		if (!Ogre::ResourceGroupManager::getSingletonPtr())
			return openXMLDocumentOnFile(fileName, XMLDoc);

		//open from resource

		// Strip the path
		gkString basename, path;
		Ogre::StringUtil::splitFilename(fileName, basename, path);

		Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().
			openResource( basename, resGroupName );

		gkString data = pStream->getAsString();
		// Open the .scene File
		
		XMLDoc.Parse( data.c_str() );
		pStream->close();
		pStream.setNull();

		if (XMLDoc.Error())
		{			
			gkPrintf("[XML] Error - The TiXmlDocument reported an error: %s", XMLDoc.ErrorDesc());
			return false;
		}
	} 
	catch(Ogre::Exception &e) 
	{		
		gkPrintf("[XML] Error - creating TiXmlDocument %s %s", fileName.c_str(), e.getDescription().c_str());
		return false;
	}

	return true;
}

TiXmlElement* openXMLDocumentRoot(const gkString& fileName, TiXmlDocument &XMLDoc, 
							  const gkString& rootElementName, const gkString& resGroupName)
{
	if (!openXMLDocument(fileName, XMLDoc, resGroupName))
		return NULL;
	
	// Validate the File
	TiXmlElement* XMLRoot = XMLDoc.RootElement();
	if (!rootElementName.empty() && rootElementName !=  XMLRoot->Value()) {
		gkPrintf( "[XML] Error - Invalid .xml File. Missing %s", rootElementName.c_str());
		return NULL;
	}

	return XMLRoot;
}


//--

bool getValue(const TiXmlElement* XMLNode, gkString& value)
{
	if (!XMLNode || !XMLNode->GetText()) 
		return false;
	
	value = XMLNode->GetText();
	return true;
}

bool getValue(const TiXmlElement* XMLNode, gkScalar &value)
{
	if (!XMLNode || !XMLNode->GetText()) 
		return false;
	
	value = parseReal(XMLNode->GetText());
	return true;
}

bool getValue(const TiXmlElement* XMLNode, int& value)
{
	if (!XMLNode || !XMLNode->GetText()) 
		return false;
	
	value = parseInt(XMLNode->GetText());
	return true;
}

bool getValue(const TiXmlElement* XMLNode, UTuint32& value)
{
	if (!XMLNode || !XMLNode->GetText()) 
		return false;
	
	value = parseUInt(XMLNode->GetText());
	return true;
}

bool getValue(const TiXmlElement* XMLNode, bool& value)
{
	if (!XMLNode || !XMLNode->GetText()) 
		return false;
	
	value = parseBool(XMLNode->GetText());
	return true;
}


//--


bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, gkString& value)
{
	if (!XMLNode || !XMLNode->Attribute(attrib)) return false;

	value = XMLNode->Attribute(attrib.c_str());
	return true;
}

bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, gkScalar &value)
{
	if (!XMLNode || !XMLNode->Attribute(attrib)) return false;

	value = parseReal(XMLNode->Attribute(attrib.c_str()));
	return true;
}

bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, int& value)
{
	if (!XMLNode || !XMLNode->Attribute(attrib)) return false;

	value = parseInt(XMLNode->Attribute(attrib.c_str()));
	return true;
}

bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, UTuint32& value)
{
	if (!XMLNode || !XMLNode->Attribute(attrib)) return false;

	value = parseUInt(XMLNode->Attribute(attrib.c_str()));
	return true;
}

bool getAttrib(const TiXmlElement* XMLNode, const gkString& attrib, bool& value)
{
	if (!XMLNode || !XMLNode->Attribute(attrib)) return false;

	value = parseBool(XMLNode->Attribute(attrib.c_str()));
	return true;
}

//--

gkVector3 parseVector3(const TiXmlElement* XMLNode, bool normalize)
{
	gkVector3 v(getAttribScalar(XMLNode, "x"), getAttribScalar(XMLNode, "y"), getAttribScalar(XMLNode, "z"));

	if (normalize) v.normalise();

	return v;
}

gkQuaternion parseQuaternion(const TiXmlElement* XMLNode)
{
	gkQuaternion orientation(gkQuaternion::IDENTITY);

	if (XMLNode->Attribute("qx")) 
	{

		orientation.x = getAttribScalar(XMLNode, "qx");
		orientation.y = getAttribScalar(XMLNode, "qy");
		orientation.z = getAttribScalar(XMLNode, "qz");
		orientation.w = getAttribScalar(XMLNode, "qw");

	} 
	else if (XMLNode->Attribute("angle")) 
	{

		gkVector3 axis;

		axis.x = getAttribScalar(XMLNode, "x", 0);
		axis.y = getAttribScalar(XMLNode, "y", 1);
		axis.z = getAttribScalar(XMLNode, "z", 0);
		
		gkScalar angle = getAttribScalar(XMLNode, "angle", 0);;

		orientation.FromAngleAxis(Ogre::Angle(angle), axis);

	} 
	else if (XMLNode->Attribute("pitch") || XMLNode->Attribute("yaw") || XMLNode->Attribute("roll")) 
	{
		
		gkScalar pitch = getAttribScalar(XMLNode, "pitch");
		gkScalar yaw   = getAttribScalar(XMLNode, "yaw");
		gkScalar roll  = getAttribScalar(XMLNode, "roll");

		gkMatrix3 mat;
		mat.FromEulerAnglesXYZ(Ogre::Angle(pitch), Ogre::Angle(yaw), Ogre::Angle(roll));
		orientation.FromRotationMatrix(mat);
	}

	return orientation;
}

gkColor parseColour(const TiXmlElement* XMLNode)
{
	return gkColor(
		getAttribScalar(XMLNode, "r", 0),
		getAttribScalar(XMLNode, "g", 0),
		getAttribScalar(XMLNode, "b", 0),
		getAttribScalar(XMLNode, "a", 1));
}

//--

bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkVector3 &v, bool normalize)
{
	const TiXmlElement* pElement = XMLNode->FirstChildElement(name);
	if (!pElement) return false;
	
	v = parseVector3(pElement, normalize);
	return true;
}

bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkQuaternion &q)
{
	const TiXmlElement* pElement = XMLNode->FirstChildElement(name);
	if (!pElement) return false;
	
	q = parseQuaternion(pElement);
	return true;
}

bool getElement(const TiXmlElement* XMLNode, const gkString& name, gkColor &c)
{
	const TiXmlElement* pElement = XMLNode->FirstChildElement(name);
	if (!pElement) return false;
	
	c = parseColour(pElement);
	return true;
}

} //xml