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

    GK_INLINE gkLogicSocket<bool>* getUpdate() {return GET_SOCKET(UPDATE);}
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

    GK_INLINE gkLogicSocket<gkString>* getInput() {return GET_SOCKET(INPUT);}
	GK_INLINE gkLogicSocket<gkString>* getOutput() {return GET_SOCKET(OUTPUT);}

};

class gkObjectSetterNode : public gkSetterNode
{
public:

	enum
	{
		INPUT = MAX_SOCKETS,
		OUTPUT,
		XPOS,
		YPOS,
		HIT_POINT
	};

	DECLARE_SOCKET_TYPE(INPUT, gkString);
	DECLARE_SOCKET_TYPE(OUTPUT, gkGameObject*);
	DECLARE_SOCKET_TYPE(XPOS, gkScalar);
	DECLARE_SOCKET_TYPE(YPOS, gkScalar);
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

    GK_INLINE gkLogicSocket<gkString>* getInput() {return GET_SOCKET(INPUT);}
	GK_INLINE gkLogicSocket<gkGameObject*>* getOutput() {return GET_SOCKET(OUTPUT);}
    GK_INLINE gkLogicSocket<gkScalar>* getX() {return GET_SOCKET(XPOS);}
    GK_INLINE gkLogicSocket<gkScalar>* getY() {return GET_SOCKET(YPOS);}
	GK_INLINE gkLogicSocket<gkVector3>* getHitPoint() {return GET_SOCKET(HIT_POINT);}

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

	void update(gkScalar tick);

    GK_INLINE gkLogicSocket<gkGameObject*>* getInput() {return GET_SOCKET(INPUT);}
	GK_INLINE gkLogicSocket<gkVector3>* getOutput() {return GET_SOCKET(OUTPUT);}
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

	void update(gkScalar tick);

    GK_INLINE gkLogicSocket<gkGameObject*>* getInput() {return GET_SOCKET(INPUT);}
	GK_INLINE gkLogicSocket<gkQuaternion>* getOutput() {return GET_SOCKET(OUTPUT);}
};


#endif//_gkSetterNode_h_

