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
#ifndef _gkNewMotionNode_h_
#define _gkNewMotionNode_h_

#include "gkLogicNode.h"

class gkGameObject;

class gkNewMotionNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		X,
		Y,
		Z,
		TARGET,
		MAX_SOCKETS
	};

    gkNewMotionNode(gkLogicTree *parent, size_t id);
	virtual ~gkNewMotionNode() {}
    
    void update(gkScalar tick);
    bool evaluate(gkScalar tick);

    // socket access
    GK_INLINE gkLogicSocket* getUpdate()    {return &m_sockets[UPDATE];}
    GK_INLINE gkLogicSocket* getX()         {return &m_sockets[X];}
    GK_INLINE gkLogicSocket* getY()         {return &m_sockets[Y];}
    GK_INLINE gkLogicSocket* getZ()         {return &m_sockets[Z];}
	GK_INLINE gkLogicSocket* getTarget()    {return &m_sockets[TARGET];}

	virtual void DoUpdate() = 0;

protected:

	gkVector3 m_XYZ;

private:

    gkLogicSocket m_sockets[MAX_SOCKETS];
};

class gkRotateNode : public gkNewMotionNode
{
public:

	gkRotateNode(gkLogicTree *parent, size_t id)
		: gkNewMotionNode(parent, id)
	{
	}
	
	~gkRotateNode() {}

	void DoUpdate();
};

class gkLinearVelNode : public gkNewMotionNode
{
public:

	gkLinearVelNode(gkLogicTree *parent, size_t id)
		: gkNewMotionNode(parent, id)
	{
	}
	
	~gkLinearVelNode() {}

	void DoUpdate();
};



#endif//_gkNewMotionNode_h_
