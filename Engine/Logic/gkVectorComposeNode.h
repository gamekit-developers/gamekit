/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef GKVECTORCOMPOSENODE_H
#define GKVECTORCOMPOSENODE_H

#include "gkLogicNode.h"

class gkVectorComposeNode : public gkLogicNode
{
public:

	enum
	{
		X,
		Y,
		Z,
		VEC
	};

	DECLARE_SOCKET_TYPE(X, gkScalar);
	DECLARE_SOCKET_TYPE(Y, gkScalar);
	DECLARE_SOCKET_TYPE(Z, gkScalar);
	DECLARE_SOCKET_TYPE(VEC, gkVector3);

	gkVectorComposeNode(gkLogicTree* parent, size_t id)
		: gkLogicNode(parent, id)
	{
		ADD_ISOCK(X, 0);
		ADD_ISOCK(Y, 0);
		ADD_ISOCK(Z, 0);
		ADD_OSOCK(VEC, gkVector3::ZERO);
	}

	virtual ~gkVectorComposeNode() {}

	void update(gkScalar tick)
	{
		gkVector3 out = gkVector3(GET_SOCKET_VALUE(X), GET_SOCKET_VALUE(Y), GET_SOCKET_VALUE(Z));

		SET_SOCKET_VALUE(VEC, out);
	}

};

#endif // GKVECTORCOMPOSENODE_H
