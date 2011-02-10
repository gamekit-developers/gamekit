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
#include "gkWindowSystem.h"
#include "gkMouseNode.h"
#include "gkLogicSocket.h"
#include "gkEngine.h"
#include "gkLogger.h"

gkMouseNode::gkMouseNode(gkLogicTree* parent, size_t id) :
	gkLogicNode(parent, id)
{
	ADD_ISOCK(SCALE_X, 1);
	ADD_ISOCK(SCALE_Y, 1);
	ADD_OSOCK(MOTION, false);
	ADD_OSOCK(REL_X, 0);
	ADD_OSOCK(REL_Y, 0);
	ADD_OSOCK(ABS_X, 0);
	ADD_OSOCK(ABS_Y, 0);
	ADD_OSOCK(WHEEL, 0);
	ADD_OSOCK(WHEEL_MOTION, false);

}

bool gkMouseNode::evaluate(gkScalar tick)
{
	gkMouse* dev = gkWindowSystem::getSingleton().getMouse();

	gkScalar x = 0, y = 0;
	gkScalar x_scale = GET_SOCKET_VALUE(SCALE_X);
	gkScalar y_scale = GET_SOCKET_VALUE(SCALE_Y);

	if (dev->moved)
	{
		if (GET_SOCKET(REL_X)->isConnected()) x = dev->relative.x * x_scale;
		if (GET_SOCKET(REL_Y)->isConnected()) y = dev->relative.y * y_scale;
	}

	if (GET_SOCKET(ABS_X)->isConnected()) SET_SOCKET_VALUE(ABS_X, dev->position.x * x_scale);
	if (GET_SOCKET(ABS_Y)->isConnected()) SET_SOCKET_VALUE(ABS_Y, dev->position.y * y_scale);

	SET_SOCKET_VALUE(REL_X, x);
	SET_SOCKET_VALUE(REL_Y, y);
	SET_SOCKET_VALUE(MOTION, dev->moved);
	SET_SOCKET_VALUE(WHEEL, dev->wheelDelta);
	SET_SOCKET_VALUE(WHEEL_MOTION, dev->wheelDelta ? true : false);

	return false;
}
