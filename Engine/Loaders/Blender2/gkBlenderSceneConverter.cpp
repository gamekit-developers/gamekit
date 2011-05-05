/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): xavier.thomas.1980.
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
#include "Blender.h"

#include "gkBlenderDefines.h"
#include "gkBlenderSceneConverter.h"
#include "Converters/gkAnimationConverter.h"
#include "Converters/gkLogicBrickConverter.h"
#include "Converters/gkMeshConverter.h"
#include "Converters/gkSkeletonConverter.h"
#include "gkParticleManager.h"
#include "gkParticleObject.h"
#include "gkParticleResource.h"
#include "OgreKit.h"



gkBlenderSceneConverter::gkBlenderSceneConverter(gkBlendFile* fp, Blender::Scene* sc)
	:   m_bscene(sc), m_gscene(0), m_file(fp), m_groupName(fp->getResourceGroup())
{
	m_logic = new gkLogicLoader();
}


gkBlenderSceneConverter::~gkBlenderSceneConverter()
{
	delete m_logic;
}



bool gkBlenderSceneConverter::validObject(Blender::Object* bobj)
{
	// test for usable object type
	switch (bobj->type)
	{
	case OB_EMPTY:      // SceneNode
	case OB_LAMP:       // Light
	case OB_CAMERA:     // Camera
	case OB_MESH:       // Entity + Mesh
	case OB_ARMATURE:   // SceneNode + Skeleton
		return true;
	}
	return false;
}



void gkBlenderSceneConverter::applyParents(utArray<Blender::Object*> &children)
{
	UTsize i;
	for (i = 0; i < children.size(); i++)
	{
		Blender::Object* bchild = children.at(i);
		gkGameObject*    gchild = m_gscene->getObject(GKB_IDNAME(bchild));

		if (gchild)
		{
			gkGameObject* gpar = m_gscene->getObject(GKB_IDNAME(bchild->parent));
			if (gpar)
				gchild->setParent(gpar);
		}
	}
}



void gkBlenderSceneConverter::convertSoundScene(void)
{
#ifdef OGREKIT_OPENAL_SOUND

	if (!m_gscene)
		return;

	gkSoundSceneProperties& props = m_gscene->getSoundScene();

	if (m_bscene->audio.distance_model == 0)
		props.m_distModel = gkSoundSceneProperties::DM_NONE;
	else if (m_bscene->audio.distance_model == 1)
		props.m_distModel = gkSoundSceneProperties::DM_INVERSE;
	else if (m_bscene->audio.distance_model == 2)
		props.m_distModel = gkSoundSceneProperties::DM_INVERSE_CLAMP;
	else if (m_bscene->audio.distance_model == 3)
		props.m_distModel = gkSoundSceneProperties::DM_LINEAR;
	else if (m_bscene->audio.distance_model == 4)
		props.m_distModel = gkSoundSceneProperties::DM_LINEAR_CLAMP;
	else if (m_bscene->audio.distance_model == 5)
		props.m_distModel = gkSoundSceneProperties::DM_EXPONENT;
	else if (m_bscene->audio.distance_model == 6)
		props.m_distModel = gkSoundSceneProperties::DM_EXPONENT_CLAMP;

	props.m_sndSpeed = m_bscene->audio.speed_of_sound;
	props.m_dopplerFactor = m_bscene->audio.doppler_factor;

#endif
}



