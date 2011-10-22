/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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
#include "gkRandomActuator.h"
#include "gkGameObject.h"
#include "gkVariable.h"



gkRandomActuator::gkRandomActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	: gkLogicActuator(object, link, name), m_seed(0), m_distribution(0), m_prop(""),
	  m_min(0), m_max(1), m_constant(0), m_mean(0), m_deviation(0), m_halflife(0), m_current(0), m_count(0)
{
	m_randGen = new utRandomNumberGenerator(0);
}


gkRandomActuator::~gkRandomActuator()
{
	delete m_randGen;
}


gkLogicBrick* gkRandomActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkRandomActuator* act = new gkRandomActuator(*this);
	act->cloneImpl(link, dest);
	act->m_randGen = new utRandomNumberGenerator(m_seed);
	return act;
}


void gkRandomActuator::setSeed(int v)
{
	m_seed = v;
	m_randGen->setSeed(m_seed);
	m_current = m_randGen->rand32();
	m_count = 0;
}


void gkRandomActuator::execute(void)
{
	gkVariable* variable;

	if (isPulseOff())
		return;

	if (!m_object->isInstanced())
		return;

	if (m_object->hasVariable(m_prop))
		variable = m_object->getVariable(m_prop);
	else
		return;


	switch (m_distribution)
	{
	case RA_BOOL_CONSTANT:
		variable->setValue((bool)(m_constant > 0.5f));
		return;

	case RA_BOOL_UNIFORM:
		if (m_count > 31)
		{
			m_current = m_randGen->rand32();
			m_count = 0;
		}
		variable->setValue((bool)((m_current >> m_count) & 0x1));
		m_count ++;
		return;

	case RA_BOOL_BERNOUILLI:
		variable->setValue((bool)(m_randGen->randUnit() < m_mean));
		return;

	case RA_INT_CONSTANT:
		variable->setValue((int)(m_constant + 0.5));
		return;

	case RA_INT_UNIFORM:
		{
			int min = (int)m_min + 0.5;
			int max = (int)m_max + 0.5;
			if (max > min)
			{
				variable->setValue(m_randGen->randRangeInt(min, max));
			}
			return;
		}

	case RA_INT_POISSON:
		variable->setValue(m_randGen->randPoisson(m_mean));
		return;

	case RA_FLOAT_CONSTANT:
		variable->setValue(m_constant);
		return;

	case RA_FLOAT_UNIFORM:
		variable->setValue(m_randGen->randRange(m_min, m_max));
		return;

	case RA_FLOAT_NORMAL:
		variable->setValue(m_randGen->randNormal(m_mean, m_deviation));
		return;

	case RA_FLOAT_NEGEXP:
		variable->setValue(m_randGen->randNegativeExponential(m_halflife));
		gkMath::Exp(0.f);
		return;
	}

	setPulse(BM_OFF);

}
