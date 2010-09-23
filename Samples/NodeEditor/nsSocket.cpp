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
#include "nsSocket.h"
#include "nsNode.h"



nsSocket::nsSocket(nsNode* nd, nsSocketDef* def)
	:   m_type(def), m_parent(nd),
	    m_from(0)
{
	UT_ASSERT(m_parent);
	m_value = def->getValue();

	m_rect = def->getRect();
	updateFromParent();
}



nsSocket::~nsSocket()
{
	m_tosockets.clear();
	m_from = 0;
}



void nsSocket::connect(nsSocket* oth)
{
	// ignore links on the same node
	if (oth && oth->m_parent == m_parent)
		return;

	if (oth)
	{
		if (m_from == oth)
		{
			// ignore linking to the same socket
			return;
		}

		if (m_from != 0)
		{
			nsSocketList& links = m_from->m_tosockets;

			// other is zero so erase this in the from node
			UT_ASSERT((!links.empty() && links.find(this)) && "Socket improperly linked!");
			links.erase(this);

			bool isFirst = getPrev() == 0;

			nsSocket* sock = 0;

			if (isFirst)
				sock = getNext();
			else
			{
				// try top down
				sock = getPrev();
				if (sock)
				{
					nsSocket* root = sock;
					while (root)
					{
						sock = root;
						root = root->getPrev();
						if (!root)
							break;
					}
				}
			}

			while (sock)
			{
				if (sock != this && !sock->isConnected())
				{
					if (m_from->canConnect(sock))
					{
						sock->connect(m_from);
						break;
					}
				}
				sock = sock->getNext();
			}
		}

		// linking to
		UT_ASSERT(oth->isOutput() && isInput());

		m_from = oth;
		m_from->m_tosockets.push_back(this);
	}
	else
	{
		// make sure this is an input
		UT_ASSERT(isInput());


		if (m_from)
		{
			nsSocketList& links = m_from->m_tosockets;

			// other is zero so erase this in the from node
			UT_ASSERT((!links.empty() && links.find(this)) && "Socket improperly linked!");
			links.erase(this);
			m_from = 0;
		}
	}
}


void nsSocket::updateFromParent(void)
{
	if (!m_parent)
		return;
	NSrect prect = m_parent->getRect();
	m_derrivedRect = m_type->getRect();
	m_derrivedRect.x = prect.x + m_rect.x;
	m_derrivedRect.y = prect.y + m_rect.y;
}


void nsSocket::unlink(void)
{
	// remove all refs to this

	if (m_type->isOutput())
	{
		if (!m_tosockets.empty())
		{
			utListIterator<nsSocketList> it(m_tosockets);
			while (it.hasMoreElements())
			{
				nsSocket* sock = it.getNext();
				if (sock->m_from == this)
					sock->m_from = 0;
			}
		}
	}
	else
	{
		if (m_from)
		{
			nsSocketList& links = m_from->m_tosockets;
			UT_ASSERT((!links.empty() && links.find(this)) && "Socket improperly linked!");
			links.erase(this);
		}
	}

	m_tosockets.clear();
	m_from = 0;
}


nsSocketDef::nsSocketDef(nsDirection dir, const nsString& name, int id, double min, double max, const nsValue& val)
	:   m_dir(dir),
	    m_name(name),
	    m_docStr(""),
	    m_palette(),
	    m_id(id),
	    m_uid(-1),
	    m_min(min),
	    m_max(max),
	    m_default(val)
{
}



nsSocketDef::~nsSocketDef()
{
}


nsSocket* nsSocketDef::create(nsNode* parent)
{
	return new nsSocket(parent, this);
}
