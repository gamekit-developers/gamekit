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
#include "gkConstraintManager.h"
#include "gkConstraint.h"
#include "gkGameObject.h"



gkConstraintManager::gkConstraintManager()
{
}



gkConstraintManager::~gkConstraintManager()
{
	clear();
}



void gkConstraintManager::clear(void)
{
	m_updateConstraints.clear();

	UTsize i;

	utHashTableIterator<ConstraintObjectMap> iter(m_objectMapConstraints);
	while (iter.hasMoreElements())
	{
		Constraints* cons = iter.getNext().second;

		for (i = 0; i < cons->size(); ++i)
		{
			gkConstraint* co = cons->at(i);
			delete co;
		}

		delete cons;
	}

	m_objectMapConstraints.clear();
}



void gkConstraintManager::notifyObjectDestroyed(gkGameObject* gobj)
{
	UTsize pos, i;

	pos = m_objectMapConstraints.find(gobj);
	if (pos != UT_NPOS)
	{
		Constraints* oldc = m_objectMapConstraints.at(pos);
		for (i = 0; i < oldc->size(); ++i)
		{
			gkConstraint* cons = oldc->at(i);

			removeUpdate(cons);
			delete cons;
		}


		m_objectMapConstraints.remove(gobj);
		delete oldc;
	}

}



void gkConstraintManager::notifyObjectCloned(gkGameObject* oobj, gkGameObject* nobj)
{
	UTsize pos, i;

	pos = m_objectMapConstraints.find(oobj);
	if (pos != UT_NPOS)
	{
		GK_ASSERT(!hasConstraints(nobj));

		Constraints* oldc = m_objectMapConstraints.at(pos);
		Constraints* newc = new Constraints();

		for (i = 0; i < oldc->size(); ++i)
			newc->push_back(oldc->at(i)->clone(nobj));


		m_objectMapConstraints.insert(nobj, newc);
	}
}



void gkConstraintManager::notifyInstanceCreated(gkGameObject* gobj)
{
	Constraints& cons = getConstraints(gobj);

	if (!cons.empty())
	{
		ConstraintIterator iter(cons);
		while (iter.hasMoreElements())
			m_updateConstraints.push_back(iter.getNext());
	}
}


void gkConstraintManager::notifyInstanceDestroyed(gkGameObject* gobj)
{
	Constraints& cons = getConstraints(gobj);

	if (!cons.empty())
	{
		ConstraintIterator iter(cons);
		while (iter.hasMoreElements())
			removeUpdate(iter.getNext());
	}
}



void gkConstraintManager::addConstraint(gkGameObject* obj, gkConstraint* cons)
{
	UTsize pos;

	GK_ASSERT(obj && cons);
	cons->setObject(obj);


	pos = m_objectMapConstraints.find(obj);
	if (pos == UT_NPOS)
	{
		Constraints* arr = new Constraints();
		arr->push_back(cons);

		m_objectMapConstraints.insert(obj, arr);
	}
	else
		m_objectMapConstraints.at(pos)->push_back(cons);
}



void gkConstraintManager::removeConstraint(gkGameObject* obj, gkConstraint* cons)
{
	UTsize pos;

	removeUpdate(cons);

	pos = m_objectMapConstraints.find(obj);
	if (pos != UT_NPOS)
	{
		Constraints* cos = m_objectMapConstraints.at(pos);
		if (cos != 0)
		{
			pos = cos->find(cons);
			if (pos != UT_NPOS)
			{
				cos->erase(pos);
				delete cons;
			}

			if (cos->empty())
			{
				m_objectMapConstraints.remove(cos);
				delete cos;
			}
		}
	}
}



gkConstraintManager::Constraints& gkConstraintManager::getConstraints(gkGameObject* obj)
{
	static Constraints nill;

	UTsize pos = m_objectMapConstraints.find(obj);
	if (pos != UT_NPOS)
	{
		Constraints* cos = m_objectMapConstraints.at(pos);
		if (cos != 0)
			return (*cos);
	}

	return nill;
}



bool gkConstraintManager::hasConstraints(gkGameObject* obj)
{
	return m_objectMapConstraints.find(obj) != UT_NPOS;
}



void gkConstraintManager::update(gkScalar delta)
{
	if (!m_updateConstraints.empty())
	{
		UTsize i, s;
		Constraints::Pointer p;

		i = 0;
		s = m_updateConstraints.size();
		p = m_updateConstraints.ptr();


		while (i < s)
		{
			gkConstraint* co = p[i++];
			gkGameObject* ob = co->getObject();

			if (ob && ob->isInstanced())
			{
				if (co->update(delta))
					co->_applyMatrix();
			}
		}
	}
}



void gkConstraintManager::removeUpdate(gkConstraint* cons)
{
	UTsize pos;
	pos = m_updateConstraints.find(cons);
	if (pos != UT_NPOS)
		m_updateConstraints.erase(cons);
}
