/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Xavier T.
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
#ifndef _akAnimation_h_
#define _akAnimation_h_

#include "utCommon.h"
#include "utString.h"

#include "akCommon.h"
#include "akMathUtils.h"



class akAnimation
{
protected:
	akScalar             m_start, m_end;
	
public:
	akAnimation() : m_start(1), m_end(1) {}
	virtual ~akAnimation() {}
	
	UT_INLINE akScalar         getLength(void) const       { return m_end - m_start; }
	UT_INLINE akScalar         getStart(void) const        { return m_start; }
	UT_INLINE akScalar         getEnd(void) const          { return m_end; }
	
	UT_INLINE void             setStart(akScalar v)        { m_start = v; }
	UT_INLINE void             setEnd(akScalar v)          { m_end = v; }
	
	virtual void evaluate(const akScalar& time, const akScalar& weight, void* object) const = 0;

};







#endif//_akAnimation_h_
