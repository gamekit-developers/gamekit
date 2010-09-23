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
#ifndef _nsXmlUtils_h_
#define _nsXmlUtils_h_

#include <wx/xml/xml.h>
#include "nsString.h"
#include "nsVariable.h"
#include "nsMath.h"



// nsilities for parsing node attributes
class nsXmlUtils
{
public:


	static bool     hasAttribute(const nsString& attr, wxXmlNode* node);
	static bool     getAttributeBool(const nsString& attr, wxXmlNode* node, bool def = false);
	static int      getAttributeInt(const nsString& attr, wxXmlNode* node, int def = 0);
	static NSfloat  getAttributeFloat(const nsString& attr, wxXmlNode* node, NSfloat def = 0);
	static nsString getAttributeString(const nsString& attr, wxXmlNode* node, const nsString& def = "");
	static NSvec2   getAttributeVec2(const nsString& attr, wxXmlNode* node, const NSvec2& def = NSvec2());
	static NSvec3   getAttributeVec3(const nsString& attr, wxXmlNode* node, const NSvec3& def = NSvec3());
	static NSvec4   getAttributeVec4(const nsString& attr, wxXmlNode* node, const NSvec4& def = NSvec4());
	static NSquat   getAttributeQuat(const nsString& attr, wxXmlNode* node, const NSquat& def = NSquat());
	static NSrect   getAttributeRect(const nsString& attr, wxXmlNode* node, const NSrect& def = NSrect());
	static NScolor  getAttributeColor(const nsString& attr, wxXmlNode* node, const NScolor& def = NScolor());
};

#endif//_nsXmlUtils_h_
