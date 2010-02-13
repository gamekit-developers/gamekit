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
#include "gkMotionActuator.h"
#include "gkGameObject.h"



gkMotionActuator::gkMotionActuator(gkGameObject *object, const gkString &name) 
:       gkLogicActuator(object, name),
        m_type(0), m_loc(0,0,0), m_rot(0,0,0), m_locL(false), m_rotL(false)
{
}

gkMotionActuator::~gkMotionActuator()
{
}

void gkMotionActuator::execute(void)
{
    GK_ASSERT(m_object);
    if (!m_object->isLoaded())
        return;

    if (m_type == MT_SIMPLE)
    {
        if (!gkFuzzyVec(m_loc))
            m_object->translate(m_loc, m_locL ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
        if (!gkFuzzyVec(m_rot))
            m_object->rotate(gkMathUtils::getQuatFromEuler(m_rot, false), m_rotL ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
    }
}
