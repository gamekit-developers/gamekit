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
#ifndef _gkMultiplexerNode_h_
#define _gkMultiplexerNode_h_

#include "gkLogicNode.h"

template<typename T>
class gkMultiplexerNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		INPUT_FALSE,
		INPUT_TRUE,
		SEL,
		OUTPUT
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(INPUT_FALSE, T);
	DECLARE_SOCKET_TYPE(INPUT_TRUE, T);
	DECLARE_SOCKET_TYPE(SEL, bool);
	DECLARE_SOCKET_TYPE(OUTPUT, T);

	gkMultiplexerNode(gkLogicTree* parent, size_t id)
		: gkLogicNode(parent, id)
	{
		ADD_ISOCK(UPDATE, true);
		ADD_ISOCK(INPUT_FALSE, T());
		ADD_ISOCK(INPUT_TRUE, T());
		ADD_ISOCK(SEL, false);
		ADD_OSOCK(OUTPUT, T());
	}

	virtual ~gkMultiplexerNode() {}

	bool evaluate(gkScalar tick)
	{
		if (GET_SOCKET_VALUE(UPDATE))
		{
			if (GET_SOCKET_VALUE(SEL))
			{
				SET_SOCKET_VALUE(OUTPUT, GET_SOCKET_VALUE(INPUT_TRUE));
			}
			else
			{
				SET_SOCKET_VALUE(OUTPUT, GET_SOCKET_VALUE(INPUT_FALSE));
			}
		}

		return false;
	}
};

#endif//_gkMultiplexerNode_h_
