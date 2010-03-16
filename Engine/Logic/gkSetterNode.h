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
		INPUT,
		OUTPUT,
		JUST_ONCE,
		MAX_SOCKETS
	};

    gkSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkSetterNode() {}

	bool evaluate(gkScalar tick);

	void update(gkScalar tick);

    GK_INLINE gkLogicSocket* getUpdate() {return &m_sockets[UPDATE];}
    GK_INLINE gkLogicSocket* getInput() {return &m_sockets[INPUT];}
	GK_INLINE gkLogicSocket* getOutput() {return &m_sockets[OUTPUT];}
	GK_INLINE gkLogicSocket* getJustOnce() {return &m_sockets[JUST_ONCE];}

protected:

	gkLogicSocket m_sockets[MAX_SOCKETS];
};

class gkStringSetterNode : public gkSetterNode
{
public:

    gkStringSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkStringSetterNode() {}

	void update(gkScalar tick);
};

class gkObjectSetterNode : public gkSetterNode
{
public:

	enum
	{
		XPOS,
		YPOS,
		HIT_POINT,
		MAX_SOCKETS
	};

	enum INPUT_TYPE
	{
		NAME,
		SCREEN_XY
	};

    gkObjectSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkObjectSetterNode() {}

    GK_INLINE gkLogicSocket* getX() {return &m_sockets[XPOS];}
    GK_INLINE gkLogicSocket* getY() {return &m_sockets[YPOS];}

	GK_INLINE gkLogicSocket* getHitPoint() {return &m_sockets[HIT_POINT];}

	void update(gkScalar tick);

	void setType(INPUT_TYPE type) {m_type = type;}

private:

	INPUT_TYPE m_type;

	gkLogicSocket m_sockets[MAX_SOCKETS];
};

class gkPositionSetterNode : public gkSetterNode
{
public:

    gkPositionSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkPositionSetterNode() {}

	void update(gkScalar tick);
};

class gkOrientationSetterNode : public gkSetterNode
{
public:

    gkOrientationSetterNode(gkLogicTree *parent, size_t id);

	virtual ~gkOrientationSetterNode() {}

	void update(gkScalar tick);
};


#endif//_gkSetterNode_h_
