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
#include "gkLogicNode.h"
#include "gkLogicTree.h"


using namespace Ogre;



gkLogicNode::gkLogicNode(gkLogicTree* parent, UTsize handle) :
	m_handle(handle), m_object(0), m_other(0), m_parent(parent),
	m_hasLinks(false), m_priority(0)
{
}

gkLogicNode::~gkLogicNode()
{
	{
		SocketIterator iter(m_inputs);

		while (iter.hasMoreElements())
		{
			gkILogicSocket* pSocket = iter.getNext();

			delete pSocket;
		}
	}

	{
		SocketIterator iter(m_outputs);

		while (iter.hasMoreElements())
		{
			gkILogicSocket* pSocket = iter.getNext();

			delete pSocket;
		}
	}
}



gkILogicSocket* gkLogicNode::getInputSocket(UTsize index)
{
	if (index >= 0 && index < m_inputs.size())
		return m_inputs.at(index);
	return 0;
}


gkILogicSocket* gkLogicNode::getOutputSocket(UTsize index)
{
	if (index >= 0 && index < m_outputs.size())
		return m_outputs.at(index);
	return 0;
}
