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
#ifndef _gkSerialize_h_
#define _gkSerialize_h_

#include "gkMathUtils.h"
#include "gkTransformState.h"
#include "gkString.h"
#include "Utils/utString.h"


#define GK_MAX_TEXTURE 18
class gkMesh;


// ----------------------------------------------------------------------------
//
//                              Texture Types
//
// ----------------------------------------------------------------------------


class gkTexureProperties
{
public:

	enum Blend
	{
		BM_MIXTURE,
		BM_MULTIPLY,
		BM_ADDITIVE,
		BM_SUBTRACT,
	};

	enum Mode
	{
		TM_SPLAT        = (1 << 0),
		TM_INVERT       = (1 << 1),
		TM_ALPHA        = (1 << 2),
		TM_SPECULAR     = (1 << 3),
		TM_EMMISIVE     = (1 << 4),
		TM_AMBIENT      = (1 << 5),
		TM_REFRACTION   = (1 << 6),
		TM_MIRROR       = (1 << 7),
		TM_NORMAL       = (1 << 8),
	};


	enum Type
	{
		IMT_IMAGE,
		IMT_CUBEMAP,
	};

public:

	gkTexureProperties()
		:   m_name(),
		    m_image(),
		    m_color(1.f, 1.f, 1.f),
		    m_layer(0),
		    m_type(IMT_IMAGE),
		    m_blend(BM_MULTIPLY),
		    m_mode(0),
		    m_mix(1.f)
	{
	}


	gkString    m_name;
	gkString    m_image;
	gkColor     m_color;
	int         m_layer;
	int         m_type;
	int         m_blend;
	int         m_mode;
	gkScalar    m_mix;
};


// ----------------------------------------------------------------------------
//
//                              Material Types
//
// ----------------------------------------------------------------------------


class gkMaterialProperties
{
public:

	enum Mode
	{
		MA_RECEIVESHADOWS   = (1 << 0),
		MA_LIGHTINGENABLED  = (1 << 1),
		MA_WIREFRAME        = (1 << 2),
		MA_DEPTHWRITE       = (1 << 3),
		MA_INVISIBLE        = (1 << 4),
		MA_TWOSIDE          = (1 << 5),
		MA_ALPHABLEND       = (1 << 6),
		MA_ADDITIVEBLEND    = (1 << 7),
		MA_HASFACETEX       = (1 << 8),
	};


public:
	gkMaterialProperties()
		:   m_name(),
		    m_mode(MA_RECEIVESHADOWS | MA_LIGHTINGENABLED | MA_DEPTHWRITE),
		    m_diffuse(1.f,1.f,1.f,1.f),
		    m_specular(0.f, 0.f, 0.f, 0.f),
		    m_hardness(0.f),
		    m_refraction(1.f),
		    m_emissive(.0f),
		    m_ambient(.5f),
		    m_spec(.0f),
		    m_alpha(1.f),
		    m_depthOffset(0.f),
		    m_totaltex(0)
	{
	}

	gkString                m_name;
	unsigned int            m_mode;
	gkColor                 m_diffuse;
	gkColor                 m_specular;
	gkScalar                m_hardness;
	gkScalar                m_refraction;
	gkScalar                m_emissive;
	gkScalar                m_ambient;
	gkScalar                m_spec;
	gkScalar                m_alpha;
	gkScalar                m_depthOffset;
	int                     m_totaltex;
	gkTexureProperties      m_textures[GK_MAX_TEXTURE];


};


// ----------------------------------------------------------------------------
//
//                              Camera Types
//
// ----------------------------------------------------------------------------

class gkCameraProperties
{
public:

	enum Type
	{
		CA_PERSPECTIVE,
		CA_ORTHOGRAPHIC,
	};

public:


	gkCameraProperties()
		:   m_clipstart(0.1f),
		    m_clipend(100.f),
		    m_fov(49.13f),
		    m_start(false),
		    m_type(CA_ORTHOGRAPHIC)
	{
	}

	gkScalar    m_clipstart;
	gkScalar    m_clipend;
	gkScalar    m_fov;
	bool        m_start;
	int         m_type;
};


// ----------------------------------------------------------------------------
//
//                              Entity Types
//
// ----------------------------------------------------------------------------


class gkEntityProperties
{
public:
	gkEntityProperties()
		:   m_mesh(0),
		    m_casts(false),
		    m_source(""),
		    m_startPose("")
	{
	}

	gkMesh         *m_mesh;
	bool            m_casts;
	gkString        m_source;
	gkString        m_startPose;
};