void gkBlenderSceneConverter::convertWorld(void)
{
	if (!m_gscene)
		return;

	gkSceneProperties& sprops = m_gscene->getProperties();
	gkSceneMaterial& props = sprops.m_material;

	if (m_bscene->world)
	{
		Blender::World* world = m_bscene->world;

		sprops.m_gravity    = gkVector3(0.f, 0.f, -world->gravity);
		props.m_ambient.r   = world->ambr;
		props.m_ambient.g   = world->ambg;
		props.m_ambient.b   = world->ambb;
		props.m_horizon.r   = world->horr;
		props.m_horizon.g   = world->horg;
		props.m_horizon.b   = world->horb;
		props.m_zenith.r    = world->zenr;
		props.m_zenith.g    = world->zeng;
		props.m_zenith.b    = world->zenb;
		props.m_name = GKB_IDNAME(world);

		// take half far distance of the main camera
		props.m_distance = (m_bscene->camera && m_bscene->camera->type == OB_CAMERA ?
		                    ((Blender::Camera*)m_bscene->camera->data)->clipend * 0.5f : 10000.f);


		if ((world->skytype & WO_SKYBLEND) || (world->skytype & WO_SKYREAL))
		{
			props.m_type = (world->skytype & WO_SKYBLEND) ? gkSceneMaterial::LINEAR : gkSceneMaterial::REFLECTED;
		}

		if (world->mode & WO_MIST)
		{
			sprops.m_fog.m_mode          =  world->mistype == 0 ? gkFogParams::FM_QUAD :
			                                world->mistype == 1 ? gkFogParams::FM_LIN  :
			                                gkFogParams::FM_EXP;
			sprops.m_fog.m_start         = world->miststa;
			sprops.m_fog.m_end           = sprops.m_fog.m_start + world->mistdist;
			sprops.m_fog.m_intensity     = world->misi;
			sprops.m_fog.m_color         = props.m_horizon;

			if ((world->skytype & WO_SKYBLEND))
			{
				gkVector3 a(world->horr, world->horg, world->horb), b(world->zenr, world->zeng, world->zenb);
				gkVector3 c = gkMathUtils::interp(a, b, 0.5);

				sprops.m_fog.m_color.r = c.x;
				sprops.m_fog.m_color.g = c.y;
				sprops.m_fog.m_color.b = c.z;
			}
		}
	}
}



void gkBlenderSceneConverter::convertObjectGroup(gkGameObjectGroup* gobj, Blender::Object* bobj)
{
	if (!bobj)
		return;

	for (Blender::GroupObject* group = (Blender::GroupObject*)bobj->dup_group->gobject.first; group;
	        group = (Blender::GroupObject*)group->next)
	{
		Blender::Object* gob = group->ob;
		if (gob->transflag& OB_DUPLIGROUP && gob->dup_group != 0)
			convertObjectGroup(gobj, gob);
		else
		{
			gkGameObject* oth = m_gscene->getObject(GKB_IDNAME(gob));
			if (oth)
				gobj->addObject(oth);
		}
	}

}


void gkBlenderSceneConverter::convertGroups(utArray<Blender::Object*> &groups)
{
	gkGroupManager* mgr = gkGroupManager::getSingletonPtr();

	// This is a complete list of groups & containing objects.
	// The gkGameObjectGroup is a containter, the gkGameObjectGroupInstance
	// is where the object should be added / removed from the scene.
	
	//	for (Blender::Group* bgrp = (Blender::Group*)m_file->_getInternalFile()->m_group.first; bgrp != 0; 
	//	bgrp = (Blender::Group*)bgrp->id.next)

	gkBlendListIterator iter = m_file->_getInternalFile()->getGroupList();
	while (iter.hasMoreElements())
	{
		Blender::Group* bgrp = (Blender::Group*)iter.getNext();


		const gkResourceName groupName(GKB_IDNAME(bgrp), m_groupName);

		if (mgr->exists(groupName))
		{
			// Can most likely assert here
			continue;
		}

		gkGameObjectGroup* group = (gkGameObjectGroup*)mgr->create(groupName);


		for (Blender::GroupObject* bgobj = (Blender::GroupObject*)bgrp->gobject.first; bgobj; bgobj = bgobj->next)
		{
			if (bgobj->ob)
			{
				Blender::Object* bobj = bgobj->ob;

				if (!validObject(bobj))
					continue;

				gkGameObject* gobj = m_gscene->getObject(GKB_IDNAME(bobj));

				// add it to the list
				if (gobj)
					group->addObject(gobj);
			}
		}

		// Destroy if empty
		if (group->isEmpty())
			mgr->destroy(group);
		else
			mgr->attachGroupToScene(m_gscene, group);

	}

	// Process user created groups.
	utArray<Blender::Object*>::Iterator it = groups.iterator();
	while (it.hasMoreElements())
	{
		Blender::Object* bobj = it.getNext();


		// Should not fail
		GK_ASSERT((bobj->transflag& OB_DUPLIGROUP && bobj->dup_group != 0));


		// Owning group
		Blender::Group* bgobj = bobj->dup_group;
		const gkResourceName groupName(GKB_IDNAME(bgobj), m_groupName);


		if (mgr->exists(groupName))
		{
			gkGameObjectGroup* ggobj = (gkGameObjectGroup*)mgr->getByName(groupName);


			gkGameObjectInstance* inst = ggobj->createGroupInstance(m_gscene, gkResourceName(GKB_IDNAME(bobj), m_groupName));
			if (inst)
				convertObject(bobj, inst->getRoot());
		}
	}
}




