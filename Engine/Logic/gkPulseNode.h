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
#ifndef _gkPulseNode_h_
#define _gkPulseNode_h_

#include "gkLogicNode.h"

class gkPulseNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		OUTPUT
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(OUTPUT, bool);

	gkPulseNode(gkLogicTree* parent, size_t id);

	virtual ~gkPulseNode() {}

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);
};

#endif//_gkPulseNode_h_
