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
#ifndef _gkObjActionNode_h_
#define _gkObjActionNode_h_

#include "gkLogicNode.h"

class gkGameObject;

class gkObjActionNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		TARGET,
		MAX_SOCKETS
	};

    gkObjActionNode(gkLogicTree *parent, size_t id);
	virtual ~gkObjActionNode() {}
    
    bool evaluate(gkScalar tick);

    // socket access
    GK_INLINE gkLogicSocket* getUpdate()    {return &m_sockets[UPDATE];}
	GK_INLINE gkLogicSocket* getTarget()    {return &m_sockets[TARGET];}

private:

    gkLogicSocket m_sockets[MAX_SOCKETS];
};

class gkRotateNode : public gkObjActionNode
{
public:

	enum
	{
		X,
		Y,
		Z,
		MAX_SOCKETS
	};

	gkRotateNode(gkLogicTree *parent, size_t id);

	~gkRotateNode() {}

	void update(gkScalar tick);

    GK_INLINE gkLogicSocket* getX()         {return &m_sockets[X];}
    GK_INLINE gkLogicSocket* getY()         {return &m_sockets[Y];}
    GK_INLINE gkLogicSocket* getZ()         {return &m_sockets[Z];}

private:

    gkLogicSocket m_sockets[MAX_SOCKETS];

};

class gkLinearVelNode : public gkObjActionNode
{
public:

	enum
	{
		X,
		Y,
		Z,
		MAX_SOCKETS
	};


	gkLinearVelNode(gkLogicTree *parent, size_t id);
	~gkLinearVelNode() {}

	void update(gkScalar tick);

    GK_INLINE gkLogicSocket* getX()         {return &m_sockets[X];}
    GK_INLINE gkLogicSocket* getY()         {return &m_sockets[Y];}
    GK_INLINE gkLogicSocket* getZ()         {return &m_sockets[Z];}

private:

    gkLogicSocket m_sockets[MAX_SOCKETS];

};

class gkLoadNode : public gkObjActionNode
{
public:

	gkLoadNode(gkLogicTree *parent, size_t id)
		: gkObjActionNode(parent, id){}

	~gkLoadNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);
};

class gkUnloadNode : public gkObjActionNode
{
public:

	gkUnloadNode(gkLogicTree *parent, size_t id)
		: gkObjActionNode(parent, id){}

	~gkUnloadNode() {}

	void update(gkScalar tick);
};

class gkReloadNode : public gkObjActionNode
{
public:

	gkReloadNode(gkLogicTree *parent, size_t id)
		: gkObjActionNode(parent, id){}

	~gkReloadNode() {}

	void update(gkScalar tick);
};

class gkSetOrientationNode : public gkObjActionNode
{
public:

	enum
	{
		INPUT,
		MAX_SOCKETS
	};

	gkSetOrientationNode(gkLogicTree *parent, size_t id);

	~gkSetOrientationNode() {}

	void update(gkScalar tick);

    GK_INLINE gkLogicSocket* getInput()	{return &m_sockets[INPUT];}

private:

    gkLogicSocket m_sockets[MAX_SOCKETS];

};



#endif//_gkObjActionNode_h_
