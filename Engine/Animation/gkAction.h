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
#ifndef _gkAction_h_
#define _gkAction_h_

#include "gkMathUtils.h"
#include "gkResource.h"
#include "Animation/gkActionDefs.h"

class gkAction : public gkResource
{
protected:
	gkScalar             m_start, m_end;
	
public:
	gkAction(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
		:	gkResource(creator, name, handle), m_start(1), m_end(1)
	{}
	
	virtual ~gkAction() {}
	
	GK_INLINE gkScalar         getLength(void) const       { return m_end - m_start; }
	GK_INLINE gkScalar         getStart(void) const        { return m_start; }
	GK_INLINE gkScalar         getEnd(void) const          { return m_end; }
	
	GK_INLINE void             setStart(gkScalar v)        { m_start = v; }
	GK_INLINE void             setEnd(gkScalar v)          { m_end = v; }
	
	virtual void evaluate(const gkScalar& time, const gkScalar& delta, const gkScalar& weight, gkGameObject* object) const = 0;

};







#endif//_gkAction_h_