// ----------------------------------------------------------------------------
//
//                              Light Types
//
// ----------------------------------------------------------------------------

class gkLightProperties
{
public:

	enum Type
	{
		LI_POINT,
		LI_SPOT,
		LI_DIR,
	};


public:

	gkLightProperties()
		:   m_diffuse(0.f,0.f,0.f),
		    m_specular(0.f, 0.f, 0.f),
		    m_type(LI_POINT),
		    m_spot(30.f, 40.f),
		    m_direction(0.f, 0.f, -1.f),
		    m_power(1.f),
		    m_falloff(1.f),
		    m_range(10000.f),
		    m_constant(1.f),
		    m_linear(0.f),
		    m_quadratic(0.f),
		    m_casts(false),
		    m_extra(false)
	{
		m_param[0] = m_param[1] = 0;
		m_param[2] = m_param[3] = 0;
	}

	gkColor             m_diffuse;
	gkColor             m_specular;
	int                 m_type;
	gkVector2           m_spot;
	gkVector3           m_direction;
	gkScalar            m_power;
	gkScalar            m_falloff;
	gkScalar            m_range;
	gkScalar            m_constant;
	gkScalar            m_linear;
	gkScalar            m_quadratic;
	bool                m_casts;
	bool                m_extra;
	gkScalar            m_param[4];
};



// ----------------------------------------------------------------------------
//
//                              Physics Types
//
// ----------------------------------------------------------------------------
enum gkPhysicsType
{
	GK_NO_COLLISION,
	GK_STATIC,
	GK_DYNAMIC,
	GK_RIGID,
	GK_SOFT,
};


enum gkPhysicsMode
{
	GK_NO_SLEEP     = (1 << 0),
	GK_CONTACT      = (1 << 1),
	GK_LOCK_LINV_X  = (1 << 2),
	GK_LOCK_LINV_Y  = (1 << 3),
	GK_LOCK_LINV_Z  = (1 << 4),
	GK_LOCK_ANGV_X  = (1 << 5),
	GK_LOCK_ANGV_Y  = (1 << 6),
	GK_LOCK_ANGV_Z  = (1 << 7),
};

enum gkPhysicsShape
{
	SH_BOX,
	SH_CONE,
	SH_CYLINDER,
	SH_SPHERE,
	SH_CONVEX_TRIMESH,
	SH_GIMPACT_MESH,
	SH_BVH_MESH,
};


class gkPhysicsProperties
{
public:

	gkPhysicsProperties()
		:   m_type(GK_NO_COLLISION),
		    m_mode(0),
		    m_shape(SH_SPHERE),
		    m_margin(.06f),
		    m_cpt(1.f),
		    m_mass(0.f),
		    m_radius(1.f),
		    m_linearDamp(.025f),
		    m_angularDamp(.18f),
		    m_formFactor(.4f),
		    m_minVel(0.f),
		    m_maxVel(0.f),
		    m_restitution(0.f),
		    m_friction(0.5f)
	{
	}

	int         m_type;
	int         m_mode;
	int         m_shape;
	gkScalar    m_margin;
	gkScalar    m_cpt;
	gkScalar    m_mass;
	gkScalar    m_radius;
	gkScalar    m_linearDamp;
	gkScalar    m_angularDamp;
	gkScalar    m_formFactor;
	gkScalar    m_minVel;
	gkScalar    m_maxVel;
	gkScalar    m_restitution;
	gkScalar    m_friction;

	bool isContactListener(void)    const { return (m_mode & GK_CONTACT) != 0;}
	bool isDosser(void)             const { return (m_mode & GK_NO_SLEEP) == 0;}
	bool isRigidOrDynamic(void)     const { return m_type == GK_DYNAMIC || m_type == GK_RIGID;}
	bool isRigidOrStatic(void)      const { return m_type == GK_STATIC  || m_type == GK_RIGID;}
	bool isPhysicsObject(void)      const { return m_type != GK_NO_COLLISION; }
	bool isStatic(void)             const { return m_type == GK_STATIC; }
	bool isDynamic(void)            const { return m_type == GK_DYNAMIC; }
	bool isRigid(void)              const { return m_type == GK_RIGID; }
	bool isSoft(void)               const { return m_type == GK_SOFT; }
	bool isMeshShape(void)          const { return m_shape >= SH_CONVEX_TRIMESH;}
};


// ----------------------------------------------------------------------------
//
//                              Game Object Types
//
// ----------------------------------------------------------------------------

