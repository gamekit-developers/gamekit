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
#include "gkVisibilityActuator.h"
#include "gkGameObject.h"
#include "OgreSceneNode.h"



gkVisibilityActuator::gkVisibilityActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name),
	    m_flag(0)
{
}


gkVisibilityActuator::~gkVisibilityActuator()
{
}



gkLogicBrick* gkVisibilityActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkVisibilityActuator* act = new gkVisibilityActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}



void gkVisibilityActuator::execute(void)
{
	if (isPulseOff())
		return;

	if (!m_object->isInstanced())
		return;

	m_object->getNode()->setVisible((m_flag & VA_INVIS_FLAG) == 0, (m_flag & VA_CHILDREN) != 0);

	setPulse(BM_OFF);

}
