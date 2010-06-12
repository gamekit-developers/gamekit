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
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreTexture.h"
#include "OgreTextureManager.h"
#include "OgreImage.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"

#include "blender.h"
#include "bBlenderFile.h"
#include "bMain.h"

#include "gkBlenderDefines.h"
#include "gkBlenderSceneConverter.h"
#include "OgreKit.h"


#define VEC3CPY(a, b) {a.x= b.x; a.y= b.y; a.z= b.z;}
#define VEC2CPY(a, b) {a.x= b.x; a.y= b.y;}

// float normal from short
#define VEC3CPN(a, b) {a.x= (b[0]/32767.f); a.y= (b[1]/32767.f); a.z= (b[2]/32767.f);}
#define BlenderMaterial(ob, i) ((ob && ob->mat && i <= ob->totcol) ? ob->mat[gkClamp<int>(i, 0, 16)] : 0)
#define PtrSaftyCheck(x) (x != 0)

// simple skybox creator 
class gkSkyBoxGradient : public Ogre::ManualResourceLoader
{
protected:

	Blender::World *m_world;

	void fill(Ogre::Image &ima, int size, const Ogre::ColourValue &v, const Ogre::PixelFormat &fmt);
	void grad(Ogre::Image &ima, int x1, int y1, int x2, int y2, int size, 
		const Ogre::ColourValue &s, const Ogre::ColourValue &e, const Ogre::PixelFormat &fmt);

public:
	gkSkyBoxGradient(Blender::World *wo);
	virtual ~gkSkyBoxGradient();

	void loadResource(Ogre::Resource* resource);
};

// Convert Blender's logic brick system.
class gkLogicLoader
{
protected:

    struct ResolveObject
    {
        gkLogicSensor *lsens;
        gkLogicController *lcont;
        Blender::bController *cont;
        Blender::bActuator *act;
    };

    typedef utList<ResolveObject>   ResolveObjectList;
    typedef utList<gkLogicLink *>    CreatedList;

    ResolveObjectList               m_missing;
    CreatedList                     m_createdLinks;

public:
    gkLogicLoader();
    ~gkLogicLoader();

    void convertObject(Blender::Object *bobj, gkGameObject *gobj);
    void resolveLinks(void);

    static int getKey(int kc);
};



class gkBlenderMeshConverter
{
public:

    gkBlenderMeshConverter(gkBlendFile *fp, gkMesh *gmesh, Blender::Object *bobject, Blender::Mesh *bmesh);
    ~gkBlenderMeshConverter();

    void convert(void);

private:

    typedef utArray<int>                                AssignmentIndexList;
    typedef utArray<gkDeformVertex>                     AssignmentList;
    typedef utHashTable<utIntHashKey, AssignmentList>   AssignmentListMap;

    void convertBoneAssignments(int dgi, AssignmentListMap &dest);
    void assignBoneAssignments(gkSubMesh *me, AssignmentListMap &dest);
    void findWeight(int i, gkSubMesh *me, AssignmentListMap &dest);


    struct PackedFace
    {
        Blender::MDeformVert    *dverts;
        Blender::MVert          *verts;
        gkVector2               uvLayers[8][4];
        int                     totlay;
        unsigned int            *colors;
        unsigned int            *index;
    };


    struct TempFace
    {
        gkVertex v0, v1, v2;
        unsigned int i0, i1, i2;
    };


    void convertIndexedTriangle( TempFace *dest,
                                 unsigned int v0,
                                 unsigned int v1,
                                 unsigned int v2,
                                 PackedFace &face
                               );

    void calcNormal(TempFace *tri);

    void convertMaterial(Blender::Material *bma, gkMaterialProperties &gma, class gkMeshHashKey &hk);
    void convertTextureFace(gkMaterialProperties &gma, class gkMeshHashKey &hk, Blender::Image **imas);

    int findTextureLayer(Blender::MTex *te);


    unsigned int packColour(Blender::MCol col, bool opengl);


    gkMesh          *m_gmesh;
    Blender::Mesh   *m_bmesh;
    gkBlendFile     *m_file;
    Blender::Object *m_bobj;
};



static Blender::Material* gkLoaderUtils_getMaterial(Blender::Object *ob, int index)
{
    if (!ob || ob->totcol == 0) return 0;

    index = gkClamp<int>(index, 1, ob->totcol);
    Blender::Material* ma;

    if (ob->colbits & (1 << (index - 1)))
        ma = (Blender::Material*)ob->mat[index-1];
    else
    {
        Blender::Mesh *me = (Blender::Mesh*)ob->data;
        ma = (Blender::Material*)me->mat[index-1];
    }
    return ma;
}


static void gkLoaderUtils_getLayers(
        bParse::bBlenderFile *fp, 
        Blender::Mesh *mesh, 
        Blender::MTFace **eightLayerArray, 
        Blender::MCol **oneMCol, 
        int &validLayers)
{
    GK_ASSERT(mesh);

    bParse::bMain *mp = fp->getMain();
    GK_ASSERT(mp);

    validLayers = 0;

    Blender::CustomDataLayer *layers = (Blender::CustomDataLayer*)mesh->fdata.layers;
    if (layers)
    {
        // push valid layers
        for (int i = 0; i < mesh->fdata.totlayer && validLayers < 8; i++)
        {
            if (layers[i].type == CD_MTFACE && eightLayerArray)
            {
                Blender::MTFace *mtf = (Blender::MTFace *)layers[i].data;
                if (mtf)
                    eightLayerArray[validLayers++] = mtf;
            }
            else if (layers[i].type == CD_MCOL)
            {
                if (oneMCol && !(*oneMCol))
                    *oneMCol = static_cast<Blender::MCol*>(layers[i].data);
            }
        }
    }
    else
    {
        if (eightLayerArray && mesh->mtface)
            eightLayerArray[validLayers++] = mesh->mtface;
        if (oneMCol && mesh->mcol)
            *oneMCol = mesh->mcol;


    }
}


// ----------------------------------------------------------------------------
gkBlenderSceneConverter::gkBlenderSceneConverter(gkBlendFile *fp, Blender::Scene *sc)
    :   m_bscene(sc), m_gscene(0), m_file(fp)
{
    m_logic = new gkLogicLoader();
}

// ----------------------------------------------------------------------------
gkBlenderSceneConverter::~gkBlenderSceneConverter()
{
    delete m_logic;
}


