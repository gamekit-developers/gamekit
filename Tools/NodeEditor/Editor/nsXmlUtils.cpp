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
#include "nsXmlUtils.h"


static const wxString nsXmlInvalidAttribute = wxString("Attr_Invalid");



bool     nsXmlUtils::hasAttribute(const nsString& attr, wxXmlNode* node)
{
	return node->HasAttribute(attr);
}


bool     nsXmlUtils::getAttributeBool(const nsString& attr, wxXmlNode* node, bool def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	attrWx = attrWx.MakeLower();
	return attrWx == ("true") || atoi(wxToAscii(attrWx)) != 0;
}



int      nsXmlUtils::getAttributeInt(const nsString& attr, wxXmlNode* node, int def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return atoi(wxToAscii(attrWx));
}


NSfloat  nsXmlUtils::getAttributeFloat(const nsString& attr, wxXmlNode* node, NSfloat def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return (NSfloat)atof(wxToAscii(attrWx));
}



nsString  nsXmlUtils::getAttributeString(const nsString& attr, wxXmlNode* node, const nsString& def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return nsString(attrWx);
}


NSvec2   nsXmlUtils::getAttributeVec2(const nsString& attr, wxXmlNode* node, const NSvec2& def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return nsStringConverter::toVec2(nsString(attrWx));
}


NSvec3   nsXmlUtils::getAttributeVec3(const nsString& attr, wxXmlNode* node, const NSvec3& def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return nsStringConverter::toVec3(nsString(attrWx));
}


NSvec4   nsXmlUtils::getAttributeVec4(const nsString& attr, wxXmlNode* node, const NSvec4& def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return nsStringConverter::toVec4(nsString(attrWx));
}


NSquat   nsXmlUtils::getAttributeQuat(const nsString& attr, wxXmlNode* node, const NSquat& def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return nsStringConverter::toQuat(nsString(attrWx));
}


NSrect   nsXmlUtils::getAttributeRect(const nsString& attr, wxXmlNode* node, const NSrect& def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return nsStringConverter::toRect(nsString(attrWx));
}


NScolor  nsXmlUtils::getAttributeColor(const nsString& attr, wxXmlNode* node, const NScolor& def)
{
	wxString attrWx = node->GetAttribute(attr, nsXmlInvalidAttribute);
	if (attrWx == nsXmlInvalidAttribute)
		return def;
	return nsStringConverter::toColorHex(nsString(attrWx));
}