void gkBlenderSceneConverter::convertObject(Blender::Object* bobj, gkGameObject* gobj)
{
	if (!m_gscene)
		return;

	GK_ASSERT(validObject(bobj));

	if (gobj == 0)
	{
		gkHashedString name(GKB_IDNAME(bobj));
		switch (bobj->type)
		{
		case OB_EMPTY:      gobj = m_gscene->createObject(name);    break;
		case OB_LAMP:       gobj = m_gscene->createLight(name);     break;
		case OB_CAMERA:     gobj = m_gscene->createCamera(name);    break;
		case OB_MESH:       gobj = m_gscene->createEntity(name);    break;
		case OB_ARMATURE:   gobj = m_gscene->createSkeleton(name);  break;
		}

	}


	// all game object property types
	if (gobj)
	{
		gkScalar animfps = m_bscene->r.frs_sec / m_bscene->r.frs_sec_base;
		
		convertObjectGeneral(gobj, bobj);
		convertObjectProperties(gobj, bobj);
		convertObjectPhysics(gobj, bobj);
		convertObjectConstraints(gobj, bobj);
		convertObjectLogic(gobj, bobj);
		convertObjectAnimations(gobj, bobj, animfps);

		// object data
		switch (bobj->type)
		{
		case OB_LAMP:       convertObjectLamp(gobj, bobj);        break;
		case OB_CAMERA:     convertObjectCamera(gobj, bobj);      break;
		case OB_MESH:       convertObjectMesh(gobj, bobj);        break;
		case OB_ARMATURE:   convertObjectArmature(gobj, bobj);    break;
		}

		convertObjectParticles(gobj, bobj); //need mesh info
	}
}




void gkBlenderSceneConverter::convertObjectGeneral(gkGameObject* gobj, Blender::Object* bobj)
{
	gkQuaternion quat;
	gkVector3 loc, scale;
	gkMatrix4 obmat = gkMathUtils::getFromFloat(bobj->obmat);


	gkMathUtils::extractTransform(obmat, loc, quat, scale);

	// prevent zero scale
	gkVector3 scaleTest = gkVector3(bobj->size[0], bobj->size[1], bobj->size[2]);
	if (scaleTest.isZeroLength())
		scale = gkVector3(1.f, 1.f, 1.f);

	gkGameObjectProperties& props = gobj->getProperties();

	if (bobj->parent && validObject(bobj->parent))
		props.m_parent = GKB_IDNAME(bobj->parent);


	props.m_transform = gkTransformState(loc, quat, scale);

	if (bobj->restrictflag & OB_RESTRICT_RENDER)
		props.m_mode |= GK_INVISIBLE;

	gobj->setActiveLayer((m_bscene->lay & bobj->lay) != 0);
	gobj->setLayer((UTuint32)bobj->lay);
}




void gkBlenderSceneConverter::convertObjectProperties(gkGameObject* gobj, Blender::Object* bobj)
{
	// Attach variables to object ( used in game logic )
	for (Blender::bProperty* prop = (Blender::bProperty*)bobj->prop.first; prop; prop = prop->next)
	{
		gkVariable* gop = 0;
		if (prop->type == GPROP_BOOL)
		{
			gop = gobj->createVariable(prop->name, prop->flag != 0);
			gop->setValue(prop->data != 0);
		}
		else if (prop->type == GPROP_INT)
		{
			gop = gobj->createVariable(prop->name, prop->flag != 0);
			gop->setValue((int)prop->data);
		}
		else if (prop->type == GPROP_FLOAT)
		{
			gop = gobj->createVariable(prop->name, prop->flag != 0);
			gop->setValue(*((gkScalar*)&prop->data));
		}
		else if (prop->type == GPROP_STRING)
		{
			gop = gobj->createVariable(prop->name, prop->flag != 0);
			gop->setValue(gkString((char*)prop->poin));
		}
		else if (prop->type == GPROP_TIME)
		{
			gop = gobj->createVariable(prop->name, prop->flag != 0);
			gop->setValue(*((gkScalar*)&prop->data));
		}

		if (gop) gop->makeDefault();
	}
}



