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

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);

    gkObjActionNode(gkLogicTree *parent, size_t id);
	virtual ~gkObjActionNode() {}
    
    bool evaluate(gkScalar tick);
};

class gkRotateNode : public gkObjActionNode
{
public:

	enum
	{
		X = MAX_SOCKETS,
		Y,
		Z
	};

	DECLARE_SOCKET_TYPE(X, gkScalar);
	DECLARE_SOCKET_TYPE(Y, gkScalar);
	DECLARE_SOCKET_TYPE(Z, gkScalar);

	gkRotateNode(gkLogicTree *parent, size_t id);

	~gkRotateNode() {}

	void update(gkScalar tick);
};

class gkLinearVelNode : public gkObjActionNode
{
public:

	enum
	{
		X = MAX_SOCKETS,
		Y,
		Z
	};

	DECLARE_SOCKET_TYPE(X, gkScalar);
	DECLARE_SOCKET_TYPE(Y, gkScalar);
	DECLARE_SOCKET_TYPE(Z, gkScalar);

	gkLinearVelNode(gkLogicTree *parent, size_t id);
	~gkLinearVelNode() {}

	void update(gkScalar tick);
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
		INPUT = MAX_SOCKETS
	};

	DECLARE_SOCKET_TYPE(INPUT, gkQuaternion);

	gkSetOrientationNode(gkLogicTree *parent, size_t id);

	~gkSetOrientationNode() {}

	void update(gkScalar tick);
};

class gkDisableDeactivationNode : public gkObjActionNode
{
public:

	gkDisableDeactivationNode(gkLogicTree *parent, size_t id)
		: gkObjActionNode(parent, id){}

	~gkDisableDeactivationNode() {}

	void update(gkScalar tick);
};

#endif//_gkObjActionNode_h_
