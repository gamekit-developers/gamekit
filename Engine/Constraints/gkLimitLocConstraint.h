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
#ifndef _gkLimitLocConstraint_h_
#define _gkLimitLocConstraint_h_

#include "gkConstraint.h"

class gkLimitLocConstraint : public gkConstraint
{
public:

    gkLimitLocConstraint();
    virtual ~gkLimitLocConstraint() {}

    bool update(gkGameObject *ob);
    gkConstraint* clone(void);

    void setMinX(gkScalar v);
    void setMaxX(gkScalar v);

    void setMinY(gkScalar v);
    void setMaxY(gkScalar v);

    void setMinZ(gkScalar v);
    void setMaxZ(gkScalar v);

private:
    short m_flag[2];

    gkScalar x[2];
    gkScalar y[2];
    gkScalar z[2];
};



GK_INLINE void gkLimitLocConstraint::setMinX(gkScalar v)
{
    m_flag[0] |= 1;
    x[0] = v;
}


GK_INLINE void gkLimitLocConstraint::setMaxX(gkScalar v)
{
    m_flag[1] |= 1;
    x[1] = v;
}


GK_INLINE void gkLimitLocConstraint::setMinY(gkScalar v)
{
    m_flag[0] |= 2;
    y[0] = v;
}


GK_INLINE void gkLimitLocConstraint::setMaxY(gkScalar v)
{
    m_flag[1] |= 2;
    y[1] = v;
}


GK_INLINE void gkLimitLocConstraint::setMinZ(gkScalar v)
{
    m_flag[0] |= 4;
    z[0] = v;
}


GK_INLINE void gkLimitLocConstraint::setMaxZ(gkScalar v)
{
    m_flag[1] |= 4;
    z[1] = v;
}






#endif//_gkLimitLocConstraint_h_