void gkBlenderSceneConverter::convertObjectParticles(gkGameObject* gobj, Blender::Object* bobj)
{	
#ifdef OGREKIT_USE_PARTICLE
	for (Blender::ParticleSystem* ps = (Blender::ParticleSystem*)bobj->particlesystem.first; ps; ps = ps->next)
	{
		gkString name = ps->name;
		gkString pname = GKB_IDNAME(ps->part);

		gkParticleResource* resource = gkParticleManager::getSingleton().getByName<gkParticleResource>(gkResourceName(pname, m_groupName));
		if (!resource)
			continue;
		
		gkGameObjectProperties& gprops = gobj->getProperties();

		gkParticleObject* pobj = m_gscene->createParticleObject(gkUtils::getUniqueName(name));
		if (pobj)
		{
			pobj->setActiveLayer(gobj->isInActiveLayer());
			pobj->setLayer(gobj->getLayer());

			gkParticleSettingsProperties& sprops = resource->getParticleProperties();
			gkParticleSystemProperties& props = pobj->getParticleProperties();
			
			gprops.m_particleObjs.push_back(pobj->getName());

			props.m_name = name;
			props.m_seed = ps->seed;
			props.m_settings = pname;
						
			gkGameObjectProperties& gparticleprops = pobj->getProperties();
			gparticleprops.m_parent = gobj->getName();
			gparticleprops.m_transform = gobj->getProperties().m_transform;

			props.m_material = "<gkBuiltin/Halo>";

			if (!sprops.m_drawEmitter)
				gobj->getProperties().m_mode |= GK_INVISIBLE;
				
			Blender::Material* ma = BlenderMaterial(bobj, sprops.m_material);
			if (ma)
			{
				props.m_material = GKB_IDNAME(ma);
			}

			gkEntity* entity = gobj->getEntity();
			if (entity)
			{
				props.m_mesh = entity->getMesh();
				//pobj->setMaterialName(entity->getMesh()->getFirstMaterial().m_name);
			}
		}
	}
#endif
}

