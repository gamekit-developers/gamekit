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
#ifndef _gkGrabNode_h_
#define _gkGrabNode_h_

#include "gkPickNode.h"

class gkGameObject;

class gkGrabNode : public gkPickNode
{
public:

	enum
	{
		THROW_OBJECT = MAX_SOCKETS,
		TARGET,
		GRAB_DIRECTION,
		THROW_VEL,
		RELATED_OFFSET_POSITION,
		THROWED
	};

	DECLARE_SOCKET_TYPE(THROW_OBJECT, bool);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);
	DECLARE_SOCKET_TYPE(GRAB_DIRECTION, gkVector3);
	DECLARE_SOCKET_TYPE(THROW_VEL, gkVector3);
	DECLARE_SOCKET_TYPE(RELATED_OFFSET_POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(THROWED, bool);

	gkGrabNode(gkLogicTree* parent, size_t id);

	~gkGrabNode();

	void update(gkScalar tick);
	bool evaluate(gkScalar tick);

private:

	void ThrowObject();
	Ogre::Ray GetRay();
	gkVector3 GetPivotPosition();

private:

	gkGameObject* m_target;
};

#endif//_gkGrabNode_h_
