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
#ifndef _gkConstraint_h_
#define _gkConstraint_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "gkTransformState.h"


// Constraint base class
class gkConstraint : public utListClass<gkConstraint>::Link
{
public:

    typedef enum ConstraintTypes
    {
        CONST_NONE = 0,
        CONST_LIMIT_ROT,
        CONST_LIMIT_LOC,
    }ConstraintTypes;

public:


    gkConstraint(ConstraintTypes type);
    virtual ~gkConstraint() {}

    GK_INLINE void setInfluence(gkScalar v)         {m_influence = gkClampf(v, 0.0, 1.0);}
    GK_INLINE gkScalar getInfluence(void)           {return m_influence;}
    GK_INLINE void setLocal(bool v)                 {m_isLocal = v;}
    GK_INLINE bool getLocal(void)                   {return m_isLocal;}

    GK_INLINE void setMatrix(const gkTransformState &m) {m_matrix = m;}
    GK_INLINE gkTransformState& getMatrix(void)         {return m_matrix;}

    virtual bool update(gkGameObject *ob) = 0;

protected:
    const ConstraintTypes   m_type;
    bool                    m_isLocal;
    gkScalar                m_influence;
    gkTransformState        m_matrix;
};


#endif//_gkConstraint_h_
