/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkTimerNode_h_
#define _gkTimerNode_h_

#include "gkLogicNode.h"

class gkTimerNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		VALUE,
		RESULT
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(VALUE, gkScalar);
	DECLARE_SOCKET_TYPE(RESULT, gkScalar);

	gkTimerNode(gkLogicTree* parent, size_t id);
	virtual ~gkTimerNode() {}

	void update(gkScalar tick);
	bool evaluate(gkScalar tick);
};

#endif//_gkTimerNode_h_
