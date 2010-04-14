/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C & Erwin Coumans.

    Contributor(s): Nestor Silveira.
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
#include "OgreLight.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonManager.h"

#include "gkObjectLoader.h"
#include "gkLight.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkSkeleton.h"
#include "gkLoaderUtils.h"
#include "gkMathUtils.h"
#include "gkBlenderDefines.h"
#include "blender.h"

#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "gkCharacter.h"
#include "gkAction.h"
#include "gkActionChannel.h"
#include "gkBlendFile.h"
#include "gkVariable.h"

#include "bBlenderFile.h"
#include "bMain.h"

#include "gkLimitLocConstraint.h"
#include "gkLimitRotConstraint.h"


#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"


using namespace Ogre;


gkGameObjectLoader::gkGameObjectLoader(gkBlendFile *fp, Blender::Scene* sc, Blender::Object *ob)
:       m_file(fp), m_object(ob), m_scene(sc)
{
    GK_ASSERT(m_file && m_object && m_scene);
}

gkGameObjectLoader::~gkGameObjectLoader()
{
}


// Load game object information
void gkGameObjectLoader::load(gkObject *baseClass)
{
    GK_ASSERT(m_file && m_object && baseClass);
    gkGameObject *ob = static_cast<gkGameObject*>(baseClass);



    gkQuaternion quat;
    gkVector3 loc, scale;
    gkMatrix4 obmat = gkMathUtils::getFromFloat(m_object->obmat);

    if (ob->isInstance())
    {
        // instances are an offset from the origional dupli group 
        gkLoaderUtils ut(m_file->getInternalFile());
        ut.extractInstanceTransform(ob, m_object, loc, quat, scale);
    }
    else
    {
        // will be the parent or world tranform
        if (!m_object->parent)
            gkMathUtils::extractTransform(obmat, loc, quat, scale);
        else
        {
            gkMatrix4 parent = gkMathUtils::getFromFloat(m_object->parent->obmat);

            obmat = parent.inverse() * obmat;
            gkMathUtils::extractTransform(obmat, loc, quat, scale);
        }
    }


    gkGameObjectProperties &props = ob->getProperties();

    props.position      = loc;
    props.orientation   = quat;
    props.scale         = scale;
		
	gkObject* object = ob->getAttachedObject();
	props.isStatic = object && object->getCollisionObject() && object->getCollisionObject()->isStaticObject();
	props.isGhost = object && object->getCollisionObject() && 
		(object->getCollisionObject()->getCollisionFlags() & btCollisionObject::CF_CHARACTER_OBJECT);

    switch (ob->getType())
    {
    case GK_CAMERA:
        setCamera(ob);
        break;
    case GK_ENTITY:
        setEntity(ob);
        break;
    case GK_LIGHT:
        setLight(ob);
        break;
    case GK_SKELETON:
        setSkeleton(ob);
        break;
    }


    setConstraints(ob);
    setProperties(ob);
}

void gkGameObjectLoader::setProperties(gkGameObject *ob)
{
    GK_ASSERT(ob && m_object);

    // Attach variables to object ( used in game logic )
    for (Blender::bProperty *prop = (Blender::bProperty*)m_object->prop.first; prop; prop = prop->next)
    {
        gkVariable *gop = 0;

        if (prop->type == GPROP_BOOL)
        {
            gop = ob->createVariable(prop->name, prop->flag != 0);
            gop->setValue(prop->data != 0);
        }
        else if (prop->type == GPROP_INT)
        {
            gop = ob->createVariable(prop->name, prop->flag != 0);
            gop->setValue((int)prop->data);
        }
        else if (prop->type == GPROP_FLOAT)
        {
            gop = ob->createVariable(prop->name, prop->flag != 0);
            gop->setValue(*((gkScalar*)&prop->data));
        }
        else if (prop->type == GPROP_STRING)
        {
            gop = ob->createVariable(prop->name, prop->flag != 0);
            gop->setValue(gkString((char*)prop->poin));
        }
        else if (prop->type == GPROP_TIME)
        {
            gop = ob->createVariable(prop->name, prop->flag != 0);
            gop->setValue(*((gkScalar*)&prop->data));
        }
    }
}



