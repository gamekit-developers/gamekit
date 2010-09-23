/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#include "gkGroupNode.h"
#include "gkLogicTree.h"

gkGroupNode::gkGroupNode(gkLogicTree* parent, size_t id)
	: gkLogicNode(parent, id)
{
	ADD_ISOCK(EXECUTE, false);
	ADD_OSOCK(RETURN, false);
}

bool gkGroupNode::evaluate(gkScalar tick)
{
	return GET_SOCKET_VALUE(EXECUTE) && !m_groupList.empty();
}

void gkGroupNode::update(gkScalar tick)
{
	utListIterator<Groups> it(m_groupList);
	while (it.hasMoreElements())
	{
		gkLogicTree* ltree = it.getNext();
		if (ltree->getAttachedObject() != m_object)
			ltree->attachObject(m_object);

		ltree->execute(tick);
	}
	// TODO push a return from exec
	SET_SOCKET_VALUE(RETURN, true);
}
