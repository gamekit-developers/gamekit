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
#ifndef _gkPrintNode_h_
#define _gkPrintNode_h_

#include "gkLogicNode.h"

// print incomming socket information to
// a specific stream, right now it's stdio
class gkPrintNode : public gkLogicNode
{
public:

	enum
	{
		PRINT,
		VALUE
	};

	DECLARE_SOCKET_TYPE(PRINT, bool);
	DECLARE_SOCKET_TYPE(VALUE, gkString);

	gkPrintNode(gkLogicTree* parent, size_t id);
	virtual ~gkPrintNode() {}

	void update(gkScalar tick);
	bool evaluate(gkScalar tick);
};




#endif//_gkPrintNode_h_
