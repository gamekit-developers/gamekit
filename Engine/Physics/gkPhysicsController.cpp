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

#include "gkDynamicsWorld.h"
#include "gkPhysicsController.h"
#include "gkEntity.h"
#include "gkMesh.h"
#include "gkCharacter.h"

#include "OgreSceneNode.h"
#include "OgreMovableObject.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"




gkPhysicsController::gkPhysicsController(gkGameObject* object, gkDynamicsWorld* owner)
	:    m_owner(owner),
	     m_object(object),
	     m_collisionObject(0),
	     m_shape(0),
	     m_suspend(false),
	     m_dbvtMark(true)
{
	// initial copy from object
	m_props = object->getProperties().m_physics;
}



gkPhysicsController::~gkPhysicsController()
{
	if (m_shape)
	{
		destroyShape(m_shape);
		m_shape = 0;
	}
}

void gkPhysicsController::destroyShape(btCollisionShape* shape)
{
	if (!shape) return;

	if (shape->isCompound())
	{
		btCompoundShape* compShape = static_cast<btCompoundShape*>(shape);
		int i;
		for (i = 0; i < compShape->getNumChildShapes(); i++)
		{
			btCollisionShape* childShape = compShape->getChildShape(i);
			if (childShape->isCompound())
				destroyShape(childShape);
			else
				delete childShape;
		}
	}

	delete shape;
}

void gkPhysicsController::setShape(btCollisionShape* shape)
{
	if (m_collisionObject)
	{
		destroyShape(m_shape);

		m_shape = shape;
		m_collisionObject->setCollisionShape(m_shape);
	}
}


bool gkPhysicsController::isStaticObject(void)
{
	return m_props.isStatic();
}


gkPhysicsProperties& gkPhysicsController::getProperties(void)
{
	GK_ASSERT(m_object);
	return m_props;
}


gkContactInfo::Array& gkPhysicsController::getContacts(void)
{
	return m_localContacts;
}


gkContactInfo::Iterator gkPhysicsController::getContactIterator(void)
{
	return gkContactInfo::Iterator(m_localContacts);
}



bool gkPhysicsController::collidesWith(gkGameObject* ob, gkContactInfo* cpy)
{
	if (!m_localContacts.empty())
	{
		UTsize i, s;
		gkContactInfo::Array::Pointer p;

		i = 0;
		s = m_localContacts.size();
		p = m_localContacts.ptr();

		while (i < s)
		{
			GK_ASSERT(p[i].collider);

			if (p[i].collider->getObject() == ob)
			{
				if (cpy) *cpy = p[i];
				return true;
			}
			++i;
		}
	}
	return false;
}



bool gkPhysicsController::collidesWith(const gkString& name, gkContactInfo* cpy, bool emptyFilter)
{

	if (!m_localContacts.empty())
	{
		if (name.empty() && emptyFilter)
		{
			if (cpy) *cpy = m_localContacts.at(0);
			return true;
		}


		UTsize i, s;
		gkContactInfo::Array::Pointer p;

		i = 0;
		s = m_localContacts.size();
		p = m_localContacts.ptr();

		while (i < s)
		{
			GK_ASSERT(p[i].collider);
			gkGameObject* gobj = p[i].collider->getObject();

			if (name.find(gobj->getName()) != gkString::npos)
			{
				if (cpy) *cpy = p[i];
				return true;
			}

			++i;
		}
	}

	return false;
}




bool gkPhysicsController::sensorCollides(const gkString& prop, const gkString& material, bool onlyActor, bool testAllMaterials, utArray<gkGameObject*>* collisionList)
{
	if (collisionList){
		collisionList->clear();
	}
	if (onlyActor && !m_object->getProperties().isActor())
		return false;


	if (!m_localContacts.empty())
	{
		if (!collisionList && prop.empty() && material.empty())
		{
			// there are contacts and we do not care about property, nor empty, nor we need a list of objects (list = NULL)
			// any filter
			return true;
		}

		UTsize i, s;
		gkContactInfo::Array::Pointer p;

		i = 0;
		s = m_localContacts.size();
		p = m_localContacts.ptr();

		while (i < s)
		{
			GK_ASSERT(p[i].collider);
			gkGameObject* gobj = p[i].collider->getObject();


			if (onlyActor)
			{

				if (prop.empty() && material.empty())
					return true;

				if (gobj->getProperties().isActor())
				{
					if (!prop.empty())
					{
						if (gobj->hasVariable(prop)){
							if (!collisionList){
								return true;
							}
							else if (collisionList->find(gobj)==UT_NPOS)
								collisionList->push_back(gobj);
						}
					}
					else if (!material.empty())
					{
						if (gobj->hasSensorMaterial(material, !testAllMaterials))
						{
							if (!collisionList)
							{
								return true;
							}
							else if (collisionList->find(gobj)==UT_NPOS){
								collisionList->push_back(gobj);
							}
						}
					}
				}
			}
			else
			{
				if (prop.empty() && material.empty())
					if (!collisionList)
					{
						return true;
					}
					else if (collisionList->find(gobj)==UT_NPOS)
					{
						collisionList->push_back(gobj);
					}

				if (!prop.empty())
				{
					if (gobj->hasVariable(prop))
					{
						if (!collisionList)
						{
							return true;
						}
						else if (collisionList->find(gobj)==UT_NPOS)
						{
							collisionList->push_back(gobj);
						}
					}
				}
				else if (!material.empty())
				{
					if (gobj->hasSensorMaterial(material, !testAllMaterials)){
						if (!collisionList)
						{
							return true;
						}
						else if (collisionList->find(gobj)==UT_NPOS)
						{
							collisionList->push_back(gobj);
						}
					}
				}
			}

			++i;
		}
	}
	if (!collisionList)
		return false;
	else if (collisionList->empty())
		return false;
	else
		return true;
}




