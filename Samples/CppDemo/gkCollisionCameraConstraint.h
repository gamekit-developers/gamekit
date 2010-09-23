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
#ifndef _gkCollisionCameraConstraint_h_
#define _gkCollisionCameraConstraint_h_

#include "Constraints/gkConstraint.h"


class gkCollisionCameraConstraint : public gkConstraint
{
public:

	gkCollisionCameraConstraint();
	virtual ~gkCollisionCameraConstraint();

	bool update(gkScalar delta);


	gkConstraint* clone(gkGameObject* clob);


	///Target to include in casting, fires a ray from m_object to the target.
	GK_INLINE void setTarget(gkGameObject* tar) {m_target = tar;}

	///Ray length/distance from object to target.
	GK_INLINE void setLength(gkScalar v) {m_length = v;}
	///Moves the ray forward
	GK_INLINE void setForwardOffs(gkScalar v) {m_yOffs = v;}

	///Moves the ray down
	GK_INLINE void setDownOffs(gkScalar v) {m_zOffs = v;}


private:

	gkGameObject* m_target;
	class gkRayTest* m_cast;
	gkScalar m_length, m_yOffs, m_zOffs, m_lhf;
};



#endif//_gkCollisionCameraConstraint_h_