void gkBlenderSceneConverter::convertObjectConstraints(gkGameObject* gobj, Blender::Object* bobj)
{
	// TODO: add more constraints
	gkConstraintManager* mgr = m_gscene->getConstraintManager();

	for (Blender::bConstraint* bc = (Blender::bConstraint*)bobj->constraints.first; bc; bc = bc->next)
	{

		if (bc->type == CONSTRAINT_TYPE_RIGIDBODYJOINT)
		{

			Blender::bRigidBodyJointConstraint* jc = (Blender::bRigidBodyJointConstraint*)bc->data;

			gkPhysicsConstraintProperties p;
			p.m_target = GKB_IDNAME(jc->tar);
			p.m_axis  = gkVector3(jc->axX, jc->axY, jc->axZ);
			p.m_pivot = gkVector3(jc->pivX, jc->pivY, jc->pivZ);
			for (int i = 0; i < 6; i++)
			{
				p.m_minLimit[i] = jc->minLimit[i];
				p.m_maxLimit[i] = jc->maxLimit[i];
			}
			p.m_flag = jc->flag;
			p.m_disableLinkedCollision = (jc->flag & CONSTRAINT_DISABLE_LINKED_COLLISION) != 0;

			
			if (jc->type == CONSTRAINT_RB_BALL)
				p.m_type = GK_BALL_CONSTRAINT;
			else if (jc->type == CONSTRAINT_RB_HINGE)
				p.m_type = GK_HINGE_CONSTRAINT;			
			else if (jc->type == CONSTRAINT_RB_CONETWIST)			
				p.m_type = GK_CONETWIST_CONSTRAINT;			
			else if (jc->type == CONSTRAINT_RB_VEHICLE)			
				p.m_type = GK_VEHICLE_CONSTRAINT;			
			else if (jc->type == CONSTRAINT_RB_GENERIC6DOF)			
				p.m_type = GK_D6_CONSTRAINT;

			gobj->getProperties().m_physics.m_constraints.push_back(p);

		}
		else
		{
			if (bc->enforce == 0.0)
				continue;

			gkConstraint* co = 0;

			if (bc->type == CONSTRAINT_TYPE_ROTLIMIT)
			{
				// rotation is in radians.
				Blender::bRotLimitConstraint* lr = (Blender::bRotLimitConstraint*)bc->data;
				if (!lr->flag)
					continue;

				gkLimitRotConstraint* c = new gkLimitRotConstraint();
				co = c;

				if (lr->flag & LIMIT_XROT)
					c->setLimitX(gkVector2(lr->xmin * gkDPR, lr->xmax * gkDPR));
				if (lr->flag & LIMIT_YROT)
					c->setLimitY(gkVector2(lr->ymin * gkDPR, lr->ymax * gkDPR));
				if (lr->flag & LIMIT_ZROT)
					c->setLimitZ(gkVector2(lr->zmin * gkDPR, lr->zmax * gkDPR));
			}
			else if (bc->type == CONSTRAINT_TYPE_LOCLIMIT)
			{
				Blender::bLocLimitConstraint* ll = (Blender::bLocLimitConstraint*)bc->data;
				if (!ll->flag)
					continue;

				gkLimitLocConstraint* c = new gkLimitLocConstraint();
				co = c;


				if (ll->flag & LIMIT_XMIN) c->setMinX(ll->xmin);
				if (ll->flag & LIMIT_XMAX) c->setMaxX(ll->xmax);
				if (ll->flag & LIMIT_YMIN) c->setMinY(ll->ymin);
				if (ll->flag & LIMIT_YMAX) c->setMaxY(ll->ymax);
				if (ll->flag & LIMIT_ZMIN) c->setMinZ(ll->zmin);
				if (ll->flag & LIMIT_ZMAX) c->setMaxZ(ll->zmax);
			}

			if (co)
			{
				co->setSpace(bc->ownspace == CONSTRAINT_SPACE_LOCAL ? TRANSFORM_LOCAL : TRANSFORM_WORLD);
				co->setInfluence(bc->enforce);		
	
				mgr->addConstraint(gobj, co);
			}
		}
	}
}



