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
#ifndef _gkVisibilityActuator_h_
#define _gkVisibilityActuator_h_

#include "gkLogicActuator.h"




class gkVisibilityActuator : public gkLogicActuator
{
public:

	enum VisFlag
	{
		VA_INVIS_FLAG  = (1 << 0),
		VA_OCCLUDER    = (1 << 1),
		VA_CHILDREN    = (1 << 2),
	};

private:
	int m_flag;

public:

	gkVisibilityActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkVisibilityActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	GK_INLINE void  setFlag(int v)       {m_flag = v;}
	GK_INLINE int   getFlag(void)  const {return m_flag;}

	void execute(void);
};


#endif//_gkVisibilityActuator_h_
