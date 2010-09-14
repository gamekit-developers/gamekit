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
#include "nsNode.h"
#include "nsNodeTree.h"




nsNode::nsNode(nsNodeTree* tree, nsNodeDef* def)
	:   m_type(def),
	    m_tree(tree),
	    m_state(NDST_INACTIVE),
	    m_attachedObject(""),
	    m_data(0)
{

	m_rect.setPosition(0, 0);
	m_rect.setSize(def->getSize());

	// create inputs from definition types
	nsSocketDefIterator in = def->getInputIterator();
	while (in.hasMoreElements())
		m_inputs.push_back(in.getNext()->create(this));


	// create outputs from definition types
	nsSocketDefIterator out = def->getOutputIterator();
	while (out.hasMoreElements())
		m_outputs.push_back(out.getNext()->create(this));

	m_data = def->getData();
}



nsNode::nsNode(nsNode* cpy, nsNodeTree* tree)
	:   m_type(0), m_tree(tree), m_state(NDST_INACTIVE)
{
	UT_ASSERT(cpy);

	m_type          = cpy->m_type;
	m_rect          = cpy->m_rect;
	m_attachedObject = cpy->m_attachedObject;
	m_data          = cpy->getData()->clone();

	// clone sockets
	nsSocket* sock;
	if (!cpy->m_inputs.empty())
	{
		sock = cpy->m_inputs.begin();
		while (sock)
		{
			m_inputs.push_back(sock->clone(this));
			sock = sock->getNext();
		}
	}

	if (!cpy->m_outputs.empty())
	{
		sock = cpy->m_outputs.begin();
		while (sock)
		{
			m_outputs.push_back(sock->clone(this));
			sock = sock->getNext();
		}
	}
}



nsNode::~nsNode()
{
	nsSocket* sock, *tsock;

	delete m_data;


	if (!m_inputs.empty())
	{
		sock = m_inputs.begin();
		while (sock)
		{
			tsock = sock;
			sock = sock->getNext();
			delete tsock;
		}
		m_inputs.clear();
	}

	if (!m_outputs.empty())
	{
		sock = m_outputs.begin();
		while (sock)
		{
			tsock = sock;
			sock = sock->getNext();
			delete tsock;
		}

		m_outputs.clear();
	}
}



void nsNode::unlink(void)
{
	// remove all refs to this
	nsSocket* sock;
	if (!m_inputs.empty())
	{
		sock = m_inputs.begin();
		while (sock)
		{
			sock->unlink();
			sock = sock->getNext();
		}
	}

	if (!m_outputs.empty())
	{
		sock = m_outputs.begin();
		while (sock)
		{
			sock->unlink();
			sock = sock->getNext();
		}
	}
}



nsNode* nsNode::clone(nsNodeTree* newTree)
{
	return new nsNode(this, newTree);
}


void nsNode::translate(NSfloat x, NSfloat y)
{
	setPosition(m_rect.x + x, m_rect.y + y);
}


void nsNode::setPosition(NSfloat x, NSfloat y)
{
	m_rect.x = x;
	m_rect.y = y;

	nsSocket* sock;
	if (!m_inputs.empty())
	{
		sock = m_inputs.begin();
		while (sock)
		{
			sock->updateFromParent();
			sock = sock->getNext();
		}
	}

	if (!m_outputs.empty())
	{
		sock = m_outputs.begin();
		while (sock)
		{
			sock->updateFromParent();
			sock = sock->getNext();
		}
	}
}





nsNodeDef::nsNodeDef()
	:   m_type(NT_NULL),
	    m_group(NT_GROUP_MAX),
	    m_name(""),
	    m_palette(),
	    m_inputs(),
	    m_outputs(),
	    m_size(nsNodeHeaderSize, nsNodeHeaderSize)
{
}


nsNodeDef::nsNodeDef(nsNodeTypes type, nsGroupTypes group, const nsString& name, const nsString& docs)
	:   m_type(type),
	    m_group(group),
	    m_name(name),
	    m_docStr(docs),
	    m_palette(),
	    m_inputs(),
	    m_outputs(),
	    m_size(nsNodeHeaderSize, nsNodeHeaderSize)
{
	m_size.x = NS_MAKE_NODE_DEF_WIDTH(name.size());
}



nsNodeDef::~nsNodeDef()
{
	if (!m_inputs.empty())
	{
		nsSocketDef* def = m_inputs.begin(), *tmp;
		while (def)
		{
			tmp = def;
			def = def->getNext();
			delete tmp;
		}
	}
	if (!m_outputs.empty())
	{
		nsSocketDef* def = m_outputs.begin(), *tmp;
		while (def)
		{
			tmp = def;
			def = def->getNext();
			delete tmp;
		}
	}
}


void nsNodeDef::addInput(nsSocketDef* def)
{
	def->setUid(m_inputs.size());
	m_inputs.push_back(def);
	def->setRect(NSrect(-nsDefaultSocketSize, m_size.y, nsDefaultSocketSize, nsDefaultSocketSize));
	m_size.y += nsDefaultSocketOffset;
}


void nsNodeDef::addOutput(nsSocketDef* def)
{
	def->setUid(m_outputs.size());
	m_outputs.push_back(def);
	def->setRect(NSrect(m_size.x, m_size.y, nsDefaultSocketSize, nsDefaultSocketSize));
	m_size.y += nsDefaultSocketOffset;
}



void nsNodeDef::alignInputs(nsSocketAlignment align)
{
	if (align == NS_SAL_BOTTOM)
	{
		if (!m_inputs.empty())
		{
			NSfloat ypos = m_size.y;
			nsSocketDef* def = m_inputs.end();
			while (def)
			{
				ypos -= nsDefaultSocketOffset;
				def->setRect(NSrect(-nsDefaultSocketSize, ypos, nsDefaultSocketSize, nsDefaultSocketSize));
				def = def->getPrev();
			}
		}
	}
	else if (align == NS_SAL_TOP)
	{
		if (!m_inputs.empty())
		{
			NSfloat ypos = nsNodeHeaderSize + 1;
			nsSocketDef* def = m_inputs.begin();
			while (def)
			{
				def->setRect(NSrect(-nsDefaultSocketSize, ypos, nsDefaultSocketSize, nsDefaultSocketSize));
				def = def->getNext();
				ypos += nsDefaultSocketOffset;
			}
		}
	}
}


void nsNodeDef::alignOutputs(nsSocketAlignment align)
{
	if (align == NS_SAL_TOP)
	{
		if (!m_outputs.empty())
		{
			NSfloat ypos = nsNodeHeaderSize + 1;
			nsSocketDef* def = m_outputs.begin();
			while (def)
			{
				def->setRect(NSrect(m_size.x, ypos, nsDefaultSocketSize, nsDefaultSocketSize));
				def = def->getNext();
				ypos += nsDefaultSocketOffset;
			}
		}
	}
	else if (align == NS_SAL_BOTTOM)
	{
		if (!m_outputs.empty())
		{
			NSfloat ypos = m_size.y;
			nsSocketDef* def = m_outputs.end();
			while (def)
			{
				ypos -= nsDefaultSocketOffset;
				def->setRect(NSrect(m_size.x, ypos, nsDefaultSocketSize, nsDefaultSocketSize));
				def = def->getPrev();
			}
		}
	}
}