// ----------------------------------------------------------------------------
bool gkBlenderSceneConverter::validObject(Blender::Object *bobj)
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


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::applyParents(utArray<Blender::Object *> &children)
{
    UTsize i;
    for (i=0; i<children.size(); i++)
    {
        Blender::Object *bchild = children.at(i);
        gkGameObject    *gchild = m_gscene->getObject(GKB_IDNAME(bchild));

        if (gchild)
        {
            gkGameObject *gpar = m_gscene->getObject(GKB_IDNAME(bchild->parent));
            if (gpar)
                gchild->setParent(gpar);
        }
    }
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertWorld(void)
{
    if (!m_gscene)
        return;

    gkSceneProperties &props = m_gscene->getProperties();

    if (m_bscene->world)
    {
        Blender::World *world = m_bscene->world;

        props.m_gravity     = gkVector3(0.f, 0.f, -world->gravity);
        props.m_ambient.r   = world->ambr;
        props.m_ambient.g   = world->ambg;
        props.m_ambient.b   = world->ambb;
        props.m_world.r     = world->horr;
        props.m_world.g     = world->horg;
        props.m_world.b     = world->horb;

        if ((world->skytype & WO_SKYBLEND) || (world->skytype & WO_SKYREAL))
        {
            // multi stop gradients are not implemented for now
            // so WO_SKYREAL is WO_SKYBLEND

            // take half far distance of the main camera
            props.m_skyDist = m_bscene->camera && m_bscene->camera->type == OB_CAMERA ?
                            ((Blender::Camera *)m_bscene->camera->data)->clipend * 0.5 : 10000.f;

            props.m_skyMat = GKB_IDNAME(world);
            loadSkyBox();
        }


        if (world->mode & WO_MIST)
        {
            props.m_fog.m_mode          =   world->mistype == 0 ? gkFogParams::FM_QUAD :
                                            world->mistype == 1 ? gkFogParams::FM_LIN  :
                                            gkFogParams::FM_EXP;
            props.m_fog.m_start         = world->miststa;
            props.m_fog.m_end           = props.m_fog.m_start + world->mistdist;
            props.m_fog.m_intensity     = world->misi;
            props.m_fog.m_color         = props.m_world;

            if ((world->skytype & WO_SKYBLEND))
            {
                gkVector3 a(world->horr, world->horg, world->horb), b(world->zenr, world->zeng, world->zenb);
                gkVector3 c = gkMathUtils::interp(a, b, 0.5);
                props.m_fog.m_color.r = c.x;
                props.m_fog.m_color.g = c.y;
                props.m_fog.m_color.b = c.z;
            }
        }

    }

}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectGroup(gkGameObjectGroup *gobj, Blender::Object *bobj)
{
    if (!bobj)
        return;

    for (Blender::GroupObject *group = (Blender::GroupObject *)bobj->dup_group->gobject.first; group;
            group = (Blender::GroupObject *)group->next)
    {
        Blender::Object *gob = group->ob;
        if (gob->transflag &OB_DUPLIGROUP && gob->dup_group != 0)
            convertObjectGroup(gobj, gob);
        else
        {
            gkGameObject *oth = m_gscene->getObject(GKB_IDNAME(gob));
            if (oth)
                gobj->addObject(oth);
        }
    }

}

// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertGroups(utArray<Blender::Object *> &groups, utArray<Blender::Object *> &children)
{
    bParse::bListBasePtr *lbp = m_file->getInternalFile()->getMain()->getGroup();
    for (int i=0; i<lbp->size(); ++i)
    {
        Blender::Group *bgrp = (Blender::Group*)lbp->at(i);

        if (!m_gscene->hasGroup(GKB_IDNAME(bgrp)))
        {
            gkGameObjectGroup *ggrp= 0;

            for (Blender::GroupObject *bgobj = (Blender::GroupObject*)bgrp->gobject.first; 
                bgobj; bgobj = bgobj->next)
            {
                if (bgobj->ob)
                {
                    if (m_gscene->hasObject(GKB_IDNAME(bgobj->ob)))
                    {
                        if (ggrp == 0) 
                            ggrp = m_gscene->createGroup(GKB_IDNAME(bgrp));

                        gkGameObject *gobj = m_gscene->getObject(GKB_IDNAME(bgobj->ob));
                        ggrp->addObject(gobj);

                    }
                }
            }
        }
    }



    for (UTsize i=0; i<groups.size(); ++i)
    {
        Blender::Object *ob = groups.at(i);

        if (ob->transflag &OB_DUPLIGROUP && ob->dup_group != 0)
        {
            gkGameObjectGroup *grp = 0;

            if (!m_gscene->hasGroup(GKB_IDNAME(ob->dup_group)))
            {
                // create initial group objects
                grp = m_gscene->createGroup(GKB_IDNAME(ob->dup_group));
                convertObjectGroup(grp, ob);
            }
            else
                grp = m_gscene->getGroup(GKB_IDNAME(ob->dup_group));

            if (grp != 0)
            {
                convertObject(ob);


                gkGameObject *obp = m_gscene->getObject(GKB_IDNAME(ob));
                obp->setGroupOwner(true);
                m_gscene->createInstance(obp, grp);
            }
        }

    }
}



// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObject(Blender::Object *bobj)
{
    if (!m_gscene)
        return;

    GK_ASSERT(validObject(bobj));

    gkGameObject *gobj = 0;

    switch (bobj->type)
    {
    case OB_EMPTY:      gobj = m_gscene->createObject(GKB_IDNAME(bobj));    break;
    case OB_LAMP:       gobj = m_gscene->createLight(GKB_IDNAME(bobj));     break;
    case OB_CAMERA:     gobj = m_gscene->createCamera(GKB_IDNAME(bobj));    break;
    case OB_MESH:       gobj = m_gscene->createEntity(GKB_IDNAME(bobj));    break;
    case OB_ARMATURE:   gobj = m_gscene->createSkeleton(GKB_IDNAME(bobj));  break;
    }



    // all game object property types
    if (gobj)
    {
        convertObjectGeneral(gobj, bobj);
        convertObjectProperties(gobj, bobj);
        convertObjectPhysics(gobj, bobj);
        convertObjectConstraints(gobj, bobj);
        convertObjectLogic(gobj, bobj);


        // object data
        switch (bobj->type)
        {
        case OB_LAMP:       convertObjectLamp(gobj, bobj);        break;
        case OB_CAMERA:     convertObjectCamera(gobj, bobj);      break;
        case OB_MESH:       convertObjectMesh(gobj, bobj);        break;
        case OB_ARMATURE:   convertObjectArmature(gobj, bobj);    break;
        }
    }
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectGeneral(gkGameObject *gobj, Blender::Object *bobj)
{
    gkQuaternion quat;
    gkVector3 loc, scale;
    gkMatrix4 obmat = gkMathUtils::getFromFloat(bobj->obmat);

    if (!bobj->parent)
        gkMathUtils::extractTransform(obmat, loc, quat, scale);
    else
    {
        gkMatrix4 parent = gkMathUtils::getFromFloat(bobj->parent->obmat);

        obmat = parent.inverse() * obmat;
        gkMathUtils::extractTransform(obmat, loc, quat, scale);
    }

    // prevent zero scale
    gkVector3 scaleTest = gkVector3(bobj->size.x, bobj->size.y, bobj->size.z);
    if (scaleTest.isZeroLength())
        scale = gkVector3(1.f, 1.f, 1.f);


    gkGameObjectProperties &props = gobj->getProperties();

    props.m_transform = gkTransformState(loc, quat, scale);
    if (bobj->dtx & OB_BOUNDBOX)
        props.m_mode |= GK_SHOWBB;

    if (bobj->restrictflag & OB_RESTRICT_RENDER)
        props.m_mode |= GK_INVISIBLE;

    gobj->setActiveLayer((m_bscene->lay & bobj->lay)!=0);

}

// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectProperties(gkGameObject *gobj, Blender::Object *bobj)
{
    // Attach variables to object ( used in game logic )
    for (Blender::bProperty *prop = (Blender::bProperty *)bobj->prop.first; prop; prop = prop->next)
    {
        gkVariable *gop = 0;
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
            gop->setValue(*((gkScalar *)&prop->data));
        }
        else if (prop->type == GPROP_STRING)
        {
            gop = gobj->createVariable(prop->name, prop->flag != 0);
            gop->setValue(gkString((char *)prop->poin));
        }
        else if (prop->type == GPROP_TIME)
        {
            gop = gobj->createVariable(prop->name, prop->flag != 0);
            gop->setValue(*((gkScalar *)&prop->data));
        }
    }
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectConstraints(gkGameObject *gobj, Blender::Object *bobj)
{
    // TODO: setup physics constraints & add more

    for (Blender::bConstraint *bc = (Blender::bConstraint *)bobj->constraints.first; bc; bc = bc->next)
    {
        if (bc->enforce == 0.0)
            continue;


        if (bc->type == CONSTRAINT_TYPE_ROTLIMIT)
        {
            Blender::bRotLimitConstraint *lr = (Blender::bRotLimitConstraint *)bc->data;
            if (!lr->flag)
                continue;

            gkLimitRotConstraint *c = new gkLimitRotConstraint();

            if (lr->flag & LIMIT_XROT)
                c->setLimitX(gkVector2(lr->xmin, lr->xmax));
            if (lr->flag & LIMIT_YROT)
                c->setLimitY(gkVector2(lr->ymin, lr->ymax));
            if (lr->flag & LIMIT_ZROT)
                c->setLimitZ(gkVector2(lr->zmin, lr->zmax));

            c->setLocal(bc->ownspace == CONSTRAINT_SPACE_LOCAL);
            c->setInfluence(bc->enforce);
            gobj->addConstraint(c);
        }
        else if (bc->type == CONSTRAINT_TYPE_LOCLIMIT)
        {
            Blender::bLocLimitConstraint *ll = (Blender::bLocLimitConstraint *)bc->data;
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
            gobj->addConstraint(c);
        }
    }
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectPhysics(gkGameObject *gobj, Blender::Object *bobj)
{
    gkGameObjectProperties  &props  = gobj->getProperties();
    gkPhysicsProperties     &phy    = props.m_physics;

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

    if (bobj->gameflag & OB_COLLISION_RESPONSE)             phy.m_mode |= GK_NO_SLEEP;
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
        Blender::Mesh *me = (Blender::Mesh*)bobj->data;

        if (me)
        {
            Blender::Material *ma = BlenderMaterial(me, 0);
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
        if (phy.m_minVel >0.f || phy.m_maxVel >0.f)
        {
            gkLimitVelocityConstraint *vc = new gkLimitVelocityConstraint();
            vc->setLimit(gkVector2(phy.m_minVel, phy.m_maxVel));
            gobj->addConstraint(vc);
        }
    }
}

// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectLogic(gkGameObject *gobj, Blender::Object *bobj)
{
    m_logic->convertObject(bobj, gobj);
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectCamera(gkGameObject *gobj, Blender::Object *bobj)
{
    GK_ASSERT(gobj->getType() == GK_CAMERA && bobj->data);
    gkCamera *obj = static_cast<gkCamera *>(gobj);

    Blender::Camera *camera = static_cast<Blender::Camera *>(bobj->data);

    gkCameraProperties &props = obj->getCameraProperties();

    props.m_type        = gkCameraProperties::CA_PERSPECTIVE; // todo ortho
    props.m_clipend     = camera->clipend;
    props.m_clipstart   = camera->clipsta;
    props.m_fov         = gkScalar(360) * gkMath::ATan(gkScalar(16) / camera->lens).valueRadians() / gkPi;
    props.m_start       = m_bscene->camera == bobj;
}

// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectLamp(gkGameObject *gobj, Blender::Object *bobj)
{
    GK_ASSERT(gobj->getType() == GK_LIGHT && bobj->data);

    gkLight *obj = static_cast<gkLight *>(gobj);

    gkLightProperties &props = obj->getLightProperties();
    Blender::Lamp *la = static_cast<Blender::Lamp *>(bobj->data);

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

    props.m_casts   = true;
    props.m_spot.y  = la->spotsize > 128 ? 128 : la->spotsize;
    props.m_spot.x  = gkMax(gkRadian(la->spotblend).valueDegrees(), props.m_spot.y);
    props.m_falloff = 128.f * la->spotblend;
}

// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectMesh(gkGameObject *gobj, Blender::Object *bobj)
{
    GK_ASSERT(gobj->getType() == GK_ENTITY && bobj->data);
    gkEntity *obj = static_cast<gkEntity *>(gobj);

    gkEntityProperties &props = obj->getEntityProperties();



    Blender::Mesh *me =  (Blender::Mesh *)bobj->data;


    // this is shared for faster conversion times
    // and better efficiency
    if (!m_gscene->hasMesh(GKB_IDNAME(me)))
    {
        props.m_mesh = m_gscene->createMesh(GKB_IDNAME(me));
        gkBlenderMeshConverter meconv(m_file, props.m_mesh, bobj, me);
        meconv.convert();
    }
    else
        props.m_mesh = m_gscene->getMesh(GKB_IDNAME(me));


    props.m_casts = gobj->getProperties().m_physics.isRigidOrDynamic() || !gobj->getProperties().isPhysicsObject();

    Blender::Material *matr = BlenderMaterial(me, 0);
    if (matr)
        props.m_casts = (matr->mode & MA_SHADBUF) != 0;

    // if it has an action save the initial pose / animation name
    if (bobj->parent)
    {
        Blender::Object *par = bobj->parent;
        Blender::bAction *act = par->action;
        if (!act && par->proxy_from)
            act = par->proxy_from->action;

        if (act) props.m_startPose = (GKB_IDNAME(act));
    }
}

// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectActions(gkGameObject *gobj, Blender::Object *bobj)
{
    GK_ASSERT(gobj->getType() == GK_SKELETON && bobj->data);
    gkSkeleton *obj = static_cast<gkSkeleton *>(gobj);

    // create actions if needed
    bParse::bMain *mp = m_file->getInternalFile()->getMain();

    // for all actions convert
    bParse::bListBasePtr *lbp = mp->getAction();

    for (int i = 0; i < lbp->size(); ++i)
    {
        Blender::bAction *bact = (Blender::bAction *)lbp->at(i);

        // find ownership
        Blender::bActionChannel *bac = (Blender::bActionChannel *)bact->chanbase.first;

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
                    Blender::IpoCurve *icu = (Blender::IpoCurve *)bac->ipo->curve.first;
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


// ----------------------------------------------------------------------------
class gkSkeletonLoader
{
public:
    gkSkeletonLoader() : m_skeleton(0), m_armature(0) {}

    void buildBoneTree(Blender::Bone *cur, Blender::Bone *prev, gkBone *parent);

    gkSkeleton              *m_skeleton;
    Blender::bArmature      *m_armature;
};




// ----------------------------------------------------------------------------
void gkSkeletonLoader::buildBoneTree(Blender::Bone *cur, Blender::Bone *prev, gkBone *parent)
{
    // This is the normal resposition
    GK_ASSERT(cur);
    GK_ASSERT(m_skeleton);

    gkMatrix4 parBind = gkMatrix4::IDENTITY;
    if (prev != 0 && parent != 0)
        parBind = gkMathUtils::getFromFloatNorm(prev->arm_mat).inverse();

    // create the ogre bone
    gkBone *bone = m_skeleton->createBone(cur->name);
    if (parent)
        bone->setParent(parent);


    gkMatrix4 bind = parBind * gkMathUtils::getFromFloatNorm(cur->arm_mat);

    gkQuaternion rot; gkVector3 loc, scl;
    gkMathUtils::extractTransformFast(bind, loc, rot, scl);

    if (rot.isNaN())
    {
        rot = gkQuaternion();
        scl = gkVector3(1,1,1);
    }

    bone->setRestPosition(gkTransformState(loc, rot, scl));


    Blender::Bone *chi = static_cast<Blender::Bone *>(cur->childbase.first);
    while (chi)
    {
        // recurse
        buildBoneTree(chi, cur, bone);
        chi = chi->next;
    }
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectSkeleton(gkGameObject *gobj, Blender::Object *bobj)
{
    GK_ASSERT(gobj->getType() == GK_SKELETON && bobj->data);

    gkSkeletonLoader loader;

    loader.m_armature = static_cast<Blender::bArmature *>(bobj->data);
    loader.m_skeleton = static_cast<gkSkeleton *>(gobj);


    // create bone lists && transforms
    Blender::Bone *bone = static_cast<Blender::Bone *>(loader.m_armature->bonebase.first);
    while (bone)
    {
        // only process root bones
        if (bone->parent == 0)
            loader.buildBoneTree(bone, NULL, NULL);
        bone = bone->next;
    }
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convertObjectArmature(gkGameObject *gobj, Blender::Object *bobj)
{
    convertObjectSkeleton(gobj, bobj);
    convertObjectActions(gobj, bobj);
}

// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::loadSkyBox()
{
    Blender::World *wo = m_bscene->world;


    // use user defined 
    Ogre::MaterialPtr matptr = Ogre::MaterialManager::getSingleton().getByName(GKB_IDNAME(wo));
    if (!matptr.isNull())
    {
        // use blender ori 
        m_gscene->getProperties().m_skyOri = gkEuler(-90, 0, 0).toQuaternion();
        return;
    }

    matptr = Ogre::MaterialManager::getSingleton().create(GKB_IDNAME(wo), "<gkBuiltin>");



    gkSkyBoxGradient *grad = new gkSkyBoxGradient(wo);
    m_file->_registerLoader(grad);

    Ogre::TexturePtr txptr =  Ogre::TextureManager::getSingleton().create(GKB_IDNAME(wo), "<gkBuiltin>", true, grad);
    matptr->setLightingEnabled(false);
    matptr->setReceiveShadows(false);


    Ogre::TextureUnitState *tus = matptr->getTechnique(0)->getPass(0)->createTextureUnitState();
    tus->setTextureName(GKB_IDNAME(wo), Ogre::TEX_TYPE_CUBE_MAP);
    tus->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
}


// ----------------------------------------------------------------------------
void gkBlenderSceneConverter::convert(void)
{
    if (m_gscene)
        return;

    m_gscene = gkSceneManager::getSingleton().create(GKB_IDNAME(m_bscene));


    if (m_bscene->world)
        convertWorld();



    utArray<Blender::Object *> groups, children;
    for (Blender::Base *base = (Blender::Base *)m_bscene->base.first; base; base = base->next)
    {
        if (!base->object)
            continue;

        Blender::Object *bobj = base->object;

        // non - conversion object
        if (!validObject(bobj))
            continue;


        if (bobj->transflag &OB_DUPLIGROUP && bobj->dup_group != 0)
            groups.push_back(bobj);
        else
            convertObject(bobj);

        // save parent / child hierarchy
        if (bobj->parent)
            children.push_back(bobj);
    }

    // build group instances
    convertGroups(groups, children);


    applyParents(children);

    m_logic->resolveLinks();
}


// ----------------------------------------------------------------------------
//
//                          Mesh Loader
//
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
class gkMeshHashKey
{
protected:

    friend class    gkBlenderMeshConverter;

    short           m_matnr, m_mode;
    bool            m_blenderMat;

    Blender::Image *m_images[8];

public:
    gkMeshHashKey()
        : m_matnr(0), m_mode(0), m_blenderMat(false)
    {
        for (int i=0; i<8; ++i) m_images[i] = 0;
    }

    // Blender materials
    gkMeshHashKey(int mat_nr, int mode)
        : m_matnr(mat_nr), m_mode(0), m_blenderMat(true)
    {
        for (int i=0; i<8; ++i) m_images[i] = 0;
        if (mode & TF_INVISIBLE)    m_mode |= gkMaterialProperties::MA_INVISIBLE;
        if (mode & TF_TWOSIDE)      m_mode |= gkMaterialProperties::MA_TWOSIDE;
    }


    // Blender texture materials
    gkMeshHashKey(int mode, int alpha, Blender::Image *images[8])
        : m_matnr(-1), m_mode(0), m_blenderMat(false)
    {
        m_mode |= gkMaterialProperties::MA_RECEIVESHADOWS;
        m_mode |= gkMaterialProperties::MA_DEPTHWRITE;

        if (mode & TF_INVISIBLE)    m_mode |= gkMaterialProperties::MA_INVISIBLE;
        if (mode & TF_LIGHT)        m_mode |= gkMaterialProperties::MA_LIGHTINGENABLED;
        if (mode & TF_TWOSIDE)      m_mode |= gkMaterialProperties::MA_TWOSIDE;
        if ((alpha & TF_CLIP))      m_mode &= ~gkMaterialProperties::MA_DEPTHWRITE;
        if (alpha &TF_ADD)          m_mode |= gkMaterialProperties::MA_ADDITIVEBLEND;
        if (mode & TF_TEX)          m_mode |= gkMaterialProperties::MA_HASFACETEX;

        if (alpha &TF_ALPHA || alpha & TF_CLIP)
            m_mode |= gkMaterialProperties::MA_ALPHABLEND;
        for (int i=0; i<8; ++i) m_images[i] = images[i];
    }

    // Copy constructor
    gkMeshHashKey(const gkMeshHashKey &k)
        :   m_matnr(k.m_matnr), m_mode(k.m_mode),  m_blenderMat(k.m_blenderMat)

    {
        for (int i=0; i<8; ++i) m_images[i] = m_blenderMat ? 0 : k.m_images[i];
    }


    bool operator == (const gkMeshHashKey& rhs) const
    {
        if (m_blenderMat)
            return m_matnr == rhs.m_matnr;

        return  m_mode == rhs.m_mode && 
                m_images[0] == rhs.m_images[0] && 
                m_images[1] == rhs.m_images[1] && 
                m_images[2] == rhs.m_images[2] && 
                m_images[3] == rhs.m_images[3] && 
                m_images[4] == rhs.m_images[4] && 
                m_images[5] == rhs.m_images[5] && 
                m_images[6] == rhs.m_images[6] && 
                m_images[7] == rhs.m_images[7];
    }


};

class gkMeshPair
{
public:
    gkMeshHashKey test;
    gkSubMesh     *item;

    gkMeshPair() : test(), item(0)
    {
    }

    gkMeshPair& operator = (const gkMeshPair& p)
    {
        if (this != &p)
        {
            test = gkMeshHashKey(p.test);
            item = p.item;
        }
        return *this;
    }

    bool operator == (const gkMeshPair& rhs) const
    {
        return test == rhs.test;
    }

};

// ----------------------------------------------------------------------------
gkBlenderMeshConverter::gkBlenderMeshConverter(gkBlendFile *fp, gkMesh *gmesh, Blender::Object *bobject, Blender::Mesh *bmesh)
    :   m_gmesh(gmesh), m_bmesh(bmesh), m_file(fp), m_bobj(bobject)
{
}

// ----------------------------------------------------------------------------
gkBlenderMeshConverter::~gkBlenderMeshConverter()
{
}


// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::convertIndexedTriangle(
    TempFace *dest,
    unsigned int i0,
    unsigned int i1,
    unsigned int i2,
    PackedFace &face)
{
    gkVertex &v0 = dest->v0;
    gkVertex &v1 = dest->v1;
    gkVertex &v2 = dest->v2;

    dest->i0 = face.index[i0];
    dest->i1 = face.index[i1];
    dest->i2 = face.index[i2];

    v0.vba = face.index[i0];
    v1.vba = face.index[i1];
    v2.vba = face.index[i2];

    v0.vcol = face.colors[i0];
    v1.vcol = face.colors[i1];
    v2.vcol = face.colors[i2];

    VEC3CPY(v0.co, face.verts[i0].co);
    VEC3CPY(v1.co, face.verts[i1].co);
    VEC3CPY(v2.co, face.verts[i2].co);
    VEC3CPN(v0.no, face.verts[i0].no);
    VEC3CPN(v1.no, face.verts[i1].no);
    VEC3CPN(v2.no, face.verts[i2].no);

    for (int i=0; i<face.totlay; ++i)
    {
        VEC2CPY(v0.uv[i], face.uvLayers[i][i0]);
        VEC2CPY(v1.uv[i], face.uvLayers[i][i1]);
        VEC2CPY(v2.uv[i], face.uvLayers[i][i2]);
    }
}

// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::calcNormal(TempFace *tri)
{
    gkVector3 n = (tri->v1.co - tri->v2.co).crossProduct(tri->v2.co - tri->v0.co).normalisedCopy();
    tri->v0.no = tri->v1.no = tri->v2.no = n;
}


// ----------------------------------------------------------------------------
unsigned int gkBlenderMeshConverter::packColour(Blender::MCol col, bool opengl)
{
    union
    {
        Blender::MCol col;
        unsigned int integer;
        unsigned char cp[4];
    } out_color, in_color;


    in_color.col = col;


    if (opengl) // abgr
    {
        out_color.cp[0] = in_color.cp[3]; // red
        out_color.cp[1] = in_color.cp[2]; // green
        out_color.cp[2] = in_color.cp[1]; // blue
        out_color.cp[3] = in_color.cp[0]; // alpha
    }
    else // argb
    {
        // vertex buffer is packed with VET_COLOUR_ABGR, swap b,r
        out_color.cp[2] = in_color.cp[3]; // red
        out_color.cp[1] = in_color.cp[2]; // green
        out_color.cp[0] = in_color.cp[1]; // blue
        out_color.cp[3] = in_color.cp[0]; // alpha
    }

    return out_color.integer;
}


// ----------------------------------------------------------------------------
int gkBlenderMeshConverter::findTextureLayer(Blender::MTex *te)
{
    if (!(te->texco & TEXCO_UV) || te->uvname[0]=='\0')
        return 0;

    if (m_bmesh->fdata.layers)
    {
        Blender::CustomDataLayer *cd = (Blender::CustomDataLayer *)m_bmesh->fdata.layers;
        if (cd)
        {
            int layer = 0;
            for (int i = 0; i < m_bmesh->fdata.totlayer; i++)
            {
                if (cd[i].type == CD_MTFACE)
                {
                    if (utCharEq(cd[i].name, te->uvname))
                        return layer;
                    ++layer;
                }
            }
        }
    }
    return 0;

}

// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::convertTextureFace(gkMaterialProperties &gma, gkMeshHashKey &hk, Blender::Image **imas)
{
    gma.m_mode = hk.m_mode;
    if (imas)
    {
        static char buf[32];
        static int uid = 0;

        sprintf(buf, "TextureFace %i", (uid++));
        gma.m_name = buf;
    }

    if (imas && gma.m_mode & gkMaterialProperties::MA_HASFACETEX)
    {
        gma.m_totaltex = 0;
        for (int i = 0; i < 8; i++)
        {
            if (imas[i] != 0 && PtrSaftyCheck(imas[i]))
            {
                Blender::Image *ima = imas[i];
                gkTexureProperties &gte = gma.m_textures[gma.m_totaltex++];
                gte.m_layer = i;
                gte.m_image = gte.m_name = GKB_IDNAME(ima);
            }
        }
    }
}

// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::convertMaterial(Blender::Material *bma, gkMaterialProperties &gma, gkMeshHashKey &hk)
{
    convertTextureFace(gma, hk, 0);

    gma.m_name          = GKB_IDNAME(bma);
    gma.m_hardness      = bma->har / 4.f;
    gma.m_refraction    = bma->ref;
    gma.m_emissive      = bma->emit;
    gma.m_ambient       = bma->amb;
    gma.m_spec          = bma->spec;
    gma.m_alpha         = bma->alpha;
    gma.m_diffuse       = gkColor(bma->r, bma->g, bma->b);
    gma.m_specular      = gkColor(bma->specr, bma->specg, bma->specb);

    if (bma->mode & MA_ZTRA)        gma.m_mode |= gkMaterialProperties::MA_DEPTHWRITE;
    if (bma->mode & MA_SHADOW)      gma.m_mode |= gkMaterialProperties::MA_RECEIVESHADOWS;
    if (bma->mode & MA_WIRE)        gma.m_mode |= gkMaterialProperties::MA_WIREFRAME;
    if (!(bma->mode & MA_SHLESS))   gma.m_mode |= gkMaterialProperties::MA_LIGHTINGENABLED;
    if (bma->alpha <= 0.f)          gma.m_mode |= gkMaterialProperties::MA_INVISIBLE;


    // textures
    if (bma->mtex != 0)
    {
        gma.m_totaltex = 0;

        for (int i = 0; i < MAX_MTEX; i++)
        {
            if (!PtrSaftyCheck(bma->mtex[i]) || !bma->mtex[i] || !bma->mtex[i]->tex)
                continue;

            if (bma->mtex[i]->tex->type == TEX_IMAGE)
            {
                Blender::MTex *mtex = bma->mtex[i];

                Blender::Image *ima = mtex->tex->ima;
                if (!ima)
                    continue;

                gkTexureProperties &gte = gma.m_textures[gma.m_totaltex++];
                gte.m_image = gte.m_name = GKB_IDNAME(ima);

                if (mtex->texflag & MTEX_STENCIL)
                    gte.m_mode |= gkTexureProperties::TM_SPLAT;
                if (mtex->mapto & MAP_ALPHA)
                {
                    gte.m_mode |= gkTexureProperties::TM_ALPHA;
                    gma.m_mode |= gkMaterialProperties::MA_ALPHABLEND;
                }

                switch (mtex->blendtype)
                {
                case MTEX_BLEND:    gte.m_blend = gkTexureProperties::BM_MIXTURE;   break;
                case MTEX_ADD:      gte.m_blend = gkTexureProperties::BM_ADDITIVE;  break;
                case MTEX_SUB:      gte.m_blend = gkTexureProperties::BM_SUBTRACT;  break;
                default:            gte.m_blend = gkTexureProperties::BM_MULTIPLY;  break;
                }

                gte.m_layer = findTextureLayer(mtex);
                gte.m_mix   = mtex->colfac;
            }
        }
    }
}


// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::convertBoneAssignments(int dgi, AssignmentListMap &dest)
{

    Blender::MDeformVert *dvert = m_bmesh->dvert;

    for (int n = 0; n < m_bmesh->totvert; n++)
    {
        const Blender::MDeformVert &dv = dvert[n];
        for (int w = 0; w < dv.totweight; w++)
        {
            if (dv.dw[w].def_nr == dgi)
            {
                gkDeformVertex gdw;
                gdw.group       = dgi;
                gdw.weight      = dv.dw[w].weight;
                gdw.vertexId    = 0;

                UTsize pos;
                if ((pos = dest.find(n)) == GK_NPOS)
                {
                    dest.insert(n, AssignmentList());
                    pos = dest.find(n);
                }
                if (pos != GK_NPOS)
                    dest.at(pos).push_back(gdw);
            }
        }
    }
}

// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::findWeight(int index, gkSubMesh *me, AssignmentListMap &assign)
{
    gkVertex *vbuf = me->getVertexBuffer().ptr();
    if (vbuf[index].vba == -1)
        return;

    UTsize pos = assign.find(vbuf[index].vba);
    if (pos != GK_NPOS)
    {
        AssignmentList &list = assign.at(pos);
        for (UTsize a = 0; a < list.size(); a++)
        {
            gkDeformVertex &dv = list.at(a);
            dv.vertexId = index;
            me->addDeformVert(dv);
        }
    }
    vbuf[index].vba = -1;
}



// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::assignBoneAssignments(gkSubMesh *me, AssignmentListMap &dest)
{
    UTsize totface = me->getIndexBuffer().size();
    gkTriangle *ibuf = me->getIndexBuffer().ptr();

    for (UTsize f=0; f<totface; ++f)
    {
        const gkTriangle &i = ibuf[f];

        findWeight((int)i.i0, me, dest);
        findWeight((int)i.i1, me, dest);
        findWeight((int)i.i2, me, dest);
    }

}

// ----------------------------------------------------------------------------
void gkBlenderMeshConverter::convert(void)
{
    Blender::MFace  *mface = m_bmesh->mface;
    Blender::MVert  *mvert = m_bmesh->mvert;
    Blender::MCol   *mcol =  0;
    Blender::MTFace *mtface[8] = {0,0,0,0,0,0,0,0};


    if (!mface || !mvert)
        return;

    Blender::MVert          vpak[4];
    unsigned int            cpak[4];
    unsigned int            ipak[4];
    int                     totlayer;


    gkSubMesh *curSubMesh = 0;
    utArray<gkMeshPair> meshtable;

    gkLoaderUtils_getLayers(m_file->getInternalFile(), m_bmesh, mtface, &mcol, totlayer);

    bool sortByMat          = gkEngine::getSingleton().getUserDefs().blendermat;
    bool openglVertexColor  = gkEngine::getSingleton().getUserDefs().rendersystem == OGRE_RS_GL;


    AssignmentListMap assignMap;
    bool canAssign = m_bmesh->dvert && m_bobj->parent && m_bobj->parent->type == OB_ARMATURE;
    if (canAssign)
    {
        int dgi=0;
        for (Blender::bDeformGroup *dg = (Blender::bDeformGroup *)m_bobj->defbase.first; dg; dg = dg->next, ++dgi)
        {
            m_gmesh->createVertexGroup(dg->name);
            convertBoneAssignments(dgi, assignMap);
        }
    }



    for (int fi = 0; fi < m_bmesh->totface; fi++)
    {
        const Blender::MFace &curface = mface[fi];

        // skip if face is not a triangle || quad
        if (!curface.v3)
            continue;

        const bool isQuad = curface.v4 != 0;

        TempFace t[2];
        PackedFace f;
        f.totlay = totlayer;

        if (isQuad)
        {
            vpak[0] = mvert[curface.v1];
            vpak[1] = mvert[curface.v2];
            vpak[2] = mvert[curface.v3];
            vpak[3] = mvert[curface.v4];

            ipak[0] = curface.v1;
            ipak[1] = curface.v2;
            ipak[2] = curface.v3;
            ipak[3] = curface.v4;

            if (mcol != 0)
            {
                cpak[0] = packColour(mcol[0], openglVertexColor);
                cpak[1] = packColour(mcol[1], openglVertexColor);
                cpak[2] = packColour(mcol[2], openglVertexColor);
                cpak[3] = packColour(mcol[3], openglVertexColor);
            }
            else
                cpak[0] = cpak[1] = cpak[2] = cpak[3] = 0xFFFFFFFF;


            for (int i = 0; i < totlayer; i++)
            {
                if (mtface[i] != 0)
                {
                    f.uvLayers[i][0] = gkVector2((float *)mtface[i][fi].uv[0]);
                    f.uvLayers[i][1] = gkVector2((float *)mtface[i][fi].uv[1]);
                    f.uvLayers[i][2] = gkVector2((float *)mtface[i][fi].uv[2]);
                    f.uvLayers[i][3] = gkVector2((float *)mtface[i][fi].uv[3]);
                }
            }
            f.verts     = vpak;
            f.index     = ipak;
            f.colors    = cpak;

            gkVector3 e0, e1;

            e0 = (gkVector3(&mvert[curface.v1].co.x) - gkVector3(&mvert[curface.v2].co.x));
            e1 = (gkVector3(&mvert[curface.v3].co.x) - gkVector3(&mvert[curface.v4].co.x));

            if (e0.squaredLength() < e1.squaredLength())
            {
                convertIndexedTriangle(&t[0], 0, 1, 2, f);
                convertIndexedTriangle(&t[1], 2, 3, 0, f);
            }
            else
            {
                convertIndexedTriangle(&t[0], 0, 1, 3, f);
                convertIndexedTriangle(&t[1], 3, 1, 2, f);
            }
        }
        else
        {
            for (int i = 0; i < totlayer; i++)
            {
                if (mtface[i] != 0)
                {
                    f.uvLayers[i][0] = gkVector2((float *)mtface[i][fi].uv[0]);
                    f.uvLayers[i][1] = gkVector2((float *)mtface[i][fi].uv[1]);
                    f.uvLayers[i][2] = gkVector2((float *)mtface[i][fi].uv[2]);
                }
            }

            vpak[0] = mvert[curface.v1];
            vpak[1] = mvert[curface.v2];
            vpak[2] = mvert[curface.v3];

            ipak[0] = curface.v1;
            ipak[1] = curface.v2;
            ipak[2] = curface.v3;

            if (mcol != 0)
            {
                cpak[0] = packColour(mcol[0], openglVertexColor);
                cpak[1] = packColour(mcol[1], openglVertexColor);
                cpak[2] = packColour(mcol[2], openglVertexColor);
            }
            else
                cpak[0] = cpak[1] = cpak[2] = cpak[3] = 0xFFFFFFFF;

            f.verts     = vpak;
            f.index     = ipak;
            f.colors    = cpak;

            convertIndexedTriangle(&t[0], 0, 1, 2, f);
        }

        gkMeshPair tester;
        if (sortByMat)
        {
            int mode=0;
            if (mtface[0])
                mode = mtface[0][fi].mode;

            tester.test = gkMeshHashKey(curface.mat_nr, mode);
        }
        else
        {
            Blender::Image *ima[8] = {0,0,0,0,0,0,0,0};
            for (int i = 0; i < totlayer; i++)
            {
                if (mtface[i] != 0)
                    ima[i] = mtface[i][fi].tpage;
            }

            int mode=0, alpha=0;
            if (mtface[0])
            {
                mode    = mtface[0][fi].mode;
                alpha   = mtface[0][fi].transp;
            }

            tester.test = gkMeshHashKey(mode, alpha, ima);
        }

        // find submesh
        UTsize arpos = 0;
        if ((arpos = meshtable.find(tester)) == UT_NPOS)
        {
            curSubMesh = new gkSubMesh();

            curSubMesh->setTotalLayers(totlayer);
            curSubMesh->setVertexColors(mcol != 0);

            m_gmesh->addSubMesh(curSubMesh);
            tester.item = curSubMesh;
            meshtable.push_back(tester);
        }
        else
            curSubMesh = meshtable.at(arpos).item;

        if (curSubMesh == 0) continue;


        if (!(curface.flag & ME_SMOOTH))
        {
            // face normal
            calcNormal(&t[0]);
            if (isQuad)
                t[1].v0.no = t[1].v1.no = t[1].v2.no = t[0].v0.no;
        }

        int triflag = 0;
        if (mtface[0])
        {
            if (mtface[0][fi].mode & TF_DYNAMIC)
                triflag |= gkTriangle::TRI_COLLIDER;
            if (mtface[0][fi].mode & TF_INVISIBLE)
                triflag |= gkTriangle::TRI_INVISIBLE;
        }
        else
            triflag = gkTriangle::TRI_COLLIDER;



        curSubMesh->addTriangle(t[0].v0, t[0].i0,
                                t[0].v1, t[0].i1,
                                t[0].v2, t[0].i2, triflag);

        if (isQuad)
        {
            curSubMesh->addTriangle(t[1].v0, t[1].i0,
                                    t[1].v1, t[1].i1,
                                    t[1].v2, t[1].i2, triflag);

        }

        if (mcol)
            mcol += 4;

    }

    // build materials
    utArrayIterator<utArray<gkMeshPair> > iter(meshtable);

    while (iter.hasMoreElements())
    {
        gkMeshHashKey &key  = iter.peekNext().test;
        gkSubMesh *val      = iter.peekNext().item;


        Blender::Material *bmat = BlenderMaterial(m_bmesh, key.m_matnr);
        if (key.m_blenderMat)
        {
            if (bmat)
                convertMaterial(bmat, val->getMaterial(), key);
        }
        else
            convertTextureFace(val->getMaterial(), key, (Blender::Image **)key.m_images);

        if (canAssign)
        {
            // build def groups
            assignBoneAssignments(val, assignMap);

        }

        iter.getNext();
    }
}


// ----------------------------------------------------------------------------
//
//                          GameLogic Loader
//
// ----------------------------------------------------------------------------


gkLogicLoader::gkLogicLoader()
{
}

gkLogicLoader::~gkLogicLoader()
{
}

void gkLogicLoader::resolveLinks(void)
{
    utListIterator<CreatedList> oblist(m_createdLinks);
    while (oblist.hasMoreElements())
    {
        gkLogicLink *link = oblist.getNext();

        utListIterator<ResolveObjectList> missing(m_missing);
        while (missing.hasMoreElements())
        {
            ResolveObject &ob = missing.getNext();

            if (ob.cont && ob.lsens)
            {
                gkLogicController *lc = link->findController(ob.cont);
                if (lc)
                    ob.lsens->link(lc);
            }

            if (ob.act && ob.lcont)
            {
                gkLogicActuator *la = link->findActuator(ob.act);
                if (la)
                    ob.lcont->link(la);
            }

        }
    }
}


gkString gkLogicLoader_formatText(const gkString &in)
{
    // trim any whitespace
    gkString ret = in;
    utStringUtils::trim(ret);
    return ret;
}



void gkLogicLoader::convertObject(Blender::Object *bobj, gkGameObject *gobj)
{
    GK_ASSERT(gobj && bobj);

    if (!bobj->sensors.first && !bobj->controllers.first && !bobj->actuators.first)
        return;

    gkLogicManager *rlm = gkLogicManager::getSingletonPtr();

    // new link buffer
    gkLogicLink *lnk = rlm->createLink();

    m_createdLinks.push_back(lnk);

    lnk->setState(bobj->init_state ? bobj->init_state : bobj->state);
    gobj->setState(lnk->getState());
    lnk->setObject(gobj);
    gobj->attachLogic(lnk);



    for (Blender::bActuator *bact = (Blender::bActuator *)bobj->actuators.first; bact; bact = bact->next)
    {
        gkLogicActuator *la = 0;
        switch (bact->type)
        {
        case ACT_EDIT_OBJECT:
            {
                gkEditObjectActuator *ea = new gkEditObjectActuator(gobj, lnk, bact->name);
                la = ea;

                Blender::bEditObjectActuator *bea = (Blender::bEditObjectActuator *)bact->data;

                ea->setLifeSpan(bea->time);
                ea->setAngV(gkVector3(bea->angVelocity.x, bea->angVelocity.y, bea->angVelocity.z));
                ea->setAngVL((bea->localflag & ACT_EDOB_LOCAL_ANGV) != 0);
                ea->setLinV(gkVector3(bea->linVelocity.x, bea->linVelocity.y, bea->linVelocity.z));
                ea->setLinVL((bea->localflag & ACT_EDOB_LOCAL_LINV) != 0);
                ea->setObject(GKB_IDNAME(bea->ob));

                int mode = -1;
                switch (bea->type)
                {
                case ACT_EDOB_ADD_OBJECT:   {mode = gkEditObjectActuator::EO_ADDOBJ;        break;}
                case ACT_EDOB_END_OBJECT:   {mode = gkEditObjectActuator::EO_ENDOBJ;        break;}
                case ACT_EDOB_REPLACE_MESH: {mode = gkEditObjectActuator::EO_REPLACEMESH;   break;}
                case ACT_EDOB_TRACK_TO:     {mode = gkEditObjectActuator::EO_TRACKTO;       break;}
                case ACT_EDOB_DYNAMICS:     {mode = gkEditObjectActuator::EO_DYNAMICS;      break;}
                }
                ea->setMode(mode);

                int dyn=-1;

                switch (bea->dyn_operation)
                {
                case 0: {dyn = gkEditObjectActuator::EOD_RESTORE;       break;}
                case 1: {dyn = gkEditObjectActuator::EOD_SUSPEND;       break;}
                case 2: {dyn = gkEditObjectActuator::EOD_ENABLE_BODY;   break;}
                case 3: {dyn = gkEditObjectActuator::EOD_DISABLE_BODY;  break;}
                case 4: {dyn = gkEditObjectActuator::EOD_SETMASS;       break;}
                }
                ea->setDynMode(dyn);


            } break;
        case ACT_STATE:
            {
                gkStateActuator *sa = new gkStateActuator(gobj, lnk, bact->name);
                la = sa;

                Blender::bStateActuator *bst = (Blender::bStateActuator *)bact->data;

                int op = gkStateActuator::OP_NILL;


                switch (bst->type)
                {
                case 0: {op = gkStateActuator::OP_CPY; break;}
                case 1: {op = gkStateActuator::OP_ADD; break;}
                case 2: {op = gkStateActuator::OP_SUB; break;}
                case 3: {op = gkStateActuator::OP_INV; break;}
                }

                sa->setOp(op);
                sa->setMask(bst->mask);

            } break;
        case ACT_PROPERTY:
            {
                gkPropertyActuator *pa = new gkPropertyActuator(gobj, lnk, bact->name);
                la = pa;

                Blender::bPropertyActuator *bpa = (Blender::bPropertyActuator *)bact->data;

                int op = -1;

                switch (bpa->type)
                {
                case ACT_PROP_ASSIGN:   {op = gkPropertyActuator::PA_ASSIGN; break;}
                case ACT_PROP_ADD:      {op = gkPropertyActuator::PA_ADD; break;}
                case ACT_PROP_COPY:     {op = gkPropertyActuator::PA_COPY; break;}
                case ACT_PROP_TOGGLE:   {op = gkPropertyActuator::PA_TOGGLE; break;}
                }

                pa->setType(op);

                pa->setValue(bpa->value);
                pa->setProperty(gkLogicLoader_formatText(bpa->name));
                if (bpa->ob)
                    pa->setObject(GKB_IDNAME(bpa->ob));

            } break;
        case ACT_OBJECT:
            {
                gkMotionActuator *ma = new gkMotionActuator(gobj, lnk, bact->name);
                la = ma;

                Blender::bObjectActuator *objact = (Blender::bObjectActuator *)bact->data;
                ma->setRotation(gkVector3(objact->drot.x, objact->drot.y, objact->drot.z), (objact->flag & ACT_DROT_LOCAL) != 0);
                ma->setTranslation(gkVector3(objact->dloc.x, objact->dloc.y, objact->dloc.z), (objact->flag & ACT_DLOC_LOCAL) != 0);
                ma->setForce(gkVector3(objact->forceloc.x, objact->forceloc.y, objact->forceloc.z), (objact->flag & ACT_FORCE_LOCAL) != 0);
                ma->setTorque(gkVector3(objact->forcerot.x, objact->forcerot.y, objact->forcerot.z), (objact->flag & ACT_TORQUE_LOCAL) != 0);

                ma->setLinearVelocity(gkVector3(objact->linearvelocity.x, objact->linearvelocity.y, objact->linearvelocity.z),
                                      (objact->flag & ACT_LIN_VEL_LOCAL) != 0);
                ma->setAngularVelocity(gkVector3(objact->angularvelocity.x, objact->angularvelocity.y, objact->angularvelocity.z),
                                       (objact->flag & ACT_ANG_VEL_LOCAL) != 0);

                ma->setIncrementalVelocity((objact->flag & ACT_ADD_LIN_VEL) != 0);
                ma->setDamping(gkScalar(objact->damping) / 1000.f);
            }
            break;
        case ACT_ACTION:
            {
                gkActionActuator *aa = new gkActionActuator(gobj, lnk, bact->name);
                la = aa;

                Blender::bActionActuator *baa = (Blender::bActionActuator *)bact->data;

                aa->setStart(baa->sta);
                aa->setEnd(baa->end);
                aa->setBlend(baa->blendin);

                int mode = 0;
                switch (baa->type)
                {
                case ACT_ACTION_PINGPONG:   {mode = gkActionActuator::AA_PONG;          break;}
                case ACT_ACTION_FLIPPER:    {mode = gkActionActuator::AA_FLIPPER;       break;}
                case ACT_ACTION_LOOP_STOP:  {mode = gkActionActuator::AA_LOOP_STOP;     break;}
                case ACT_ACTION_LOOP_END:   {mode = gkActionActuator::AA_LOOP_END;      break;}
                case ACT_ACTION_FROM_PROP:  {mode = gkActionActuator::AA_PROPERTY;      break;}
                case ACT_ACTION_PLAY:
                case ACT_ACTION_KEY2KEY:
                case ACT_ACTION_MOTION:
                default:
                    {mode = gkActionActuator::AA_PLAY; break;}
                }
                aa->setMode(mode);

                aa->setPriority(baa->priority);
                aa->setReset(baa->end_reset != 0);

                aa->setAction(baa->act ? GKB_IDNAME(baa->act) : "");
                aa->setProperty(gkLogicLoader_formatText(baa->frameProp));

            } break;
        case ACT_GAME:
            {
                gkGameActuator *ga = new gkGameActuator(gobj, lnk, bact->name);
                la = ga;
                Blender::bGameActuator *bga = (Blender::bGameActuator *)bact->data;

                int mode = -1;
                switch (bga->type)
                {
                case ACT_GAME_START:
                case ACT_GAME_LOAD:
                    mode = gkGameActuator::GM_START_NEW;
                    break;
                case ACT_GAME_RESTART:
                    mode = gkGameActuator::GM_RESTART;
                    break;
                case ACT_GAME_QUIT:
                    mode = gkGameActuator::GM_QUIT;
                    break;
                case ACT_GAME_SAVECFG:
                    mode = gkGameActuator::GM_SAVE;
                    break;
                case ACT_GAME_LOADCFG:
                    mode = gkGameActuator::GM_LOAD;
                    break;
                }

                ga->setMode(mode);
                if (bga->filename[0] != 0)
                    ga->setGameFile(bga->filename);

            } break;

        case ACT_VISIBILITY:
            {
                gkVisibilityActuator *va = new gkVisibilityActuator(gobj, lnk, bact->name);
                la = va;

                Blender::bVisibilityActuator *bva = (Blender::bVisibilityActuator *)bact->data;


                int fl = 0;
                if (bva->flag & ACT_VISIBILITY_INVISIBLE)
                    fl |= gkVisibilityActuator::VA_VIS_FLAG;
                if (bva->flag &ACT_VISIBILITY_OCCLUSION)
                    fl |= gkVisibilityActuator::VA_OCCLUDER;
                if (bva->flag &ACT_VISIBILITY_RECURSIVE)
                    fl |= gkVisibilityActuator::VA_CHILDREN;

                va->setFlag(fl);

            } break;
        }



        if (la)
            lnk->push(la, bact);

    }

    int uuid = 0;

    for (Blender::bController *bcont = (Blender::bController *)bobj->controllers.first; bcont; bcont = bcont->next)
    {
        gkLogicController *lc = 0;
        switch (bcont->type)
        {
        case CONT_LOGIC_OR:
        case CONT_LOGIC_XOR:
        case CONT_LOGIC_AND:
        case CONT_LOGIC_NAND:
        case CONT_LOGIC_NOR:
        case CONT_LOGIC_XNOR:
            {
                gkLogicOpController *ac = new gkLogicOpController(gobj, lnk, bcont->name);
                if (bcont->type == CONT_LOGIC_OR)
                    ac->setOp(gkLogicOpController::OP_OR);
                else if (bcont->type == CONT_LOGIC_XOR)
                    ac->setOp(gkLogicOpController::OP_XOR);
                else if (bcont->type == CONT_LOGIC_AND)
                    ac->setOp(gkLogicOpController::OP_AND);
                else if (bcont->type == CONT_LOGIC_NAND)
                    ac->setOp(gkLogicOpController::OP_NAND);
                else if (bcont->type == CONT_LOGIC_NOR)
                    ac->setOp(gkLogicOpController::OP_NOR);
                else if (bcont->type == CONT_LOGIC_XNOR)
                    ac->setOp(gkLogicOpController::OP_XNOR);
                lc = ac;
            }
            break;
        case CONT_PYTHON:
            {
                gkScriptController *sc = new gkScriptController(gobj, lnk, bcont->name);
                lc = sc;

                Blender::bPythonCont *pcon = (Blender::bPythonCont *)bcont->data;
                sc->setModule(false);

                if (pcon->text)
                {
                    gkLuaManager &lua = gkLuaManager::getSingleton();
                    if (lua.hasScript(GKB_IDNAME(pcon->text)))
                        sc->setScript(lua.getScript(GKB_IDNAME(pcon->text)));
                    else
                        sc->setScript(lua.create(GKB_IDNAME(pcon->text)));
                }

            } break;


        }

        if (lc)
        {
            if ((bcont->flag & OB_DEBUGSTATE) || (bobj->scaflag & OB_DEBUGSTATE))
                lc->setDebugMask(bcont->state_mask);

            lc->setMask(bcont->state_mask);
            lc->setPriority((bcont->flag & CONT_PRIO) != 0);


            for (int i = 0; i < bcont->totlinks; ++i)
            {
                Blender::bActuator *a = bcont->links[i];
                if (a)
                {
                    gkLogicActuator *la = lnk->findActuator(a);
                    if (la)
                        lc->link(la);
                    else
                    {
                        ResolveObject robj = {0, 0, 0, 0};
                        robj.lcont  = lc;
                        robj.act    = a;
                        m_missing.push_back(robj);
                    }

                }
            }

            lnk->push(lc, bcont);
        }
    }


    for (Blender::bSensor *bsen = (Blender::bSensor *)bobj->sensors.first; bsen; bsen = bsen->next)
    {

        gkLogicSensor *ls = 0;
        switch (bsen->type)
        {
        case SENS_RAY:
            {
                gkRaySensor *rs = new gkRaySensor(gobj, lnk, bsen->name);
                ls = rs;
                Blender::bRaySensor *brs = (Blender::bRaySensor *)bsen->data;

                int axis = 0;
                switch (brs->axisflag)
                {
                case 1: {axis = gkRaySensor::RA_XPOS; break;}
                case 0: {axis = gkRaySensor::RA_YPOS; break;}
                case 2: {axis = gkRaySensor::RA_ZPOS; break;}
                case 3: {axis = gkRaySensor::RA_XNEG; break;}
                case 4: {axis = gkRaySensor::RA_YNEG; break;}
                case 5: {axis = gkRaySensor::RA_ZNEG; break;}
                }

                rs->setRange(brs->range);
                rs->setAxis(axis);

                if (brs->matname[0] != '\0')
                    rs->setMaterial(gkLogicLoader_formatText(brs->matname));
                if (brs->propname[0] != '\0')
                    rs->setProperty(gkLogicLoader_formatText(brs->propname));

            } break;
        case SENS_TOUCH:
        case SENS_COLLISION:
            {
                // tell the gk world we want to collect collsion information
                gkCollisionSensor *col = new gkCollisionSensor(gobj, lnk, bsen->name);
                ls = col;

                if (bsen->type == SENS_COLLISION)
                {
                    Blender::bCollisionSensor *cse = (Blender::bCollisionSensor *)bsen->data;

                    if (cse->mode & SENS_COLLISION_MATERIAL)
                        col->setMaterial(gkLogicLoader_formatText(cse->materialName));
                    else
                        col->setProperty(gkLogicLoader_formatText(cse->name));
                }
                else
                {
                    Blender::bTouchSensor *bto = (Blender::bTouchSensor *)bsen->data;
                    if (bto->ma)
                        col->setMaterial(GKB_IDNAME(bto->ma));
                }

            } break;

        case SENS_ALWAYS:
            {
                gkAlwaysSensor *asn = new gkAlwaysSensor(gobj, lnk, bsen->name);
                ls = asn;
            }
            break;
        case SENS_PROPERTY:
            {
                gkPropertySensor *psn = new gkPropertySensor(gobj, lnk, bsen->name);
                ls = psn;

                Blender::bPropertySensor *bps = (Blender::bPropertySensor *)bsen->data;

                psn->setProperty(gkLogicLoader_formatText(bps->name));
                psn->setValue(bps->value);

                switch(bps->type)
                {
                case SENS_PROP_EQUAL:       psn->setType(gkPropertySensor::PS_EQUAL);       break;
                case SENS_PROP_NEQUAL:      psn->setType(gkPropertySensor::PS_NEQUAL);      break;
                case SENS_PROP_INTERVAL:    psn->setType(gkPropertySensor::PS_INTERVAL);    break;
                case SENS_PROP_CHANGED:     psn->setType(gkPropertySensor::PS_CHANGED);     break;
                }
            }
            break;
        case SENS_MOUSE:
            {
                gkMouseSensor *ms = new gkMouseSensor(gobj, lnk, bsen->name);
                ls = ms;

                Blender::bMouseSensor *mse = (Blender::bMouseSensor *)bsen->data;

                int type = 0;
                if (mse->type == BL_SENS_MOUSE_LEFT_BUTTON)
                    type = gkMouseSensor::MOUSE_LEFT;
                else if (mse->type == BL_SENS_MOUSE_MIDDLE_BUTTON)
                    type = gkMouseSensor::MOUSE_MIDDLE;
                else if (mse->type == BL_SENS_MOUSE_RIGHT_BUTTON)
                    type = gkMouseSensor::MOUSE_RIGHT;
                else if (mse->type == BL_SENS_MOUSE_WHEEL_UP)
                    type = gkMouseSensor::MOUSE_WHEEL_UP;
                else if (mse->type == BL_SENS_MOUSE_WHEEL_DOWN)
                    type = gkMouseSensor::MOUSE_WHEEL_DOWN;
                else if (mse->type == BL_SENS_MOUSE_MOVEMENT)
                    type = gkMouseSensor::MOUSE_MOTION;
                else if (mse->type == BL_SENS_MOUSE_MOUSEOVER)
                    type = gkMouseSensor::MOUSE_MOUSE_OVER;
                else if (mse->type == BL_SENS_MOUSE_MOUSEOVER_ANY)
                    type = gkMouseSensor::MOUSE_MOUSE_OVER_ANY;

                ms->setType(type);
            }
            break;
        case SENS_KEYBOARD:
            {
                gkKeyboardSensor *ks = new gkKeyboardSensor(gobj, lnk, bsen->name);
                ls = ks;

                Blender::bKeyboardSensor *bks = (Blender::bKeyboardSensor *)bsen->data;
                ks->setKey(getKey(bks->key));
                ks->setMod0(getKey(bks->qual));
                ks->setMod1(getKey(bks->qual2));
            } break;
        }

        if (ls)
        {
            for (int i = 0; i < bsen->totlinks; ++i)
            {
                Blender::bController *c = bsen->links[i];
                if (c)
                {
                    gkLogicController *lc = lnk->findController(c);
                    if (lc)
                        ls->link(lc);
                    else
                    {
                        ResolveObject robj = {0, 0, 0, 0};
                        robj.lsens = ls;
                        robj.cont = c;
                        m_missing.push_back(robj);
                    }
                }
            }
            ls->setDetector(bsen->level != 0);
            ls->setTap(bsen->tap != 0);
            ls->setFrequency(bsen->freq);
            ls->invert(bsen->invert != 0);
            int pulse = 0;

            if (bsen->pulse & SENS_PULSE_REPEAT)
                pulse |= gkLogicSensor::PM_TRUE;
            if (bsen->pulse & SENS_NEG_PULSE_MODE)
                pulse |= gkLogicSensor::PM_FALSE;

            if (pulse == 0)
                pulse = gkLogicSensor::PM_IDLE;

            ls->setStartState(lnk->getState());
            ls->setMode(pulse);
            lnk->push(ls);
        }
    }
}


int gkLogicLoader::getKey(int kc)
{
#define gkLogicCase(x) case x: return KC_##x;
    switch (kc)
    {
        gkLogicCase(AKEY)
        gkLogicCase(BKEY)
        gkLogicCase(CKEY)
        gkLogicCase(DKEY)
        gkLogicCase(EKEY)
        gkLogicCase(FKEY)
        gkLogicCase(GKEY)
        gkLogicCase(IKEY)
        gkLogicCase(JKEY)
        gkLogicCase(KKEY)
        gkLogicCase(LKEY)
        gkLogicCase(MKEY)
        gkLogicCase(NKEY)
        gkLogicCase(OKEY)
        gkLogicCase(PKEY)
        gkLogicCase(QKEY)
        gkLogicCase(RKEY)
        gkLogicCase(SKEY)
        gkLogicCase(TKEY)
        gkLogicCase(UKEY)
        gkLogicCase(VKEY)
        gkLogicCase(WKEY)
        gkLogicCase(XKEY)
        gkLogicCase(YKEY)
        gkLogicCase(ZKEY)
        gkLogicCase(ZEROKEY)
        gkLogicCase(ONEKEY)
        gkLogicCase(TWOKEY)
        gkLogicCase(THREEKEY)
        gkLogicCase(FOURKEY)
        gkLogicCase(FIVEKEY)
        gkLogicCase(SIXKEY)
        gkLogicCase(SEVENKEY)
        gkLogicCase(EIGHTKEY)
        gkLogicCase(NINEKEY)
        gkLogicCase(CAPSLOCKKEY)
        gkLogicCase(LEFTCTRLKEY)
        gkLogicCase(LEFTALTKEY)
        gkLogicCase(RIGHTALTKEY)
        gkLogicCase(RIGHTCTRLKEY)
        gkLogicCase(RIGHTSHIFTKEY)
        gkLogicCase(LEFTSHIFTKEY)
        gkLogicCase(ESCKEY)
        gkLogicCase(TABKEY)
        gkLogicCase(RETKEY)
        gkLogicCase(SPACEKEY)
        gkLogicCase(LINEFEEDKEY)
        gkLogicCase(BACKSPACEKEY)
        gkLogicCase(DELKEY)
        gkLogicCase(SEMICOLONKEY)
        gkLogicCase(PERIODKEY)
        gkLogicCase(COMMAKEY)
        gkLogicCase(QUOTEKEY)
        gkLogicCase(ACCENTGRAVEKEY)
        gkLogicCase(MINUSKEY)
        gkLogicCase(SLASHKEY)
        gkLogicCase(BACKSLASHKEY)
        gkLogicCase(EQUALKEY)
        gkLogicCase(LEFTBRACKETKEY)
        gkLogicCase(RIGHTBRACKETKEY)
        gkLogicCase(LEFTARROWKEY)
        gkLogicCase(DOWNARROWKEY)
        gkLogicCase(RIGHTARROWKEY)
        gkLogicCase(UPARROWKEY)
        gkLogicCase(PAD0)
        gkLogicCase(PAD1)
        gkLogicCase(PAD2)
        gkLogicCase(PAD3)
        gkLogicCase(PAD4)
        gkLogicCase(PAD5)
        gkLogicCase(PAD6)
        gkLogicCase(PAD7)
        gkLogicCase(PAD8)
        gkLogicCase(PAD9)
        gkLogicCase(PADPERIOD)
        gkLogicCase(PADSLASHKEY)
        gkLogicCase(PADASTERKEY)
        gkLogicCase(PADMINUS)
        gkLogicCase(PADENTER)
        gkLogicCase(PADPLUSKEY)
        gkLogicCase(F1KEY)
        gkLogicCase(F2KEY)
        gkLogicCase(F3KEY)
        gkLogicCase(F4KEY)
        gkLogicCase(F5KEY)
        gkLogicCase(F6KEY)
        gkLogicCase(F7KEY)
        gkLogicCase(F8KEY)
        gkLogicCase(F9KEY)
        gkLogicCase(F10KEY)
        gkLogicCase(F11KEY)
        gkLogicCase(F12KEY)
        gkLogicCase(PAUSEKEY)
        gkLogicCase(INSERTKEY)
        gkLogicCase(HOMEKEY)
        gkLogicCase(PAGEUPKEY)
        gkLogicCase(PAGEDOWNKEY)
        gkLogicCase(ENDKEY)
        gkLogicCase(UNKNOWNKEY)
        gkLogicCase(COMMANDKEY)
        gkLogicCase(GRLESSKEY)
    }

    if (kc == _HKEY)
        return KC_HKEY;

#undef gkLogicCase
    return KC_NONE;
}


gkSkyBoxGradient::gkSkyBoxGradient(Blender::World *wo) :
	m_world(wo)
{
}


gkSkyBoxGradient::~gkSkyBoxGradient()
{
}


void gkSkyBoxGradient::fill(Ogre::Image &ima, int size, const Ogre::ColourValue &v, const Ogre::PixelFormat &fmt)
{
	int x, sqs;
	unsigned int *ptr, r;

	sqs = size * size;
	unsigned char* tbuf = new unsigned char[sqs*4];

	Ogre::PixelUtil::packColour(v, fmt, &r);
	ptr = (unsigned int*)tbuf;
	x = 0;
	do
		ptr[x++] = r;
	while (x < sqs);

	Ogre::DataStreamPtr imaStream(new Ogre::MemoryDataStream(tbuf, sqs*4));
	ima.loadRawData(imaStream, size, size, 1, fmt);
	delete []tbuf;

}


void gkSkyBoxGradient::grad(Ogre::Image &ima, int x1, int y1, int x2, int y2, int size, 
						  const Ogre::ColourValue &s, const Ogre::ColourValue &e, 
						  const Ogre::PixelFormat &fmt)
{
	int x, y, sqs;
	float A, B, C, C1, C2;
	unsigned int *ptr;

	x1 = gkClamp<int>(x1, 0, size); x2 = gkClamp<int>(x2, 0, size);
	y1 = gkClamp<int>(y1, 0, size); y2 = gkClamp<int>(y2, 0, size);

	A = float(x2 - x1); 
	B = float(y2 - y1);
	C1 = A * x1 + B * y1;
	C2 = A * x2 + B * y2;
	sqs = size * size;


	float scale = 1.f / float(sqs);
	unsigned char* tbuf = new unsigned char[sqs*4];
	ptr = (unsigned int*)tbuf;
	Ogre::ColourValue c;

	for (x = 0; x<size; ++x)
	{
		for (y = 0; y<size; y++)
		{
			C = A * (x+0.5f) + B * (y+0.5f);

			if (C <= C1) c = s;
			else if (C >= C2) c = e;
			else c = (s * (C2 - C) + e * (C - C1))/(C2 - C1);

			unsigned int dest=0;
			Ogre::PixelUtil::packColour(c, fmt, &dest);
			(*ptr++) = dest;
		}
	}

	Ogre::DataStreamPtr stream(new Ogre::MemoryDataStream(tbuf, sqs*4));
	ima.loadRawData(stream, size, size, 1, fmt);
	delete []tbuf;
}


void gkSkyBoxGradient::loadResource(Ogre::Resource* resource)
{
	Ogre::Texture *tex = (Ogre::Texture*)resource;

	Ogre::ColourValue zen(m_world->zenr, m_world->zeng, m_world->zenb);
	Ogre::ColourValue hor(m_world->horr, m_world->horg, m_world->horb);

	Ogre::PixelFormat fmt = Ogre::PF_R8G8B8A8;

	tex->setFormat(fmt);
	tex->setUsage(Ogre::TU_DEFAULT);
	tex->setTextureType(Ogre::TEX_TYPE_CUBE_MAP);
	tex->setNumMipmaps(0);

	unsigned int size = 64;
	unsigned int sf = 16; // offset 


	Ogre::Image zeni, hori;

	Ogre::Image zhf, zhb, zhr, zhl;

	fill(zeni, size, zen, fmt);
	fill(hori, size, hor, fmt);

	unsigned int ma = size-sf;
	unsigned int mi = sf;

	grad(zhf, mi, mi, ma, mi, size, zen, hor, fmt);
	grad(zhb, mi, mi, ma, mi, size, hor, zen, fmt);
	grad(zhr, mi, mi, mi, ma, size, hor, zen, fmt);
	grad(zhl, mi, mi, mi, ma, size, zen, hor, fmt);

	Ogre::ConstImagePtrList ptrs;

	// not the correct order 
	ptrs.push_back(&zhr);
	ptrs.push_back(&zhl);
	ptrs.push_back(&zhf);
	ptrs.push_back(&zhb);
	ptrs.push_back(&hori);
	ptrs.push_back(&zeni);
	
	tex->_loadImages(ptrs);

}
