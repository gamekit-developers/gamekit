/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
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
#ifndef _gkMouseButtonNode_h_
#define _gkMouseButtonNode_h_


#include "gkLogicNode.h"
#include "gkInput.h"


class gkMouseButtonNode : public gkLogicNode
{
public:
    gkMouseButtonNode(gkLogicTree *parent, size_t id);
	virtual ~gkMouseButtonNode() {}

	bool evaluate(gkScalar tick);

    GK_INLINE void setButton(gkMouse::Buttons button)   {m_button= button;}
	GK_INLINE void setDelay(int d)                      {m_delay= d;}


    // inputs 
    GK_INLINE gkLogicSocket* getUpdate(void)        {return &m_sockets[0];}

    // outputs
    GK_INLINE gkLogicSocket* getIsDown(void)        {return &m_sockets[1];}
    GK_INLINE gkLogicSocket* getPress(void)         {return &m_sockets[2];}
    GK_INLINE gkLogicSocket* getRelease(void)       {return &m_sockets[3];}

private:
	gkLogicSocket m_sockets[4];
	gkMouse::Buttons m_button;
	int m_delay, m_counter;
	bool m_isPressed, m_pressed;
};


#endif//_gkMouseButtonNode_h_
