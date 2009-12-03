/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkPropertyNode.h"
#include "gkPropertyAttribute.h"


using namespace Ogre;



// ----------------------------------------------------------------------------
gkPropertyNode::gkPropertyNode() :
		m_name(), m_type(), m_base(), m_parent(0)
{
	m_debug.line= 0;
	m_debug.source= "";
}

// ----------------------------------------------------------------------------
gkPropertyNode::~gkPropertyNode()
{
}

// ----------------------------------------------------------------------------
void gkPropertyNode::addChild(gkPropertyNode *strc)
{
	if (!strc)
		return;

	m_children.push_back(strc);
	strc->setParent(this);
}

// ----------------------------------------------------------------------------
void gkPropertyNode::addAttribute(gkPropertyAttribute *attr)
{
	if (!attr)
		return;

	m_attributes.push_back(attr);
	m_attrMap.insert(attr->getName(), attr);
}

// ----------------------------------------------------------------------------
gkPropertyAttribute* gkPropertyNode::getAttribute(const gkHashedString& name)
{
	size_t pos= m_attrMap.find(name);
	if (pos != GK_NPOS)
		return m_attrMap.at(pos);
	return 0;
}
