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
#ifndef _gkStatementNode_h_
#define _gkStatementNode_h_

#include "gkLogicNode.h"
/*
// switch constants
#define START_IN 2
#define START_OT 12
#define CASE_MAX 23

// switch & case support upto 10 labels
class gkSwitchNode : public gkLogicNode
{
public:
    gkSwitchNode(gkLogicTree *parent, size_t id);
    virtual ~gkSwitchNode() {}

    bool evaluate(gkScalar tick);
    void update(gkScalar tick);
    void initialize();

    // number of labels to test
    GK_INLINE void setNrLabels(int nr)  {m_labels = gkClamp<int>(nr, 1, 10);}

    // inputs
    GK_INLINE gkLogicSocket* getUpdate(void)        {return &m_sockets[0];}
    GK_INLINE gkLogicSocket* getSwitch(void)        {return &m_sockets[1];}
    GK_INLINE gkLogicSocket* getCase(int idx) {
        idx = gkClamp<int>(idx, 0, 9);
        return &m_sockets[START_IN + idx];
    }

    // outputs
    GK_INLINE gkLogicSocket* getDefault(void)        {return &m_sockets[START_OT];}
    GK_INLINE gkLogicSocket* getReturn(int idx) {
        idx = gkClamp<int>(idx, 0, 9);
        return &m_sockets[(START_OT + 1) + idx];
    }


private:
    gkLogicSocket   m_sockets[CASE_MAX];
    int             m_labels;
};

*/



#endif//_gkSwitchNode_h_
