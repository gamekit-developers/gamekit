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


#define GK_MAX_TEXTURE 18
class gkMesh;

enum gkBlendType
{
	GK_BT_MIXTURE,
	GK_BT_MULTIPLY,
	GK_BT_ADDITIVE,
	GK_BT_SUBTRACT,
	GK_BT_SCREEN,
	GK_BT_DARKEN,
	GK_BT_LIGHTEN,
	GK_BT_COLOR
};

class gkTextureProperties
{
public:

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
		TM_NORMAL       = (1 << 8)
	};

	enum TexMode
	{
		TX_NEGATIVE		= (1 << 0),
		TX_STENCIL		= (1 << 1),
		TX_RGBTOINTEN	= (1 << 2),
		TX_OBJ_SPACE	= (1 << 3)
	};


	enum Type
	{
		IMT_IMAGE,
		IMT_CUBEMAP
	};

public:

	gkTextureProperties()
		:   m_name(),
		    m_image(),
		    m_color(1.f, 1.f, 1.f),
		    m_layer(0),
		    m_type(IMT_IMAGE),
		    m_blend(GK_BT_MIXTURE),
		    m_mode(0),
			m_texmode(0),
		    m_mix(1.f),
			m_normalFactor(0),
			m_diffuseColorFactor(0),
			m_diffuseAlpahFactor(0),
			m_speculaColorFactor(0),
			m_speculaHardFactor(0)
	{
	}


	gkString    m_name;
	gkString    m_image;
	gkColor     m_color;
	int         m_layer;
	int         m_type;
	int         m_blend;
	int         m_mode;
	int			m_texmode;
	gkScalar    m_mix;
	gkScalar    m_normalFactor;
	gkScalar    m_diffuseColorFactor;
	gkScalar    m_diffuseAlpahFactor;
	gkScalar    m_speculaColorFactor;
	gkScalar    m_speculaHardFactor;
};


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
		MA_HASRAMPBLEND		= (1 << 9)
	};


public:
	gkMaterialProperties()
		:   m_name(),
		    m_mode(MA_RECEIVESHADOWS | MA_LIGHTINGENABLED | MA_DEPTHWRITE),
			m_rblend(GK_BT_MIXTURE),
		    m_diffuse(1.f, 1.f, 1.f, 1.f),
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
	int						m_rblend;
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
	gkTextureProperties     m_textures[GK_MAX_TEXTURE];
};


class gkCameraProperties
{
public:

	enum Type
	{
		CA_PERSPECTIVE,
		CA_ORTHOGRAPHIC
	};

public:


	gkCameraProperties()
		:   m_clipstart(0.1f),
		    m_clipend(100.f),
		    m_fov(49.13f),
		    m_orthoscale(7.314f),
		    m_start(false),
		    m_type(CA_ORTHOGRAPHIC)
	{
	}

	gkScalar    m_clipstart;
	gkScalar    m_clipend;
	gkScalar    m_fov;
	gkScalar    m_orthoscale;
	bool        m_start;
	int         m_type;
};


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

	gkMesh*         m_mesh;
	bool            m_casts;
	gkString        m_source;
	gkString        m_startPose;
};


class gkLightProperties
{
public:

	enum Type
	{
		LI_POINT,
		LI_SPOT,
		LI_DIR
	};


public:

	gkLightProperties()
		:   m_diffuse(0.f, 0.f, 0.f),
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


enum gkPhysicsType
{
	GK_NO_COLLISION,
	GK_STATIC,
	GK_DYNAMIC,
	GK_RIGID,
	GK_SOFT
};


enum gkPhysicsMode
{
	GK_NO_SLEEP       = (1 << 0),
	GK_CONTACT        = (1 << 1),
	GK_LOCK_LINV_X    = (1 << 2),
	GK_LOCK_LINV_Y    = (1 << 3),
	GK_LOCK_LINV_Z    = (1 << 4),
	GK_LOCK_ANGV_X    = (1 << 5),
	GK_LOCK_ANGV_Y    = (1 << 6),
	GK_LOCK_ANGV_Z    = (1 << 7),
	GK_COMPOUND		  = (1 << 8),
	GK_COMPOUND_CHILD = (1 << 9)
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

enum gkPhysicsConstraintType
{
	GK_BALL_CONSTRAINT,
	GK_HINGE_CONSTRAINT,
	GK_CONETWIST_CONSTRAINT,
	GK_VEHICLE_CONSTRAINT,
	GK_D6_CONSTRAINT,
	GK_SLIDER_CONSTRAINT,
	GK_CONTACT_CONSTRAINT
};

class gkPhysicsConstraintProperties
{
public:

