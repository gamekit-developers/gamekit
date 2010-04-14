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
#ifndef _gkSetterNode_h_
#define _gkSetterNode_h_

#include "gkLogicNode.h"

class gkSetterNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		MAX_SOCKETS
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);

    gkSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkSetterNode() {}

	bool evaluate(gkScalar tick);
};

class gkStringSetterNode : public gkSetterNode
{
public:

	enum
	{
		INPUT = MAX_SOCKETS,
		OUTPUT
	};

	DECLARE_SOCKET_TYPE(INPUT, gkString);
	DECLARE_SOCKET_TYPE(OUTPUT, gkString);

    gkStringSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkStringSetterNode() {}

	void update(gkScalar tick);
};

class gkObjectSetterNode : public gkSetterNode
{
public:

	enum
	{
		INPUT = MAX_SOCKETS,
		OUTPUT,
		X,
		Y,
		RESET,
		HAS_OBJ,
		HIT_POINT
	};

	DECLARE_SOCKET_TYPE(INPUT, gkString);
	DECLARE_SOCKET_TYPE(OUTPUT, gkGameObject*);
	DECLARE_SOCKET_TYPE(X, gkScalar);
	DECLARE_SOCKET_TYPE(Y, gkScalar);
	DECLARE_SOCKET_TYPE(RESET, bool);
	DECLARE_SOCKET_TYPE(HAS_OBJ, bool);
	DECLARE_SOCKET_TYPE(HIT_POINT, gkVector3);

	enum INPUT_TYPE
	{
		NAME,
		SCREEN_XY
	};

    gkObjectSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkObjectSetterNode() {}

	void update(gkScalar tick);

	void setType(INPUT_TYPE type) {m_type = type;}

private:

	INPUT_TYPE m_type;
};

class gkPositionSetterNode : public gkSetterNode
{
public:

	enum
	{
		INPUT = MAX_SOCKETS,
		OUTPUT
	};

	DECLARE_SOCKET_TYPE(INPUT, gkGameObject*);
	DECLARE_SOCKET_TYPE(OUTPUT, gkVector3);

    gkPositionSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkPositionSetterNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);
};

class gkOrientationSetterNode : public gkSetterNode
{
public:

	enum
	{
		INPUT = MAX_SOCKETS,
		OUTPUT
	};

	DECLARE_SOCKET_TYPE(INPUT, gkGameObject*);
	DECLARE_SOCKET_TYPE(OUTPUT, gkQuaternion);

    gkOrientationSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkOrientationSetterNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);
};


#endif//_gkSetterNode_h_

