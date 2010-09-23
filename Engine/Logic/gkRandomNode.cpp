/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
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
#include "gkRandomNode.h"

gkRandomNode::gkRandomNode(gkLogicTree* parent, size_t id) :
	gkLogicNode(parent, id)
{
	ADD_ISOCK(UPDATE, true);
	ADD_ISOCK(MIN, 0);
	ADD_ISOCK(MAX, 0);
	ADD_OSOCK(RESULT, 0);
}

bool gkRandomNode::evaluate(gkScalar tick)
{
	return GET_SOCKET_VALUE(UPDATE);
}

void gkRandomNode::update(gkScalar tick)
{
	SET_SOCKET_VALUE(RESULT, gkMath::RangeRandom(GET_SOCKET_VALUE(MIN), GET_SOCKET_VALUE(MAX)));
}