bool gkPhysicsController::_markDbvt(bool v)
{
	if (m_suspend)
		return false;

	bool result = false;
	if (m_dbvtMark != v)
	{
		m_dbvtMark = v;
		if ( m_object->getType() == GK_ENTITY &&  !m_object->getProperties().isInvisible())
		{
			Ogre::MovableObject* mov = m_object->getMovable();

			if (mov)
			{
				result = mov->isVisible() != m_dbvtMark;
				mov->setVisible(m_dbvtMark);
			}
		}
	}
	return result;
}



bool gkPhysicsController::sensorTest(gkGameObject* ob, const gkString& prop, const gkString& material, bool onlyActor, bool testAllMaterials)
{
	GK_ASSERT(ob);

	if (onlyActor)
	{
		if (ob->getProperties().isActor())
		{
			if (prop.empty() && material.empty())
				return true;

			if (!prop.empty())
			{
				if (ob->hasVariable(prop))
					return true;
			}
			else if (!material.empty())
			{
				if (ob->hasSensorMaterial(material, !testAllMaterials))
					return true;
			}
		}
	}
	else
	{
		if (prop.empty() && material.empty())
			return true;

		if (!prop.empty())
		{
			if (ob->hasVariable(prop))
				return true;
		}
		else if (!material.empty())
		{
			if (ob->hasSensorMaterial(material, !testAllMaterials))
				return true;
		}
	}

	return false;
}



void gkPhysicsController::setTransformState(const gkTransformState& state)
{
	if (m_suspend || !m_collisionObject)
		return;

	m_collisionObject->setWorldTransform(state.toTransform());
}



void gkPhysicsController::updateTransform(void)
{
	if (m_suspend || !m_collisionObject)
		return;


	btTransform worldTrans;
	worldTrans.setIdentity();

	gkQuaternion rot;
	gkVector3 loc;

	if (!m_object->getParent())
	{
		rot = m_object->getOrientation();
		loc = m_object->getPosition();
	}
	else
	{
		rot = m_object->getWorldOrientation();
		loc = m_object->getWorldPosition();
	}


	worldTrans.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
	worldTrans.setOrigin(btVector3(loc.x, loc.y, loc.z));

	m_collisionObject->setWorldTransform(worldTrans);
}



void gkPhysicsController::enableContactProcessing(bool v)
{
	if (m_object)
	{
		if (v)
		{
			if (!m_props.isContactListener())
				m_props.m_mode |= GK_CONTACT;
		}
		else
		{
			if (m_props.isContactListener())
				m_props.m_mode = m_props.m_mode ^ GK_CONTACT;
		}
	}
}


gkPhysicsController* gkPhysicsController::castController(void* colObj)
{
	GK_ASSERT(colObj);
	return castController(static_cast<btCollisionObject*>(colObj));
}


gkPhysicsController* gkPhysicsController::castController(btCollisionObject* colObj)
{
	GK_ASSERT(colObj);
	return static_cast<gkPhysicsController*>(colObj->getUserPointer());
}



gkGameObject* gkPhysicsController::castObject(btCollisionObject* colObj)
{
	GK_ASSERT(colObj);
	gkPhysicsController* cont = static_cast<gkPhysicsController*>(colObj->getUserPointer());
	GK_ASSERT(cont);
	return cont->getObject();
}


gkGameObject* gkPhysicsController::castObject(const btCollisionObject* colObj)
{
	return castObject(const_cast<btCollisionObject*>(colObj));
}



void gkPhysicsController::suspend(bool v)
{
	if (m_suspend != v && m_collisionObject)
	{
		m_suspend = v;

		if (m_suspend)
			m_object->getProperties().m_physics.m_type = GK_NO_COLLISION;
		else
			m_object->getProperties().m_physics.m_type = getProperties().m_type;


		GK_ASSERT(m_owner);
		btDynamicsWorld* dyn = getOwner();


		btRigidBody* body = btRigidBody::upcast(m_collisionObject);
		btGhostObject* ghost = btGhostObject::upcast(m_collisionObject);
		if (m_suspend)
		{
			if (body)
				dyn->removeRigidBody(body);
			else if (ghost)
			{				
				dyn->removeAction(static_cast<gkCharacter*>(this));
				dyn->removeCollisionObject(m_collisionObject);
			}
			else
				dyn->removeCollisionObject(m_collisionObject);
		}
		else
		{
			if (body)
				dyn->addRigidBody(body);
			else if (ghost)
			{
				dyn->addCollisionObject(ghost, btBroadphaseProxy::CharacterFilter);
				dyn->addAction(static_cast<gkCharacter*>(this));
			}
			else
				dyn->addCollisionObject(m_collisionObject);
		}
	}
}


