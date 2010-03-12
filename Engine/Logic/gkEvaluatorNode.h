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
#ifndef _gkEvaluatorNode_h_
#define _gkEvaluatorNode_h_

#include "gkLogicNode.h"

class gkEvaluatorNode : public gkLogicNode
{
public:

	enum
	{
		A,
		B,
		OUTPUT,
		MAX_SOCKETS
	};

    gkEvaluatorNode(gkLogicTree *parent, size_t id);

	virtual ~gkEvaluatorNode() {}

	virtual bool evaluate(gkScalar tick) = 0;

	virtual void update(gkScalar tick) = 0;

    GK_INLINE gkLogicSocket* getA() {return &m_sockets[A];}
    GK_INLINE gkLogicSocket* getB() {return &m_sockets[B];}
	GK_INLINE gkLogicSocket* getOutput() {return &m_sockets[OUTPUT];}

protected:

	gkLogicSocket m_sockets[MAX_SOCKETS];
};

class gkStringEqualNode : public gkEvaluatorNode
{
public:

    gkStringEqualNode(gkLogicTree *parent, size_t id);

	~gkStringEqualNode() {}

	bool evaluate(gkScalar tick);

	void update(gkScalar tick);
};

#endif//_gkEvaluatorNode_h_