typedef enum gkGameObjectTypes
{
	GK_OB_NULL = 0,
	GK_CAMERA,
	GK_LIGHT,
	GK_ENTITY,
	GK_OBJECT,
	GK_SKELETON,
} gkGameObjectTypes;

enum gkGameObjectMode
{
	GK_GHOST        = (1 << 0),
	GK_ACTOR        = (1 << 1),
	GK_INVISIBLE    = (1 << 2),
	GK_OCCLUDER     = (1 << 3),
	GK_SHOWBB       = (1 << 4),
};



class gkGameObjectProperties
{
public:
	gkGameObjectProperties()
		:   m_transform(),
		    m_mode(0),
		    m_state(0),
		    m_physics(),
		    m_findPathFlag(0xFF)
	{
		m_transform.setIdentity();
	}


	gkTransformState    m_transform;
	int                 m_mode;
	int                 m_state;
	gkPhysicsProperties m_physics;
	unsigned char		m_findPathFlag;


	bool isContactListener(void)    const { return m_physics.isContactListener(); }
	bool isDosser(void)             const { return m_physics.isDosser(); }
	bool isRigidOrDynamic(void)     const { return m_physics.isRigidOrDynamic();}
	bool isRigidOrStatic(void)      const { return m_physics.isRigidOrStatic();}

	bool isPhysicsObject(void)      const { return m_physics.isPhysicsObject(); }
	bool isStatic(void)             const { return m_physics.isStatic(); }
	bool isDynamic(void)            const { return m_physics.isDynamic(); }
	bool isRigid(void)              const { return m_physics.isRigid(); }
	bool isSoft(void)               const { return m_physics.isSoft(); }
	bool isActor(void)              const { return (m_mode & GK_ACTOR)      != 0; }
	bool isInvisible(void)          const { return (m_mode & GK_INVISIBLE)  != 0; }
	bool isOccluder(void)           const { return (m_mode & GK_OCCLUDER)   != 0; }
	bool isGhost(void)              const { return (m_mode & GK_GHOST)      != 0; }
};


// ----------------------------------------------------------------------------
//
//                              World Types
//
// ----------------------------------------------------------------------------



class gkFogParams
{
public:
	enum FogMode
	{
		FM_NONE,
		FM_LIN,
		FM_EXP,
		FM_QUAD,
	};

public:
	gkFogParams()
		:   m_mode(FM_NONE),
		    m_start(0.f),
		    m_end(0.f),
		    m_intensity(0.f),
		    m_color(0.f,0.f,0.f)
	{
	}

	int         m_mode;
	gkScalar    m_start;
	gkScalar    m_end;
	gkScalar    m_intensity;
	gkColor     m_color;

};

// ----------------------------------------------------------------------------
//
//                              Sound Scene Types
//
// ----------------------------------------------------------------------------
class gkSoundSceneProperties
{
public:
	enum DistanceModel
	{
		DM_NONE =0,
		DM_INVERSE,
		DM_LINEAR,
		DM_EXPONENT,
		DM_INVERSE_CLAMP,
		DM_LINEAR_CLAMP,
		DM_EXPONENT_CLAMP,
	};

public:

	gkSoundSceneProperties()
		:    m_distModel(DM_INVERSE_CLAMP),
		     m_dopplerFactor(1.f),
		     m_sndSpeed(343.3f)
	{
	}

	int			m_distModel;
	gkScalar	m_dopplerFactor;
	gkScalar	m_sndSpeed;

};



// ----------------------------------------------------------------------------
//
//                              Scene Types
//
// ----------------------------------------------------------------------------


class gkSceneProperties
{
public:


	enum Manager
	{
		MA_GENERIC,
		MA_OCTREE,
	};

public:
	gkSceneProperties()
		:   m_manager(MA_GENERIC),
		    m_world(0.f, 0.f, 0.f),
		    m_ambient(0.f, 0.f, 0.f),
		    m_gravity(0.f, 0.f, -9.81f),
		    m_skyMat(""),
		    m_skyOri(1.f, 0.f, 0.f, 0.f),
		    m_skyDist(0.f),
		    m_fog()
	{
	}

	int             m_manager;
	gkColor         m_world;
	gkColor         m_ambient;
	gkVector3       m_gravity;
	gkString        m_skyMat;
	gkQuaternion    m_skyOri;
	gkScalar        m_skyDist;
	gkFogParams     m_fog;
};


#endif//_gkSerialize_h_
