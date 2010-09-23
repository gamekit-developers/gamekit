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
#include "nsNodeTree.h"



nsNodeTree::nsNodeTree(const nsString& name)
	:   m_name(name),
	    m_isGroup(false),
	    m_open(false),
	    m_groupName(),
	    m_projection(0, 0, 0, 0),
	    m_client(0),
	    m_size(0, 0),
	    m_attachedObject("")
{
}



nsNodeTree::~nsNodeTree()
{
	clear();
}



void nsNodeTree::deleteNode(nsNode* node)
{
	if (node && m_nodes.find(node) != 0)
	{
		node->unlink();
		m_nodes.erase(node);
		delete node;
	}
}



nsNode* nsNodeTree::createNode(nsNodeDef* nt)
{
	nsNode* nd = new nsNode(this, nt);
	m_nodes.push_back(nd);
	return nd;
}



nsNode* nsNodeTree::createCloneNode(nsNode* nd)
{
	nsNode* nnd = nd->clone(this);
	m_nodes.push_back(nnd);
	return nnd;
}


void nsNodeTree::bringToFront(nsNodes& list, nsNode* node)
{
	// make node the first to be rendered / hit tested
	if (list.empty())
		return;

	if ( node != 0 )
	{
		if (list.back() == node)
			return;

		nsNodes::Pointer found = list.find( node );
		if ( found != 0 )
		{
			list.erase( found );
			list.push_back( node );
		}
	}
}



void nsNodeTree::bringToFront(nsNode* node)
{
	bringToFront(m_nodes, node);
}


void nsNodeTree::clear(void)
{
	if (!m_nodes.empty())
	{
		nsNodeIterator it = getNodeIterator();
		while (it.hasMoreElements())
			delete it.getNext();

		m_nodes.clear();
	}
}
