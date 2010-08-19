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
class gkConstraint
{
public:

	gkConstraint();
	virtual ~gkConstraint() {}

	// Blending.
	GK_INLINE void              setInfluence(gkScalar v)                {m_influence = gkClampf(v, 0.0, 1.0);}
	GK_INLINE gkScalar          getInfluence(void)                      {return m_influence;}

	// Transform space.
	GK_INLINE void              setSpace(const gkTransformSpace &v)     {m_space = v;}
	GK_INLINE gkTransformSpace  getSpace(void)                          {return m_space;}


	// Matrix access.
	GK_INLINE void             setMatrix(const gkTransformState &m)     {m_matrix = m;}
	GK_INLINE gkTransformState &getMatrix(void)                         {return m_matrix;}


	// Object attachments.
	GK_INLINE void             setObject(gkGameObject *obj)             {m_object = obj;}
	GK_INLINE gkGameObject*    getObject(void)                          {return m_object;}


	// Main logic update.
	virtual bool            update(gkScalar delta) = 0;

	// Clone access.
	virtual gkConstraint   *clone(gkGameObject *clob) = 0;


	void _applyMatrix(void);

protected:

	gkGameObject           *m_object;
	gkTransformSpace        m_space;
	gkScalar                m_influence;
	gkTransformState        m_matrix;

};


#endif//_gkConstraint_h_
