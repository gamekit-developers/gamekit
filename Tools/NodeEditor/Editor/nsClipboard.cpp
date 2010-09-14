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
#include "nsClipboard.h"


typedef utHashTable<utPointerHashKey, nsNode*> nsOldNewMap;



nsNode* nsClipboard_findNode(nsOldNewMap& map, nsNode* find)
{
	// lookup nsility
	UTsize pos = map.find(find);
	if (pos != UT_NPOS)
		return map.at(pos);
	return 0;
}



void nsClipboard_link(nsOldNewMap& lookup, nsNodes& list)
{

	nsSocket* sock, *link;
	nsNode* linkPar;

	// link copied sockets
	nsNodeIterator it = nsNodeIterator(list);
	while (it.hasMoreElements())
	{
		nsNode* orig = it.getNext();
		nsNode* next = nsClipboard_findNode(lookup, orig);

		if (!next) continue;


		// do link
		sock = orig->getFirstInput();
		while (sock)
		{
			if (sock->isConnected())
			{
				link = sock->getSocketLink();

				UT_ASSERT(link);
				linkPar = link->getParent();

				// from node -> parent
				nsNode* newPar = nsClipboard_findNode(lookup, linkPar);

				if (newPar)
				{
					// first
					nsSocket* thisInp = next->getInput(sock->getType()->getUid());
					nsSocket* thisOut = newPar->getOutput(link->getType()->getUid());

					if (thisInp && thisOut)
						thisInp->connect(thisOut);
				}
			}
			sock = sock->getNext();
		}
	}
}



nsClipboard::nsClipboard()
{
}


nsClipboard::~nsClipboard()
{
	clear();
}


void nsClipboard::clear(void)
{
	if (!m_clip.empty())
	{
		// free contents
		nsNodeIterator it(m_clip);
		while (it.hasMoreElements())
			delete it.getNext();
		m_clip.clear();
	}
}


void nsClipboard::paste(nsNodeTree* tree, nsNodes& list, const NSvec2& position)
{
	// paste into list from parent tree
	if (m_clip.empty() || !tree)
		return;

	UTsize size = m_clip.size();

	nsOldNewMap lookup;
	nsNodes links;

	nsNodeIterator it(m_clip);
	while (it.hasMoreElements())
	{
		nsNode* orig = it.getNext();
		nsNode* next = tree->createCloneNode(orig);
		lookup.insert(orig, next);
		links.push_back(orig);


		NSvec2 npos = next->getRect().getPosition();
		next->setPosition(position.x, position.y);

		// maintain proportion
		if (size > 1)
			next->translate(npos.x, npos.y);

		list.push_back(next);
	}

	// re-iterate & relink
	nsClipboard_link(lookup, links);
}


void nsClipboard::copy(nsNodes& list)
{
	if (list.empty())
		return;

	// clear previous contents
	clear();

	nsOldNewMap lookup;

	// build lookup of all items
	nsNodeIterator it(list);
	while (it.hasMoreElements())
	{
		nsNode* orig = it.getNext();
		nsNode* next = orig->clone();
		lookup.insert(orig, next);

		m_clip.push_back(next);
	}

	// re-iterate & relink
	nsClipboard_link(lookup, list);
}


void nsClipboard::duplicate(nsNodeTree* tree, nsNodes& list, nsNodes& dest)
{
	if (list.empty())
		return;

	nsOldNewMap lookup;

	// build lookup of all items
	nsNodeIterator it(list);
	while (it.hasMoreElements())
	{
		nsNode* orig = it.getNext();
		nsNode* next = tree->createCloneNode(orig);
		lookup.insert(orig, next);

		dest.push_back(next);
	}

	// re-iterate & relink
	nsClipboard_link(lookup, list);
}
