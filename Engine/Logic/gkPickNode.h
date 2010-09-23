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
#ifndef _gkPickNode_h_
#define _gkPickNode_h_

#include "gkLogicNode.h"
#include "LinearMath/btVector3.h"

class gkScene;
class btPoint2PointConstraint;
class gkRigidBody;

class gkPickNode : public gkLogicNode
{
public:
	enum
	{
		UPDATE,
		CREATE_PICK,
		RELEASE_PICK,
		X,
		Y,
		DISABLE_ROTATION,
		PICKED_OBJ,
		CAUGHT_TRUE,
		CAUGHT_FALSE,
		MAX_SOCKETS
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(CREATE_PICK, bool);
	DECLARE_SOCKET_TYPE(RELEASE_PICK, bool);
	DECLARE_SOCKET_TYPE(X, gkScalar);
	DECLARE_SOCKET_TYPE(Y, gkScalar);
	DECLARE_SOCKET_TYPE(DISABLE_ROTATION, bool);
	DECLARE_SOCKET_TYPE(PICKED_OBJ, gkGameObject*);
	DECLARE_SOCKET_TYPE(CAUGHT_TRUE, bool);
	DECLARE_SOCKET_TYPE(CAUGHT_FALSE, bool);

	gkPickNode(gkLogicTree* parent, size_t id);

	~gkPickNode();

	void initialize();
	void update(Ogre::Real tick);
	bool evaluate(Ogre::Real tick);

protected:

	void CreatePick();
	void ReleasePick();
	void UpdatePick();

	virtual Ogre::Ray GetRay();
	virtual gkVector3 GetPivotPosition();

protected:

	gkRigidBody* m_pickedBody;

private:

	gkScene* m_scene;

	btPoint2PointConstraint* m_constraint;
	btVector3 m_oldPickingPos;
	Ogre::Real m_oldPickingDist;

	gkVector3 m_angularFactor;
	int m_activationState;


};

#endif//_gkPickNode_h_