void gkGameObjectLoader::setConstraints(gkGameObject *ob)
{
    for (Blender::bConstraint *bc = (Blender::bConstraint*)m_object->constraints.first; bc; bc = bc->next)
    {
        if (bc->enforce == 0.0) continue;

        if (bc->type == CONSTRAINT_TYPE_ROTLIMIT)
        {
            Blender::bRotLimitConstraint *lr = (Blender::bRotLimitConstraint*)bc->data;
            if (!lr->flag)
                continue;


            gkLimitRotConstraint *c = new gkLimitRotConstraint();

            if (lr->flag & LIMIT_XROT)
                c->setLimitX(Vector2(lr->xmin, lr->xmax));
            if (lr->flag & LIMIT_YROT)
                c->setLimitY(Vector2(lr->ymin, lr->ymax));
            if (lr->flag & LIMIT_ZROT)
                c->setLimitZ(Vector2(lr->zmin, lr->zmax));

            c->setLocal(bc->ownspace == CONSTRAINT_SPACE_LOCAL);

            c->setInfluence(bc->enforce);
            ob->addConstraint(c);
        }

        else if (bc->type == CONSTRAINT_TYPE_LOCLIMIT)
        {
            Blender::bLocLimitConstraint *ll = (Blender::bLocLimitConstraint*)bc->data;
            if (!ll->flag)
                continue;

            gkLimitLocConstraint *c = new gkLimitLocConstraint();

            if (ll->flag & LIMIT_XMIN) c->setMinX(ll->xmin);
            if (ll->flag & LIMIT_XMAX) c->setMaxX(ll->xmax);
            if (ll->flag & LIMIT_YMIN) c->setMinY(ll->ymin);
            if (ll->flag & LIMIT_YMAX) c->setMaxY(ll->ymax);
            if (ll->flag & LIMIT_ZMIN) c->setMinZ(ll->zmin);
            if (ll->flag & LIMIT_ZMAX) c->setMaxZ(ll->zmax);

            c->setLocal(bc->ownspace == CONSTRAINT_SPACE_LOCAL);
            c->setInfluence(bc->enforce);
            ob->addConstraint(c);
        }
    }
}

// Set light serialization data
void gkGameObjectLoader::setLight(gkGameObject *ob)
{
    GK_ASSERT(ob->getType() == GK_LIGHT && m_object->data);
    gkLight *obj = static_cast<gkLight*>(ob);

    gkLightProperties &props = obj->getLightProperties();
    Blender::Lamp* la = static_cast<Blender::Lamp*>(m_object->data);

    props.diffuse = ColourValue(la->r, la->g, la->b);
    if (la->mode & LA_NO_DIFF)
        props.diffuse = ColourValue::Black;

    props.specular = ColourValue(la->r, la->g, la->b);
    if (la->mode & LA_NO_SPEC)
        props.specular = ColourValue::Black;

    props.power = la->energy;
    if (la->mode & LA_NEG)
        props.power = -props.power;

    props.linear = la->att1 / la->dist;
    props.constant = 1.f;
    props.quadratic = la->att2 / (la->dist * la->dist);

    props.type = Light::LT_POINT;
    if (la->type != LA_LOCAL)
        props.type = la->type == LA_SPOT ? Light::LT_SPOTLIGHT : Light::LT_DIRECTIONAL;

    props.casts = true;
    props.spot_inner = gkRadian(la->spotblend).valueDegrees();
    props.spot_outer = la->spotsize > 128 ? 128 : la->spotsize;
    props.falloff =   128.f * la->spotblend;
    if (props.spot_inner > props.spot_outer) props.spot_inner = props.spot_outer;
}