	gkPhysicsConstraintProperties()
		:	m_type(GK_BALL_CONSTRAINT),
			m_pivot(0.f, 0.f, 0.f),
			m_axis(0.f, 0.f, 0.f),
			m_flag(0),
			m_disableLinkedCollision(true)
	{
		for (int i = 0; i < 6; i++) m_minLimit[i] = m_maxLimit[i] = 0.f;
	}

	gkString	m_target;

	int			m_type;
	gkVector3	m_pivot;
	gkVector3	m_axis;

	gkScalar	m_minLimit[6];
	gkScalar	m_maxLimit[6];	
	
	int			m_flag;
	bool		m_disableLinkedCollision;

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
	utArray<gkPhysicsConstraintProperties> m_constraints;

	GK_INLINE bool isContactListener(void)    const { return (m_mode & GK_CONTACT) != 0; }
	GK_INLINE bool isDosser(void)             const { return (m_mode & GK_NO_SLEEP) == 0; }
	GK_INLINE bool isCompound(void)	          const { return (m_mode & GK_COMPOUND) != 0; }  
	GK_INLINE bool isCompoundChild(void)	  const { return (m_mode & GK_COMPOUND_CHILD) != 0; }
	GK_INLINE bool isRigidOrDynamic(void)     const { return m_type == GK_DYNAMIC || m_type == GK_RIGID; }
	GK_INLINE bool isRigidOrStatic(void)      const { return m_type == GK_STATIC  || m_type == GK_RIGID; }
	GK_INLINE bool isPhysicsObject(void)      const { return m_type != GK_NO_COLLISION; }
	GK_INLINE bool isStatic(void)             const { return m_type == GK_STATIC; }
	GK_INLINE bool isDynamic(void)            const { return m_type == GK_DYNAMIC; }
	GK_INLINE bool isRigid(void)              const { return m_type == GK_RIGID; }
	GK_INLINE bool isSoft(void)               const { return m_type == GK_SOFT; }
	GK_INLINE bool isMeshShape(void)          const { return m_shape >= SH_CONVEX_TRIMESH; }
	GK_INLINE bool hasPhysicsConstraint(void) const { return getConstraintCount() != 0; }
	GK_INLINE bool isLinkedToOther(void)      const { return hasPhysicsConstraint() || isCompoundChild(); }

	GK_INLINE UTsize getConstraintCount(void) const { return m_constraints.size(); }
	GK_INLINE const  gkPhysicsConstraintProperties& getConstraint(UTsize i) const { return m_constraints[i]; }	
};


enum gkGameObjectTypes
{
	GK_OB_NULL = 0,
	GK_CAMERA,
	GK_LIGHT,
	GK_ENTITY,
	GK_OBJECT,
	GK_SKELETON,
	GK_PARTICLES
};

enum gkGameObjectMode
{
	GK_GHOST         = (1 << 0),  // gkCharacter controller
	GK_ACTOR         = (1 << 1),  // Sensor listener
	GK_INVISIBLE     = (1 << 2),  // Marked invisible
	GK_OCCLUDER      = (1 << 3),  // Occluder
	GK_HAS_LOGIC     = (1 << 4),  // Has game logic
	GK_IMMOVABLE     = (1 << 5),  // Marked as an immovable object
	GK_STATIC_GEOM   = (1 << 6)   // Is part of static batch geometry.
};


class gkGameObjectProperties
{
public:
	gkGameObjectProperties()
		:   m_transform(),
		    m_mode(0),
		    m_state(0),
		    m_physics(),
		    m_findPathFlag(0xFF),
		    m_parent("")
	{
		m_transform.setIdentity();
	}


	gkTransformState    m_transform;
	int                 m_mode;
	int                 m_state;
	gkPhysicsProperties m_physics;
	unsigned char       m_findPathFlag;
	gkString            m_parent;
	utArray<gkString>	m_particleObjs;


	GK_INLINE bool isContactListener(void)    const { return m_physics.isContactListener(); }
	GK_INLINE bool isDosser(void)             const { return m_physics.isDosser(); }	
	GK_INLINE bool isRigidOrDynamic(void)     const { return m_physics.isRigidOrDynamic();}
	GK_INLINE bool isRigidOrStatic(void)      const { return m_physics.isRigidOrStatic();}
	