btCollisionObject* gkPhysicsController::getCollisionObject(void)
{
	return m_collisionObject;
}



btCollisionShape* gkPhysicsController::getShape(void)
{
	return m_shape;
}




btDynamicsWorld* gkPhysicsController::getOwner(void)
{
	if (m_owner)
		return m_owner->getBulletWorld();
	return 0;
}



gkGameObject* gkPhysicsController::getObject(void)
{
	return m_object;
}


gkBoundingBox gkPhysicsController::getAabb(void) const
{
	if (m_collisionObject)
	{
		btVector3 aabbMin;
		btVector3 aabbMax;

		m_collisionObject->getCollisionShape()->getAabb(m_collisionObject->getWorldTransform(), aabbMin, aabbMax);

		gkVector3 min_aabb(aabbMin.x(), aabbMin.y(), aabbMin.z());
		gkVector3 max_aabb(aabbMax.x(), aabbMax.y(), aabbMax.z());


		return gkBoundingBox(min_aabb, max_aabb);
	}

	return gkBoundingBox();
}

void gkPhysicsController::createShape(void)
{
	GK_ASSERT(!m_shape && m_object);

	m_shape = _createShape();
}

btCollisionShape* gkPhysicsController::_createShape(void)
{
	gkMesh* me = 0;
	gkEntity* ent = m_object->getEntity();
	if (ent != 0)
		me = ent->getEntityProperties().m_mesh;

	gkVector3 size(1.f, 1.f, 1.f);
	if (me != 0)
		size = me->getBoundingBox().getHalfSize();
	else
		size *= m_props.m_radius;

	btCollisionShape* shape = 0;	

	switch (m_props.m_shape)
	{
	case SH_BOX:
		shape = new btBoxShape(btVector3(size.x, size.y, size.z));
		break;
	case SH_CONE:
		shape = new btConeShapeZ(gkMax(size.x, size.y), 2.f * size.z);
		break;
	case SH_CYLINDER:
		shape = new btCylinderShapeZ(btVector3(size.x, size.y, size.z));
		break;
	case SH_CONVEX_TRIMESH:
	case SH_GIMPACT_MESH:
	case SH_BVH_MESH:
		{
			if (me != 0)
			{
				btTriangleMesh* triMesh = me->getTriMesh();
				if (triMesh->getNumTriangles() > 0)
				{
					if (m_props.m_shape == SH_CONVEX_TRIMESH)
						shape = new btConvexTriangleMeshShape(triMesh);
					else if (m_props.m_shape == SH_GIMPACT_MESH)
						shape = new btConvexTriangleMeshShape(triMesh);
					else
						shape = new btBvhTriangleMeshShape(triMesh, true);
					break;
				}
				else
					return 0;
			}
		}
	case SH_SPHERE:
		shape = new btSphereShape(gkMax(size.x, gkMax(size.y, size.z)));
		break;
	}

	if (!shape)
		return 0;

	shape->setMargin(m_props.m_margin);
	shape->setLocalScaling(gkMathUtils::get(m_object->getScale()));


	if (m_props.isCompound())
	{
		btCompoundShape *compShape = new btCompoundShape();
		compShape->addChildShape(btTransform::getIdentity(), shape);
		return compShape;
	}
	
	return shape;
	
}




void gkPhysicsController::setTransform(const btTransform& worldTrans)
{
	GK_ASSERT(m_object && m_object->isInstanced());


	const gkQuaternion& rot = gkMathUtils::get(worldTrans.getRotation());
	const gkVector3& loc = gkMathUtils::get(worldTrans.getOrigin());

	// apply to the node and sync state next update

	Ogre::SceneNode* node = m_object->getNode();

	node->setOrientation(rot);
	node->setPosition(loc);

	m_object->notifyUpdate();
}



void gkPhysicsController::_handleManifold(btPersistentManifold* manifold)
{
	if (m_suspend || !m_props.isContactListener() || !m_object->isInstanced())
		return;


	gkPhysicsController* colA = castController(manifold->getBody0());
	gkPhysicsController* colB = castController(manifold->getBody1());

	gkPhysicsController* collider = colB;

	if (collider == this)
	{
		collider = colA;
	}

	int nrc = manifold->getNumContacts();

	if (nrc)
	{
		m_localContacts.reserve(nrc);

		for (int j = 0; j < nrc; ++j)
		{
			gkContactInfo cinf;
			btManifoldPoint& pt = manifold->getContactPoint(j);

			if (pt.getDistance() < 0.f)
			{
				cinf.collider = collider;
				cinf.point    = pt;

				m_localContacts.push_back(cinf);
			}
		}
	}
}



void gkPhysicsController::_resetContactInfo(void)
{
	if (m_props.isContactListener())
		m_localContacts.clear(true);
}