// Set camera serialization data
void gkGameObjectLoader::setCamera(gkGameObject *ob)
{
    GK_ASSERT(ob->getType() == GK_CAMERA && m_object->data);
    gkCamera *obj = static_cast<gkCamera*>(ob);

    Blender::Camera* camera = static_cast<Blender::Camera*>(m_object->data);

    gkCameraProperties &props = obj->getCameraProperties();
    props.clipend   = camera->clipend;
    props.clipstart = camera->clipsta;
    props.fov       = gkScalar(360) * gkMath::ATan(gkScalar(16) / camera->lens).valueRadians() / gkPi;
    props.start     = m_scene->camera == m_object;
}


// Set mesh serialization data
void gkGameObjectLoader::setEntity(gkGameObject *ob)
{
    GK_ASSERT(ob->getType() == GK_ENTITY && m_object->data);
    gkEntity *obj = static_cast<gkEntity*>(ob);

    gkEntityProperties& props = obj->getEntityProperties();


    // source is the mesh name, here it's
    // the same name as the entity
    props.source = GKB_IDNAME(m_object);
    props.casts = !(ob->getProperties().isStatic || ob->getProperties().isGhost);

    gkLoaderUtils loader(m_file->getInternalFile());

    Blender::Material *matr = loader.getMaterial(m_object, 0);
    if (matr && props.casts)
        props.casts = (matr->mode & MA_SHADBUF) != 0;


    // if it has an action save the initial pose / animation name
    if (m_object->parent)
    {
        Blender::Object *par = m_object->parent;
        Blender::bAction *act = par->action;
        if (!act && par->proxy_from)
            act = par->proxy_from->action;

        if (act) ob->setStartPose(GKB_IDNAME(act));
    }
}


// Set skeleton serialization data
void gkGameObjectLoader::setSkeleton(gkGameObject *ob)
{
    GK_ASSERT(ob->getType() == GK_SKELETON && m_object->data);
    gkSkeleton *obj = static_cast<gkSkeleton*>(ob);

    // client skeleton should be here
    Ogre::SkeletonPtr skel = Ogre::SkeletonManager::getSingleton().getByName(obj->getName());


    if (!skel.isNull())
    {
        // ensure loaded
        skel->load();


        Ogre::Skeleton::BoneIterator bit = skel->getBoneIterator();
        while (bit.hasMoreElements())
        {
            Ogre::Bone *bone = bit.getNext();
            obj->createBone(bone);
        }
    }

    // create actions if needed
    bParse::bBlenderFile *bf = m_file->getInternalFile();
    bParse::bMain *mp = bf->getMain();

    // for all actions convert
    bParse::bListBasePtr *lbp = mp->getAction();

    for (int i = 0; i < lbp->size(); ++i)
    {
        Blender::bAction *bact = (Blender::bAction*)lbp->at(i);


        // find ownership
        Blender::bActionChannel* bac = (Blender::bActionChannel*)bact->chanbase.first;

        if (obj->hasAction(GKB_IDNAME(bact)))
        {
            // persistent
            continue;
        }

        gkAction *act = obj->createAction(GKB_IDNAME(bact));

        // min/max
        gkVector2 range(FLT_MAX, -FLT_MAX);

        while (bac)
        {
            if (obj->hasBone(bac->name))
            {
                gkBone *bone = obj->getBone(bac->name);

                gkActionChannel *achan = new gkActionChannel(act, bone);
                act->addChannel(achan);
                if (bac->ipo)
                {
                    Blender::IpoCurve* icu = (Blender::IpoCurve*)bac->ipo->curve.first;
                    while (icu)
                    {
                        if (icu->bezt)
                        {
                            int code = -1;
                            switch (icu->adrcode)
                            {
                            case AC_QUAT_W: { code = SC_ROT_W;  break; }
                            case AC_QUAT_X: { code = SC_ROT_X;  break; }
                            case AC_QUAT_Y: { code = SC_ROT_Y;  break; }
                            case AC_QUAT_Z: { code = SC_ROT_Z;  break; }
                            case AC_LOC_X:  { code = SC_LOC_X;  break; }
                            case AC_LOC_Y:  { code = SC_LOC_Y;  break; }
                            case AC_LOC_Z:  { code = SC_LOC_Z;  break; }
                            case AC_SIZE_X: { code = SC_SCL_X;  break; }
                            case AC_SIZE_Y: { code = SC_SCL_Y;  break; }
                            case AC_SIZE_Z: { code = SC_SCL_Z;  break; }
                            }


                            // ignore any other codes
                            if (code != -1)
                            {
                                gkBezierSpline *spline = new gkBezierSpline(code);

                                switch (icu->ipo)
                                {
                                case 0://BEZT_IPO_CONST:
                                    spline->setInterpolationMethod(gkBezierSpline::BEZ_CONSTANT);
                                    break;
                                case 1://BEZT_IPO_LIN:
                                    spline->setInterpolationMethod(gkBezierSpline::BEZ_LINEAR);
                                    break;
                                case 2://BEZT_IPO_BEZ:
                                    spline->setInterpolationMethod(gkBezierSpline::BEZ_CUBIC);
                                    break;
                                }

                                Blender::BezTriple *bezt = icu->bezt;
                                for (int c = 0; c < icu->totvert; c++, bezt++)
                                {
                                    gkBezierVertex v;

                                    v.h1[0] = bezt->vec[0][0];
                                    v.h1[1] = bezt->vec[0][1];
                                    v.cp[0] = bezt->vec[1][0];
                                    v.cp[1] = bezt->vec[1][1];
                                    v.h2[0] = bezt->vec[2][0];
                                    v.h2[1] = bezt->vec[2][1];


                                    // calculate global time
                                    if (range.x > v.cp[0]) range.x = v.cp[0];
                                    if (range.y < v.cp[0]) range.y = v.cp[0];
                                    spline->addVertex(v);
                                }
                                if (spline->getNumVerts())
                                    achan->addSpline(spline);
                                else delete spline;
                            }
                        }
                        icu = icu->next;
                    }
                }
            }
            bac = bac->next;
        }

        // apply time range
        act->setStart(range.x);
        act->setEnd(range.y);

    }
}


