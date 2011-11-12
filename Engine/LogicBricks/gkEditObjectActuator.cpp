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
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkParticleObject.h"
#include "gkGameObjectInstance.h"
#include "gkEditObjectActuator.h"
#include "gkCamera.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "gkSceneManager.h"
#include "gkGameObjectManager.h"



gkEditObjectActuator::gkEditObjectActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name), m_linv(0, 0, 0), m_angv(0, 0, 0),
	    m_lvlocal(false), m_avlocal(false), m_mode(0), m_dynMode(0), m_life(0), m_obj(""),m_lastCreatedObject(0)
{
	gkGameObjectManager::getSingletonPtr()->addInstanceListener(this);
}



gkEditObjectActuator::~gkEditObjectActuator()
{
	gkGameObjectManager::getSingletonPtr()->removeInstanceListener(this);
}


gkLogicBrick* gkEditObjectActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkEditObjectActuator* act = new gkEditObjectActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}


void gkEditObjectActuator::addObject(void)
{
	if (!m_obj.empty())
	{
		gkScene* scene = m_object->getOwner();
		if (scene->hasObject(m_obj))
		{
			gkGameObject* obj = scene->getObject(m_obj);


			if ( obj->isInGroup() || obj->isGroupInstance() )
			{
				// Spawn entire group.
				gkTransformState thisTransform( m_object->getWorldPosition(),
				                                m_object->getWorldOrientation(),
				                                m_object->getWorldScale()
				                              );

				if (obj->isInGroup())
					obj->getGroup()->cloneObjects(m_scene, thisTransform, m_life, m_linv, m_lvlocal, m_angv, m_avlocal);
				else
					obj->getGroupInstance()->cloneObjects(m_scene, thisTransform, m_life, m_linv, m_lvlocal, m_angv, m_avlocal);
			}
			else
			{
				gkGameObject* nobj = scene->cloneObject(obj, m_life);

				m_lastCreatedObject = nobj;

				gkGameObjectProperties& props = nobj->getProperties();

				props.m_transform.loc = m_object->getWorldPosition();
				props.m_transform.rot = m_object->getWorldOrientation();
				nobj->createInstance();

				// apply velocities
				nobj->setLinearVelocity(m_linv, m_lvlocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);
				nobj->setAngularVelocity(m_angv,  m_avlocal ? TRANSFORM_LOCAL : TRANSFORM_PARENT);

				
				if (props.hasParticles())
				{
					UTsize i;
					for (i = 0; i < props.m_particleObjs.size(); i++)
					{
						gkGameObject* child = scene->getObject(props.m_particleObjs[i]);						
						if (child)
						{
							gkGameObject* pobj = scene->cloneObject(child, m_life);

							gkGameObjectProperties& pprops = pobj->getProperties();

							pprops.m_transform = props.m_transform;

							pobj->createInstance();
							pobj->setParentInPlace(nobj);
						}
					}
				}

			}

		}

	}
}



void gkEditObjectActuator::endObject(void)
{
	if (m_object)
	{
		if (m_lastCreatedObject==m_object)
		{
			m_lastCreatedObject=NULL;
		}
		m_object->getOwner()->endObject(m_object);
	}
}

void gkEditObjectActuator::trackToObject(void)
{
    if (m_obj.empty())
        return;

    gkGameObject* trackTarget = m_scene->findInstancedObject(m_obj);
    if (!trackTarget)
    {
        // TODO: Log warning message
        return;
    }

    gkGameObjectTypes type = m_object->getType();

    if (GK_CAMERA==type)
    {
        gkCamera* camera = m_object->getCamera();
        if (!camera)
        {
            // TODO: Log warning message
            return;
        }

        camera->getCamera()->setAutoTracking(true, trackTarget->getNode());
        camera->getCamera()->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);
    }
    else
    {
        Ogre::SceneNode* sceneNode = m_object->getNode();
        if (!sceneNode)
        {
            // TODO: log warning message
            return;
        }

        sceneNode->setAutoTracking(true, trackTarget->getNode(), Ogre::Vector3::UNIT_Y);
    }
}


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
    case EO_TRACKTO:
        trackToObject();
        break;
    default:
        // TODO: Log some warning message
        break;
	}

	setPulse(BM_OFF);
}
