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
#include "gkEditObjectActuator.h"
#include "gkScene.h"
#include "gkGameObject.h"


// ----------------------------------------------------------------------------
gkEditObjectActuator::gkEditObjectActuator(gkGameObject *object, gkLogicLink *link, const gkString &name)
	:   gkLogicActuator(object, link, name), m_linv(0,0,0), m_angv(0,0,0),
	    m_lvlocal(false), m_avlocal(false), m_mode(0), m_dynMode(0), m_life(0), m_obj("")
{
}


// ----------------------------------------------------------------------------
gkEditObjectActuator::~gkEditObjectActuator()
{
}

// ----------------------------------------------------------------------------
gkLogicBrick *gkEditObjectActuator::clone(gkLogicLink *link, gkGameObject *dest)
{
	gkEditObjectActuator *act = new gkEditObjectActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}

// ----------------------------------------------------------------------------
void gkEditObjectActuator::addObject(void)
{
	if (!m_obj.empty())
	{
		gkScene *scene = m_object->getOwner();
		if (scene->hasObject(m_obj))
		{
			gkGameObject *obj = scene->getObject(m_obj);

			gkGameObjectGroup *grp = obj->getGroup();

			if (grp !=0 )
			{

				// spawn entire group (destructible objects)
				gkTransformState parSt( m_object->getWorldPosition(),
				                        m_object->getWorldOrientation(),
				                        m_object->getWorldScale()
				                      );


				gkMatrix4 p = parSt.toMatrix(), c;


				gkGameObjectGroup::ObjectIterator objs = grp->getObjects();

				while (objs.hasMoreElements())
				{
					obj = objs.getNext().second;
					gkGameObject *nobj = scene->cloneObject(obj, m_life);

					c = obj->getProperties().m_transform.toMatrix();
					c = p * c;
					nobj->getProperties().m_transform = gkTransformState(c);

					nobj->load();

					// apply velocities
					nobj->setLinearVelocity(m_linv, m_lvlocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
					nobj->setAngularVelocity(m_angv,  m_avlocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
				}
			}
			else
			{
				gkGameObject *nobj = scene->cloneObject(obj, m_life);

				nobj->getProperties().m_transform.loc = m_object->getWorldPosition();
				nobj->getProperties().m_transform.rot = m_object->getWorldOrientation();
				nobj->load();

				// apply velocities
				nobj->setLinearVelocity(m_linv, m_lvlocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
				nobj->setAngularVelocity(m_angv,  m_avlocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
			}

		}

	}
}


// ----------------------------------------------------------------------------
void gkEditObjectActuator::endObject(void)
{
	if (m_object)
		m_object->getOwner()->endObject(m_object);
}


// ----------------------------------------------------------------------------
void gkEditObjectActuator::execute(void)
{
	if (isPulseOff())
		return;


	switch (m_mode)
	{
	case EO_ADDOBJ:
		addObject();
		break;
	case EO_ENDOBJ:
		endObject();
		break;
	}

	setPulse(BM_OFF);
}
