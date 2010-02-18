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
#ifndef _gkMouseNode_h_
#define _gkMouseNode_h_

#include "gkLogicNode.h"
#include "gkInput.h"


class gkMouseNode : public gkLogicNode
{

public:
    gkMouseNode(gkLogicTree *parent, size_t id);
    virtual ~gkMouseNode() {}

    bool evaluate(gkScalar tick);

    // inputs 
    GK_INLINE gkLogicSocket* getScaleX(void)        {return &m_sockets[0];}
    GK_INLINE gkLogicSocket* getScaleY(void)        {return &m_sockets[1];}

    // outputs
    GK_INLINE gkLogicSocket* getMotion(void)        {return &m_sockets[2];}
    GK_INLINE gkLogicSocket* getRelX(void)          {return &m_sockets[3];}
    GK_INLINE gkLogicSocket* getRelY(void)          {return &m_sockets[4];}
    GK_INLINE gkLogicSocket* getAbsX(void)          {return &m_sockets[5];}
    GK_INLINE gkLogicSocket* getAbsY(void)          {return &m_sockets[6];}
    GK_INLINE gkLogicSocket* getWheel(void)         {return &m_sockets[7];}

private:
    gkLogicSocket m_sockets[8];
};




#endif//_gkMouseNode_h_