// Rigid body loader to support dynamic loading
// and unloading at runtime of physics bodies
gkRigidBodyLoader::gkRigidBodyLoader(gkBlendFile *fp,
                                     Blender::Scene* sc,
                                     Blender::Object *ob,
                                     gkGameObject *obj)
:       m_file(fp), m_object(ob), m_scene(sc), m_gameObj(obj),
        m_triMesh(0)
{
}


gkRigidBodyLoader::~gkRigidBodyLoader()
{
    freeResources();
}

void gkRigidBodyLoader::freeResources(void)
{
    if (m_triMesh)
        delete m_triMesh;

    m_triMesh = 0;

    for (UTsize i = 0; i < m_shapes.size(); ++i)
        delete m_shapes[i];
    m_shapes.clear();
}


void gkRigidBodyLoader::load(gkObject *ob)
{
    // start clean
    freeResources();

    gkRigidBody *rigid = static_cast<gkRigidBody*>(ob);

    btVector3 size = btVector3(1, 1, 1);
    btVector3 pos = btVector3(0, 0, 0);

    gkLoaderUtils ut(m_file->getInternalFile());
    Blender::Material *mat = ut.getMaterial(m_object, 0);

    if (m_object->type != OB_MESH)
    {
        // actor is for near & radar, so this needs updated
        if (!(m_object->gameflag & OB_ACTOR))
            return;

        // use radius for bounding scale ?
        size *= m_object->inertia;
    }
    else
    {
        Blender::Mesh *me = (Blender::Mesh*)m_object->data;

        btVector3 v0, v1, v2, v3;

        btVector3 minV(1e30f, 1e30f, 1e30f), maxV(-1e30f, -1e30f, -1e30f);
        m_triMesh = new btTriangleMesh();

        Blender::MFace *mf = me->mface;
        Blender::MVert *mv = me->mvert;
        for (int i = 0; i < me->totface && mf && mv; ++i, ++mf)
        {
            // no face just an edge
            if (!mf->v3) continue;

            v0 = btVector3(mv[mf->v1].co.x, mv[mf->v1].co.y, mv[mf->v1].co.z);
            v1 = btVector3(mv[mf->v2].co.x, mv[mf->v2].co.y, mv[mf->v2].co.z);
            v2 = btVector3(mv[mf->v3].co.x, mv[mf->v3].co.y, mv[mf->v3].co.z);

            minV.setMin(v0); maxV.setMax(v0);
            minV.setMin(v1); maxV.setMax(v1);
            minV.setMin(v2); maxV.setMax(v2);

            m_triMesh->addTriangle(v0, v1, v2);

            if (mf->v4)
            {
                v3 = btVector3(mv[mf->v4].co.x, mv[mf->v4].co.y, mv[mf->v4].co.z);
                minV.setMin(v3); maxV.setMax(v3);

                m_triMesh->addTriangle(v0, v3, v2);
            }
        }

        if (!m_triMesh->getNumTriangles())
        {
            delete m_triMesh;
            m_triMesh = 0;
        }

        size = (maxV - minV) * 0.5f;
        pos  = (minV + maxV) * 0.5f;
    }



    btVector3 localPos(pos.x(), pos.y(), pos.z());
    btVector3 localSize(size.x(), size.y(), size.z());
    btTransform worldTrans;
    worldTrans.setIdentity();
    btVector3 scale;

    if (m_gameObj->isInstance())
    {
        // instances are an offset from the origional dupli group 
        gkLoaderUtils ut(m_file->getInternalFile());
        gkVector3 loc, size;
        gkQuaternion quat;
        ut.extractInstanceTransform(m_gameObj, m_object, loc, quat, size);

        worldTrans.setOrigin(btVector3(loc.x, loc.y, loc.z));
        worldTrans.getBasis().setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));
        scale = btVector3(size.x, size.y, size.z);

    }
    else
    {
        worldTrans.setOrigin(btVector3(m_object->loc.x, m_object->loc.y, m_object->loc.z));
        worldTrans.getBasis().setEulerZYX(m_object->rot.x, m_object->rot.y, m_object->rot.z);
        scale = btVector3(m_object->size.x, m_object->size.y, m_object->size.z);
    }

	GK_ASSERT(!(m_object->gameflag & OB_GHOST));

    if ((m_object->gameflag & OB_RIGID_BODY) || (m_object->gameflag & OB_DYNAMIC))
    {
        btCollisionShape *colShape = 0;

        switch (m_object->boundtype)
        {
        case OB_BOUND_BOX:
            {
                colShape = new btBoxShape(localSize);
                break;
            }
        case OB_BOUND_CYLINDER:
            {
                colShape = new btCylinderShapeZ(localSize);
                break;
            }
        case OB_BOUND_CONE:
            {
                btScalar radius = btMax(localSize[0], localSize[1]);
                btScalar height = 2.f * localSize[2];
                colShape = new btConeShapeZ(radius, height);
                break;
            }
        case OB_BOUND_POLYT:
            {
                if (m_triMesh)
                {
                    //better to approximate it, using btShapeHull
                    colShape = new btConvexTriangleMeshShape(m_triMesh);
                    break;
                }

                // fall to sphere
            }
        case OB_BOUND_POLYH:
        case OB_BOUND_DYN_MESH:
            {
                if (m_triMesh)
                {
                    btGImpactMeshShape* gimpact = new btGImpactMeshShape(m_triMesh);
                    gimpact->postUpdate();
                    colShape = gimpact;
                    break;
                }
                // fall to sphere
            }
        case OB_BOUND_SPHERE:
            {
                btScalar radius = localSize[localSize.maxAxis()];
                colShape = new btSphereShape(radius);
                break;
            }
        default:
            {

            }
        };

        if (colShape)
        {
            m_shapes.push_back(colShape);

            colShape->setMargin(m_object->margin);
            colShape->setLocalScaling(scale);
            btVector3 inertia;
            colShape->calculateLocalInertia(m_object->mass, inertia);

            btRigidBody* body = new btRigidBody(m_object->mass, 0, colShape, inertia);
            if (!(m_object->gameflag & OB_RIGID_BODY))
                body->setAngularFactor(0.f);
            if (mat)
            {
                rigid->setSensorMaterial(GKB_IDNAME(mat));
                body->setFriction(mat->friction);
                body->setRestitution(mat->reflect);
            }

            body->setContactProcessingThreshold(m_object->m_contactProcessingThreshold);

            int flags = rigid->getFlags();
            if (m_object->gameflag2 & OB_LOCK_RIGID_BODY_X_AXIS)
                flags |= gkRigidBody::RBF_LIMIT_LVEL_X;
            if (m_object->gameflag2 & OB_LOCK_RIGID_BODY_Y_AXIS)
                flags |= gkRigidBody::RBF_LIMIT_LVEL_Y;
            if (m_object->gameflag2 & OB_LOCK_RIGID_BODY_Z_AXIS)
                flags |= gkRigidBody::RBF_LIMIT_LVEL_Z;
            if (m_object->gameflag2 & OB_LOCK_RIGID_BODY_X_ROT_AXIS)
                flags |= gkRigidBody::RBF_LIMIT_AVEL_X;
            if (m_object->gameflag2 & OB_LOCK_RIGID_BODY_Y_ROT_AXIS)
                flags |= gkRigidBody::RBF_LIMIT_AVEL_Y;
            if (m_object->gameflag2 & OB_LOCK_RIGID_BODY_Z_ROT_AXIS)
                flags |= gkRigidBody::RBF_LIMIT_AVEL_Z;

            rigid->setFlags(flags);
            body->setDamping(m_object->damping, m_object->rdamping);
            body->setWorldTransform(worldTrans);
            rigid->_reinstanceBody(body);
        }
    }
    else
    {
        if (m_triMesh)
        {
            btCollisionShape* colShape = 0;
            if (m_triMesh->getNumTriangles() > 0)
            {
                btBvhTriangleMeshShape* childShape = new btBvhTriangleMeshShape(m_triMesh, true);
                m_shapes.push_back(childShape);


                /*if (scale[0] != 1. || scale[1] != 1. || scale[2] != 1.)
                {
                    colShape = new btScaledBvhTriangleMeshShape(childShape, scale);
                    m_shapes.push_back(colShape);
                }
                else*/
                    colShape = childShape;

                colShape->setMargin(m_object->margin);
				colShape->setLocalScaling(scale);

                btVector3 inertia(0, 0, 0);
                btRigidBody* colObj = new btRigidBody(0.f, 0, colShape, inertia);
                colObj->setWorldTransform(worldTrans);
                colObj->setCollisionShape(colShape);
                if (mat)
                {
                    rigid->setSensorMaterial(GKB_IDNAME(mat));
                    colObj->setFriction(mat->friction);
                    colObj->setRestitution(mat->reflect);
                }

                rigid->_reinstanceBody(colObj);
            }
            else
            {
                delete m_triMesh;
                m_triMesh = 0;
            }

        }
    }
}

