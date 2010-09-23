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
#ifndef _gkLimitRotConstraint_h_
#define _gkLimitRotConstraint_h_

#include "gkConstraint.h"

class gkLimitRotConstraint : public gkConstraint
{
public:

	gkLimitRotConstraint();
	virtual ~gkLimitRotConstraint() {}


	bool update(gkScalar delta);
	gkConstraint* clone(gkGameObject* clob);


	GK_INLINE void setLimitX(const gkVector2& v) {m_flag |= 1; mXBounds = v;}
	GK_INLINE void setLimitY(const gkVector2& v) {m_flag |= 2; mYBounds = v;}
	GK_INLINE void setLimitZ(const gkVector2& v) {m_flag |= 4; mZBounds = v;}
	GK_INLINE void clearAxis(void) {m_flag = 0;}

private:

	short m_flag;

	gkVector2 mXBounds;
	gkVector2 mYBounds;
	gkVector2 mZBounds;
};

#endif//_gkLimitRotConstraint_h_