	GK_INLINE bool isPhysicsObject(void)      const { return m_physics.isPhysicsObject(); }
	GK_INLINE bool isStatic(void)             const { return m_physics.isStatic(); }
	GK_INLINE bool isDynamic(void)            const { return m_physics.isDynamic(); }
	GK_INLINE bool isRigid(void)              const { return m_physics.isRigid(); }
	GK_INLINE bool isSoft(void)               const { return m_physics.isSoft(); }
	GK_INLINE bool isMeshShape(void)          const { return m_physics.isMeshShape(); }

	GK_INLINE bool isActor(void)              const { return (m_mode & GK_ACTOR)     != 0; }
	GK_INLINE bool isInvisible(void)          const { return (m_mode & GK_INVISIBLE) != 0; }
	GK_INLINE bool isOccluder(void)           const { return (m_mode & GK_OCCLUDER)  != 0; }
	GK_INLINE bool isGhost(void)              const { return (m_mode & GK_GHOST)     != 0; }

	GK_INLINE bool hasParticles(void)         const { return !m_particleObjs.empty(); }
};

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
		    m_color(0.f, 0.f, 0.f)
	{
	}

	int         m_mode;
	gkScalar    m_start;
	gkScalar    m_end;
	gkScalar    m_intensity;
	gkColor     m_color;

};

class gkSoundSceneProperties
{
public:
	enum DistanceModel
	{
		DM_NONE = 0,
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

	int         m_distModel;
	gkScalar    m_dopplerFactor;
	gkScalar    m_sndSpeed;

};



class gkSceneMaterial
{
public:

	enum Type
	{
		FLAT,
		LINEAR,
		RADIAL,
		REFLECTED,
	};

public:

	gkSceneMaterial()
		:    m_type(FLAT),
		     m_name(""),
		     m_horizon(0, 0, 0, 1),
		     m_zenith(0, 0, 0, 1),
		     m_ambient(0, 0, 0, 1),
		     m_distance(0.f)
	{
	}

	int      m_type;
	gkString m_name;
	gkColor  m_horizon;
	gkColor  m_zenith;
	gkColor  m_ambient;
	gkScalar m_distance;
};


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
		    m_gravity(0.f, 0.f, -9.81f),
		    m_material(),
		    m_fog()
	{
	}

	int             m_manager;
	gkVector3       m_gravity;
	gkSceneMaterial m_material;
	gkFogParams     m_fog;
};


class gkParticleSettingsProperties
{
public:
	enum Physics
	{
		PHY_NONE,
		PHY_NEWTON,
		PHY_KEYED,
		PHY_BOIDS,	
		PHY_FLUID
	};

	enum EmitFrom
	{
		EF_VERTS,
		EF_FACES,
		EF_VOLUME,
		EF_PARTICLES
	};

	enum Render
	{
		R_NONE,
		R_HALO,
		R_LINE,
		R_PATH,
		R_OBJECT,
		R_GROUP,
		R_BILLBOARD
	};


public:
	gkString		m_name;	
	
	int				m_amount;		//emission
	int				m_emitfrom;
	float			m_lifetime;
	float			m_start;
	float			m_end;
	float			m_random;
	float			m_jitter;
	int				m_render;

	gkVector3		m_velocity;		//velocity
	float			m_velNormal;	
	float			m_velTanget;

	int				m_phyType;		//physics
	float			m_size;			
	float			m_mass;
	float			m_sizeRandom;

	int				m_material;		//render
	int				m_trailCount;
	bool			m_drawEmitter;

	float			m_gravity;		//field weights


public:
	gkParticleSettingsProperties()
		:	m_amount(1000),
			m_emitfrom(EF_VERTS),
			m_lifetime(50.f),
			m_start(1.f),
			m_end(200.f),
			m_random(0.f),
			m_jitter(1.f),
			m_render(R_HALO),

			m_velocity(0.f,0.f,0.f),
			m_velNormal(1.f),
			m_velTanget(0.f),

			m_phyType(PHY_NEWTON),
			m_size(0.05f),
			m_mass(1.f),
			m_sizeRandom(0.f),

			m_material(0),
			m_trailCount(1),
			m_drawEmitter(false),

			m_gravity(1.f)
	{
	}
};

class gkParticleSystemProperties
{
public:
	gkString		m_name;
	int				m_seed;
	gkString		m_settings;
	gkString		m_material;
	gkMesh*			m_mesh;

public:
	gkParticleSystemProperties()
		:	m_seed(0),
			m_mesh(0)
	{
	}
};


#endif//_gkSerialize_h_
