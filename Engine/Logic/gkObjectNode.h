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
#ifndef _gkObjectNode_h_
#define _gkObjectNode_h_

#include "gkLogicNode.h"

// This is and object channel, for passing
// and object over a socket or getting and setting
// and object's location & rotation
class gkObjectNode : public gkLogicNode
{
public:

	enum
	{
		INPUT,
		IN_POSITION,
		IN_ROTATION,
		OUTPUT,
		OUT_POSITION,
		OUT_ROTATION
	};

	DECLARE_SOCKET_TYPE(INPUT, bool);
	DECLARE_SOCKET_TYPE(IN_POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(IN_ROTATION, gkVector3);
	DECLARE_SOCKET_TYPE(OUTPUT, bool);
	DECLARE_SOCKET_TYPE(OUT_POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(OUT_ROTATION, gkVector3);

	gkObjectNode(gkLogicTree* parent, size_t id);
	virtual ~gkObjectNode() {}
	void initialize();

	void update(gkScalar tick);
	bool evaluate(gkScalar tick);

	// todo, just pass object ptr
	GK_INLINE void setOtherObject(const gkString& name)   {m_otherName = name;}
	GK_INLINE void setFunction(gkObjectFunction f)        {m_func = f;}

private:
	gkString            m_otherName;
	gkObjectFunction    m_func;
};




#endif//_gkObjectNode_h_