void gkBlenderSceneConverter::convertObjectPhysics(gkGameObject* gobj, Blender::Object* bobj)
{
	gkGameObjectProperties&  props  = gobj->getProperties();
	gkPhysicsProperties&     phy    = props.m_physics;

	phy.m_type = GK_STATIC;
	switch (bobj->body_type)
	{
	case OB_BODY_TYPE_RIGID:        phy.m_type = GK_RIGID;          break;
	case OB_BODY_TYPE_DYNAMIC:      phy.m_type = GK_DYNAMIC;        break;
	case OB_BODY_TYPE_NO_COLLISION: phy.m_type = GK_NO_COLLISION;   break;
	}

	if (bobj->type != OB_MESH)
	{
		if (!(bobj->gameflag & OB_ACTOR))
			phy.m_type = GK_NO_COLLISION;
	}


	Blender::Object* parent = bobj->parent;
	while (parent && parent->parent) 
		parent = parent->parent;
	
	if (parent && (bobj->gameflag  & OB_CHILD) == 0)
		phy.m_type = GK_NO_COLLISION;


	if (!props.isPhysicsObject())
		return;

	if (bobj->gameflag & OB_ACTOR)
	{
		props.m_mode    |= GK_ACTOR;
		phy.m_mode      |= GK_CONTACT;
	}
	if (bobj->gameflag & OB_GHOST)
		props.m_mode |= GK_GHOST;
	if (bobj->gameflag & OB_OCCLUDER)
		props.m_mode |= GK_OCCLUDER;

	if (bobj->gameflag  & OB_CHILD)							phy.m_mode |= parent ? GK_COMPOUND_CHILD : GK_COMPOUND;
	if (bobj->gameflag  & OB_COLLISION_RESPONSE)            phy.m_mode |= GK_NO_SLEEP;
	if (bobj->gameflag2 & OB_LOCK_RIGID_BODY_X_AXIS)        phy.m_mode |= GK_LOCK_LINV_X;
	if (bobj->gameflag2 & OB_LOCK_RIGID_BODY_Y_AXIS)        phy.m_mode |= GK_LOCK_LINV_Y;
	if (bobj->gameflag2 & OB_LOCK_RIGID_BODY_Z_AXIS)        phy.m_mode |= GK_LOCK_LINV_Z;
	if (bobj->gameflag2 & OB_LOCK_RIGID_BODY_X_ROT_AXIS)    phy.m_mode |= GK_LOCK_ANGV_X;
	if (bobj->gameflag2 & OB_LOCK_RIGID_BODY_Y_ROT_AXIS)    phy.m_mode |= GK_LOCK_ANGV_Y;
	if (bobj->gameflag2 & OB_LOCK_RIGID_BODY_Z_ROT_AXIS)    phy.m_mode |= GK_LOCK_ANGV_Z;

	phy.m_minVel        = bobj->min_vel;
	phy.m_maxVel        = bobj->max_vel;
	phy.m_cpt           = bobj->m_contactProcessingThreshold;
	phy.m_linearDamp    = bobj->damping;
	phy.m_angularDamp   = bobj->rdamping;
	phy.m_mass          = bobj->mass;
	phy.m_radius        = bobj->inertia;
	phy.m_formFactor    = bobj->formfactor;
	phy.m_margin        = bobj->margin;

	if (bobj->type == OB_MESH)
	{
		Blender::Mesh* me = (Blender::Mesh*)bobj->data;

		if (me)
		{
			Blender::Material* ma = BlenderMaterial(bobj, 0);
			if (ma)
			{
				phy.m_restitution   = ma->reflect;
				phy.m_friction      = ma->friction;
			}

		}
	}

	if (phy.isRigidOrDynamic())
	{
		switch (bobj->boundtype)
		{
		case OB_BOUND_BOX:
			phy.m_shape = SH_BOX;
			break;
		case OB_BOUND_SPHERE:
			phy.m_shape = SH_SPHERE;
			break;
		case OB_BOUND_CONE:
			phy.m_shape = SH_CONE;
			break;
		case OB_BOUND_CYLINDER:
			phy.m_shape = SH_CYLINDER;
			break;
		case OB_BOUND_POLYT:
			phy.m_shape = SH_CONVEX_TRIMESH;
			break;
		case OB_BOUND_POLYH:
		case OB_BOUND_DYN_MESH:
			phy.m_shape = SH_GIMPACT_MESH;
			break;
		}
	}
	else
	{
		if (bobj->type == OB_MESH)
			phy.m_shape = SH_BVH_MESH;
		else
			phy.m_shape = SH_SPHERE;
	}

	// setup velocity constraints
	if (phy.isRigidOrDynamic())
	{
		if (phy.m_minVel > 0.f || phy.m_maxVel > 0.f)
		{
			gkLimitVelocityConstraint* vc = new gkLimitVelocityConstraint();
			vc->setLimit(gkVector2(phy.m_minVel, phy.m_maxVel));

			m_gscene->getConstraintManager()->addConstraint(gobj, vc);
		}
	}
}


void gkBlenderSceneConverter::convertObjectLogic(gkGameObject* gobj, Blender::Object* bobj)
{
	m_logic->convertObject(bobj, gobj);
}


void gkBlenderSceneConverter::convertObjectAnimations(gkGameObject* gobj, Blender::Object* bobj, gkScalar animfps)
{
	gkAnimationLoader anims(m_groupName);
	int version = m_file->_getInternalFile()->getVersion();

	anims.convertObject(gobj, bobj, version <= 249, animfps);
}