////////////////////////////////////////////
gkCharacterLoader::gkCharacterLoader(gkBlendFile *fp,
                                     Blender::Scene* sc,
                                     Blender::Object *ob,
                                     gkGameObject *obj)
:       m_file(fp), m_object(ob), m_scene(sc), m_gameObj(obj),
        m_triMesh(0)
{
}


gkCharacterLoader::~gkCharacterLoader()
{
    freeResources();
}

void gkCharacterLoader::freeResources(void)
{
    if (m_triMesh)
        delete m_triMesh;

    m_triMesh = 0;

    for (UTsize i = 0; i < m_shapes.size(); ++i)
        delete m_shapes[i];
    m_shapes.clear();
}


void gkCharacterLoader::load(gkObject *ob)
{
    // start clean
    freeResources();

    btVector3 size = btVector3(1, 1, 1);
    btVector3 pos = btVector3(0, 0, 0);

    gkLoaderUtils ut(m_file->getInternalFile());
    Blender::Material *mat = ut.getMaterial(m_object, 0);

    if (m_object->type != OB_MESH)
    {
        // actor is for near & radar, so this needs updated
        if (!(m_object->gameflag & OB_ACTOR))
            return;

        // use radius for bounding scale ?
        size *= m_object->inertia;
    }
    else
    {
        Blender::Mesh *me = (Blender::Mesh*)m_object->data;

        btVector3 v0, v1, v2, v3;

        btVector3 minV(1e30f, 1e30f, 1e30f), maxV(-1e30f, -1e30f, -1e30f);
        m_triMesh = new btTriangleMesh();

        Blender::MFace *mf = me->mface;
        Blender::MVert *mv = me->mvert;
        for (int i = 0; i < me->totface && mf && mv; ++i, ++mf)
        {
            // no face just an edge
            if (!mf->v3) continue;

            v0 = btVector3(mv[mf->v1].co.x, mv[mf->v1].co.y, mv[mf->v1].co.z);
            v1 = btVector3(mv[mf->v2].co.x, mv[mf->v2].co.y, mv[mf->v2].co.z);
            v2 = btVector3(mv[mf->v3].co.x, mv[mf->v3].co.y, mv[mf->v3].co.z);

            minV.setMin(v0); maxV.setMax(v0);
            minV.setMin(v1); maxV.setMax(v1);
            minV.setMin(v2); maxV.setMax(v2);

            m_triMesh->addTriangle(v0, v1, v2);

            if (mf->v4)
            {
                v3 = btVector3(mv[mf->v4].co.x, mv[mf->v4].co.y, mv[mf->v4].co.z);
                minV.setMin(v3); maxV.setMax(v3);

                m_triMesh->addTriangle(v0, v3, v2);
            }
        }

        if (!m_triMesh->getNumTriangles())
        {
            delete m_triMesh;
            m_triMesh = 0;
        }

        size = (maxV - minV) * 0.5f;
        pos  = (minV + maxV) * 0.5f;
    }


    btVector3 localPos(pos.x(), pos.y(), pos.z());
    btVector3 localSize(size.x(), size.y(), size.z());

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(m_object->loc.x, m_object->loc.y, m_object->loc.z));

    worldTrans.getBasis().setEulerZYX(m_object->rot.x, m_object->rot.y, m_object->rot.z);
    btVector3 scale(m_object->size.x, m_object->size.y, m_object->size.z);

    GK_ASSERT (m_object->gameflag & OB_GHOST);

    btCollisionShape *colShape = 0;

    switch (m_object->boundtype)
    {
    case OB_BOUND_BOX:
        {
            colShape = new btBoxShape(localSize);
            break;
        }
    case OB_BOUND_CYLINDER:
        {
            colShape = new btCylinderShapeZ(localSize);
            break;
        }
    case OB_BOUND_CONE:
        {
            btScalar radius = btMax(localSize[0], localSize[1]);
            btScalar height = 2.f * localSize[2];
            colShape = new btConeShapeZ(radius, height);
            break;
        }
    case OB_BOUND_POLYT:
        {
            if (m_triMesh)
            {
                //better to approximate it, using btShapeHull
                colShape = new btConvexTriangleMeshShape(m_triMesh);
                break;
            }

            // fall to sphere
        }
    case OB_BOUND_POLYH:
    case OB_BOUND_DYN_MESH:
        {
            if (m_triMesh)
            {
                btGImpactMeshShape* gimpact = new btGImpactMeshShape(m_triMesh);
                gimpact->postUpdate();
                colShape = gimpact;
                break;
            }
            // fall to sphere
        }
    case OB_BOUND_SPHERE:
        {
            btScalar radius = localSize[localSize.maxAxis()];
            colShape = new btSphereShape(radius);
            break;
        }
    default:
        {
			GK_ASSERT(false);
        }
    };

    if (colShape)
    {
        m_shapes.push_back(colShape);

        colShape->setMargin(m_object->margin);
        colShape->setLocalScaling(scale);

		btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();

		ghostObject->setCollisionShape(colShape);

        ghostObject->setWorldTransform(worldTrans);

	    gkCharacter* character = static_cast<gkCharacter*>(ob);

        character->_reinstanceCharacter(ghostObject);
    }
}
