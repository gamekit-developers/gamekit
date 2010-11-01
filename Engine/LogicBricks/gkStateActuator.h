/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gkStateActuator_h_
#define _gkStateActuator_h_

#include "gkLogicActuator.h"


class gkStateActuator : public gkLogicActuator
{
public:
	enum Op
	{
		OP_NILL,
		OP_ADD,
		OP_SUB,
		OP_CPY,
		OP_INV,
	};
protected:

	int m_stateMask, m_op;

public:

	gkStateActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkStateActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);


	void execute(void);

	GK_INLINE void setMask(int v)       {m_stateMask = v;}
	GK_INLINE void setOp(int op)        {m_op = op;}

	GK_INLINE int  getMask(void)        const {return m_stateMask;}
	GK_INLINE int  getOp(void)          const {return m_op;}
};


#endif//_gkStateActuator_h_