void gkBlenderSceneConverter::convertObjectCamera(gkGameObject* gobj, Blender::Object* bobj)
{
	GK_ASSERT(gobj->getType() == GK_CAMERA && bobj->data);
	gkCamera* obj = static_cast<gkCamera*>(gobj);

	Blender::Camera* camera = static_cast<Blender::Camera*>(bobj->data);

	gkCameraProperties& props = obj->getCameraProperties();

	if (camera->type == CAM_ORTHO)
		props.m_type    = gkCameraProperties::CA_ORTHOGRAPHIC;
	else
		props.m_type    = gkCameraProperties::CA_PERSPECTIVE;
	props.m_clipend     = camera->clipend;
	props.m_clipstart   = camera->clipsta;
	props.m_fov         = gkScalar(360) * gkMath::ATan(gkScalar(16) / camera->lens).valueRadians() / gkPi;
	props.m_orthoscale  = camera->ortho_scale;
	props.m_start       = m_bscene->camera == bobj;
}


void gkBlenderSceneConverter::convertObjectLamp(gkGameObject* gobj, Blender::Object* bobj)
{
	GK_ASSERT(gobj->getType() == GK_LIGHT && bobj->data);

	gkLight* obj = static_cast<gkLight*>(gobj);

	gkLightProperties& props = obj->getLightProperties();
	Blender::Lamp* la = static_cast<Blender::Lamp*>(bobj->data);

	props.m_diffuse = gkColor(la->r, la->g, la->b);
	if (la->mode & LA_NO_DIFF)
		props.m_diffuse = gkColor::Black;

	props.m_specular = gkColor(la->r, la->g, la->b);
	if (la->mode & LA_NO_SPEC)
		props.m_specular = gkColor::Black;

	props.m_power = la->energy;
	if (la->mode & LA_NEG)
		props.m_power = -props.m_power;

	props.m_linear    = la->att1 / la->dist;
	props.m_constant  = 1.f;
	props.m_quadratic = la->att2 / (la->dist * la->dist);


	props.m_type = gkLightProperties::LI_POINT;
	if (la->type != LA_LOCAL)
		props.m_type = la->type == LA_SPOT ? gkLightProperties::LI_SPOT : gkLightProperties::LI_DIR;

	
	props.m_spot.y  = la->spotsize > 128 ? 128 : la->spotsize;
	props.m_spot.x  = gkMax(gkRadian(la->spotblend).valueDegrees(), props.m_spot.y);
	props.m_falloff = 128.f * la->spotblend;

	props.m_casts = (la->type != LA_HEMI && 
		((la->mode & LA_SHAD_RAY) ||
		 (la->type == LA_SPOT && (la->mode & LA_SHAD_BUF))));		
}


void gkBlenderSceneConverter::convertObjectMesh(gkGameObject* gobj, Blender::Object* bobj)
{
	GK_ASSERT(gobj->getType() == GK_ENTITY && bobj->data);
	gkEntity* obj = static_cast<gkEntity*>(gobj);

	gkEntityProperties& props = obj->getEntityProperties();



	Blender::Mesh* me =  (Blender::Mesh*)bobj->data;


	// this is shared for faster conversion times
	// and better efficiency
	if (!m_gscene->hasMesh(GKB_IDNAME(me)))
	{
		props.m_mesh = m_gscene->createMesh(GKB_IDNAME(me));
		gkBlenderMeshConverter meconv(props.m_mesh, bobj, me);
		meconv.convert();
	}
	else
		props.m_mesh = m_gscene->getMesh(GKB_IDNAME(me));


	props.m_casts = gobj->getProperties().m_physics.isRigidOrDynamic() || !gobj->getProperties().isPhysicsObject();

	Blender::Material* matr = BlenderMaterial(bobj, 0);
	if (matr)
		props.m_casts = (matr->mode & MA_SHADBUF) != 0;

	// if it has an action save the initial pose / animation name
	if (bobj->parent)
	{
		Blender::Object* par = bobj->parent;
		Blender::bAction* act = par->action;
		if (!act && par->proxy_from)
			act = par->proxy_from->action;

		if (act) props.m_startPose = (GKB_IDNAME(act));
	}
}




void gkBlenderSceneConverter::convertObjectSkeleton(gkSkeletonResource* sobj, Blender::Object* bobj)
{
	GK_ASSERT(sobj && bobj->data);

	gkSkeletonConverter loader;


	loader.m_armature = static_cast<Blender::bArmature*>(bobj->data);
	loader.m_skeleton = sobj;


	// create bone lists && transforms
	Blender::Bone* bone = static_cast<Blender::Bone*>(loader.m_armature->bonebase.first);
	while (bone)
	{
		// only process root bones
		if (bone->parent == 0)
			loader.buildBoneTree(bone, NULL, NULL);
		bone = bone->next;
	}
}



