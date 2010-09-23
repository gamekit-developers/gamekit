/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#ifndef _gkRayTestNode_h_
#define _gkRayTestNode_h_

#include "gkLogicNode.h"

class gkGameObject;

class gkRayTestNode : public gkLogicNode
{
public:

	enum
	{
		ENABLE,
		TARGET,
		RAY_ORIGIN_OFFSET,
		RAY_DIRECTION,
		HIT,
		NOT_HIT,
		HIT_OBJ,
		HIT_NAME,
		HIT_POSITION
	};

	DECLARE_SOCKET_TYPE(ENABLE, bool);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);
	DECLARE_SOCKET_TYPE(RAY_ORIGIN_OFFSET, gkVector3);
	DECLARE_SOCKET_TYPE(RAY_DIRECTION, gkVector3);
	DECLARE_SOCKET_TYPE(HIT, bool);
	DECLARE_SOCKET_TYPE(NOT_HIT, bool);
	DECLARE_SOCKET_TYPE(HIT_OBJ, gkGameObject*);
	DECLARE_SOCKET_TYPE(HIT_NAME, gkString);
	DECLARE_SOCKET_TYPE(HIT_POSITION, gkVector3);

	gkRayTestNode(gkLogicTree* parent, size_t id);

	virtual ~gkRayTestNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

private:

	gkGameObject* m_object;

};

class gkScreenRayTestNode : public gkLogicNode
{
public:

	enum
	{
		ENABLE,
		SCREEN_X,
		SCREEN_Y,
		HIT,
		NOT_HIT,
		HIT_OBJ,
		HIT_NAME,
		HIT_POSITION
	};

	DECLARE_SOCKET_TYPE(ENABLE, bool);
	DECLARE_SOCKET_TYPE(SCREEN_X, gkScalar);
	DECLARE_SOCKET_TYPE(SCREEN_Y, gkScalar);
	DECLARE_SOCKET_TYPE(HIT, bool);
	DECLARE_SOCKET_TYPE(NOT_HIT, bool);
	DECLARE_SOCKET_TYPE(HIT_OBJ, gkGameObject*);
	DECLARE_SOCKET_TYPE(HIT_NAME, gkString);
	DECLARE_SOCKET_TYPE(HIT_POSITION, gkVector3);

	gkScreenRayTestNode(gkLogicTree* parent, size_t id);

	virtual ~gkScreenRayTestNode() {}

	bool evaluate(gkScalar tick);

private:

	gkGameObject* m_object;
};


#endif//_gkRayTestNode_h_
