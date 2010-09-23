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
#include "nsNodeTypeInfo.h"
#include "nsNodes.h"




NS_IMPLEMENT_SINGLETON(nsNodeTypeInfo);




nsNodeTypeInfo::nsNodeTypeInfo()
{

}


nsNodeTypeInfo::~nsNodeTypeInfo()
{
	if (!m_types.empty())
	{
		nsNodeDef* def = m_types.begin(), *tmp;
		while (def)
		{
			tmp = def;
			def = def->getNext();
			delete tmp;
		}
	}
}



nsNodeDef* nsNodeTypeInfo::findTypeInfo(int i)
{
	if (!m_types.empty())
	{
		nsNodeDef* def = m_types.begin();
		while (def)
		{
			if (def->getId() == i)
				return def;

			def = def->getNext();
		}
	}
	return 0;
}


nsNodeDef* nsNodeTypeInfo::findTypeInfo(const nsString& name)
{
	if (!m_types.empty())
	{
		nsNodeDef* def = m_types.begin();
		while (def)
		{
			if (def->getName() == name)
				return def;

			def = def->getNext();
		}
	}
	return 0;
}


nsString nsNodeTypeInfo::getGroupName(int in)
{
	switch (in)
	{
	case NT_GROUP_ANIMATION:
		return "Animation";
	case NT_GROUP_USER_INPUT:
		return "Input";
	case NS_SPE_OUTPUT:
		return "Output";
	case NT_GROUP_CONVERTER:
		return "Converter";
	default:
		break;
	}

	return "General";
}



void nsNodeTypeInfo::addType(nsNodeDef* ndef)
{
	ndef->setId(m_types.size());
	m_types.push_back(ndef);

	nsGroupTypes type = ndef->getGroup();
	if (m_groups.find(type) == UT_NPOS)
		m_groups.insert(type, GroupList());
	if (m_groups.find(type) != UT_NPOS)
		m_groups.get(type)->push_back(ndef);
}




void nsNodeTypeInfo::registerTypes(void)
{
	// Inputs
	addType(new nsMouseButtonNode());
	addType(new nsMouseMotionNode());

	// Outputs

	addType(new nsObjectMotionNode());

	// Socket Converters

	//addType(new nsBoolToIntNode());
	//addType(new nsBoolToFloatNode());
	//addType(new nsIntToBoolNode());
	//addType(new nsIntToFloatNode());
	//addType(new nsFloatToBoolNode());
	//addType(new nsFloatToIntNode());
}
