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
#include "gkPropertyActuator.h"
#include "gkGameObject.h"
#include "gkScene.h"


// ----------------------------------------------------------------------------
gkPropertyActuator::gkPropertyActuator(gkGameObject *object, gkLogicLink *link, const gkString &name)
	:   gkLogicActuator(object, link, name),
	    m_type(0), m_prop(""), m_value(""), m_othOb(""), m_init(false),
	    m_cur(0), m_oth(0)
{
}

// ----------------------------------------------------------------------------
gkPropertyActuator::~gkPropertyActuator()
{
}


// ----------------------------------------------------------------------------
gkLogicBrick *gkPropertyActuator::clone(gkLogicLink *link, gkGameObject *dest)
{
	gkPropertyActuator *act = new gkPropertyActuator(*this);
	act->cloneImpl(link, dest);
	act->m_init = false;
	act->m_cur = 0;
	return act;
}

// ----------------------------------------------------------------------------
void gkPropertyActuator::execute(void)
{
	if (isPulseOff())
		return;

	if (!m_object->isLoaded())
		return;

	if (!m_init)
	{
		if (m_object->hasVariable(m_prop))
		{

			m_cur = m_object->getVariable(m_prop);
			m_init = true;

			if (!m_othOb.empty())
			{
				gkGameObject *ob = m_object->getOwner()->getObject(m_othOb);
				if (ob->hasVariable(m_value))
					m_oth = ob->getVariable(m_value);
			}
		}
		else
		{
			//m_suspend = true;
			return;
		}
	}

	if (m_cur)
	{

		switch (m_type)
		{
		case PA_ASSIGN:
			m_cur->assign(m_value);
			break;
		case PA_ADD:
			m_cur->add(m_value);
			break;
		case PA_TOGGLE:
			m_cur->toggle(m_value);
			m_value = m_cur->getValueString();
			break;
		case PA_COPY:
			if (m_oth)
				m_cur->assign(m_oth->getValueString());
			break;
		}
	}
}
