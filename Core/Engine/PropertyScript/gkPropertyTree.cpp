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
#include "gkPropertyTree.h"
#include "gkPropertyAttribute.h"
#include "gkPropertyNode.h"


using namespace Ogre;




// ----------------------------------------------------------------------------
gkPropertyTree::gkPropertyTree(const String& fname) :
		m_file(fname)
{
}

// ----------------------------------------------------------------------------
gkPropertyTree::~gkPropertyTree()
{
	gkAttributeIterator it= gkAttributeIterator(m_attributes);
	while (it.hasMoreElements())
		delete it.getNext();


	gkNodeIterator ni= gkNodeIterator(m_nodes);
	while (ni.hasMoreElements())
		delete ni.getNext();
}


// ----------------------------------------------------------------------------
gkPropertyNode *gkPropertyTree::findNode(const String& name)
{
	gkNodeIterator it= gkNodeIterator(m_nodes);
	while (it.hasMoreElements())
	{
		gkPropertyNode *node= it.getNext();
		if (node->getName() == name)
			return node;
	}
	return 0;
}

// ----------------------------------------------------------------------------
gkPropertyNode *gkPropertyTree::findFirstType(const String& type)
{
	gkNodeIterator it= gkNodeIterator(m_nodes);
	while (it.hasMoreElements())
	{
		gkPropertyNode *node= it.getNext();
		if (node->getType() == type)
			return node;
	}
	return 0;
}

// ----------------------------------------------------------------------------
void gkPropertyTree::addNode(gkPropertyNode *nd)
{
	m_nodes.push_back(nd);
}

// ----------------------------------------------------------------------------
void gkPropertyTree::addAttribute(gkPropertyAttribute *at)
{
	m_attributes.push_back(at);
}