void gkBlenderSceneConverter::convertObjectArmature(gkGameObject* gobj, Blender::Object* bobj)
{
	Blender::bArmature* armature = static_cast<Blender::bArmature*>(bobj->data);

	const gkResourceName skelName(GKB_IDNAME(bobj), m_groupName);


	if (gkSkeletonManager::getSingleton().exists(skelName))
	{
		gkSkeletonResource* resource = gkSkeletonManager::getSingleton().getByName<gkSkeletonResource>(skelName);
		static_cast<gkSkeleton*>(gobj)->_setInternalSkeleton(resource);
	}
	else
	{
		gkSkeletonResource* resource = gkSkeletonManager::getSingleton().create<gkSkeletonResource>(skelName);
		convertObjectSkeleton(resource, bobj);
		static_cast<gkSkeleton*>(gobj)->_setInternalSkeleton(resource);
	}
}


void gkBlenderSceneConverter::convert(void)
{
	if (m_gscene)
		return;

	m_gscene = (gkScene*)gkSceneManager::getSingleton().create(gkResourceName(GKB_IDNAME(m_bscene), m_groupName));
	if (!m_gscene)
	{
		gkPrintf("SceneConverter: duplicate scene '%s'\n", (m_bscene->id.name + 2));
		return;
	}

	m_gscene->setLoadBlendFile(m_file);


	if (m_bscene->world)
		convertWorld();


	convertSoundScene();


	m_gscene->setLayer((UTuint32)m_bscene->lay);


	utArray<Blender::Object*> groups, armatureLinker;
	for (Blender::Base* base = (Blender::Base*)m_bscene->base.first; base; base = base->next)
	{
		if (!base->object)
			continue;

		Blender::Object* bobj = base->object;

		// non - conversion object
		if (!validObject(bobj))
			continue;

		if (bobj->transflag& OB_DUPLIGROUP && bobj->dup_group != 0)
			groups.push_back(bobj);
		else
			convertObject(bobj);

		if (bobj->type == OB_MESH && bobj->parent != 0 && bobj->parent->type == OB_ARMATURE)
			armatureLinker.push_back(bobj);
	}

	// build group instances
	convertGroups(groups);


	if (!armatureLinker.empty())
	{
		gkMeshManager& memgr = gkMeshManager::getSingleton();
		gkSkeletonManager& skmgr = gkSkeletonManager::getSingleton();
		gkGameObjectManager& gomgr = gkGameObjectManager::getSingleton();


		UTsize i;
		for (i = 0; i < armatureLinker.size(); ++i)
		{
			Blender::Object* obMe = armatureLinker[i];
			Blender::Object* obAr = obMe->parent;


			gkEntity* gobjEn = gomgr.getEntity(GKB_IDNAME(obMe));
			gkSkeleton* gobjSk = gomgr.getSkeleton(GKB_IDNAME(obAr));

			if (gobjEn && gobjSk)
			{
				gobjEn->setSkeleton(gobjSk);


				// Link data
				Blender::Mesh* me = static_cast<Blender::Mesh*>(obMe->data);
				Blender::bArmature* ar = static_cast<Blender::bArmature*>(obAr->data);

				if (memgr.exists(GKB_IDNAME(me)) && skmgr.exists(GKB_IDNAME(obAr)))
				{
					gkSkeletonResource* skel = skmgr.getByName<gkSkeletonResource>(GKB_IDNAME(obAr));

					memgr.getByName<gkMesh>(GKB_IDNAME(me))->_setSkeleton(skel);

					gkBone::BoneList::Iterator roots = skel->getRootBoneList().iterator();

					while (roots.hasMoreElements())
					{
						gkBone* bone = roots.getNext();
						gkMatrix4 eobmat = gkMathUtils::getFromFloat(obMe->obmat);
						gkMatrix4 sobmat = gkMathUtils::getFromFloat(obAr->obmat);

						bone->applyRootTransform(eobmat.inverse() * sobmat);
					}

				}
			}
		}
	}

	m_logic->resolveLinks();
}
