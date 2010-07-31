/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gkGameObject_h_
#define _gkGameObject_h_

#include "gkObject.h"
#include "OgreSceneNode.h"
#include "gkMathUtils.h"
#include "gkVariable.h"
#include "gkTransformState.h"
#include "gkSerialize.h"

class gkRigidBody;
class gkCharacter;
class gkLogicLink;

// Base class for all game objects
class gkGameObject : public gkObject
{
public:
	typedef utListClass<gkConstraint>                   ConstraintList;
	typedef utListIterator<ConstraintList>              ConstraintIterator;
	typedef utHashTable<gkHashedString, gkVariable *>    VariableMap;

	typedef utList<gkGameObject *> GameObjects;
	typedef utListIterator<GameObjects> GameObjectIterator;


	// life of a temporary object
	struct LifeSpan
	{
		int tick;
		int timeToLive;
	};

public:

	gkGameObject(gkScene *scene, const gkString &name, gkGameObjectTypes type);
	virtual ~gkGameObject();

	GK_INLINE gkScene                   *getOwner(void)                     {return m_scene;}
	GK_INLINE Ogre::SceneNode           *getNode(void)                      {return m_node;}
	GK_INLINE gkGameObjectTypes         getType(void)                       {return m_type;}
	GK_INLINE gkGameObjectProperties    &getProperties(void)                {return m_baseProps;}
	GK_INLINE bool                      isClone(void)                       {return m_isClone;}

	// subtype access
	GK_INLINE gkEntity      *getEntity(void)    {return m_type == GK_ENTITY ?   (gkEntity *)this : 0; }
	GK_INLINE gkCamera      *getCamera(void)    {return m_type == GK_CAMERA ?   (gkCamera *)this : 0; }
	GK_INLINE gkLight       *getLight(void)     {return m_type == GK_LIGHT ?    (gkLight *)this : 0; }
	GK_INLINE gkSkeleton    *getSkeleton(void)  {return m_type == GK_SKELETON ? (gkSkeleton *)this : 0; }
	GK_INLINE gkGameObject  *getParent(void)    {return m_parent;}

	void setParent(gkGameObject *par);

	void notifyUpdate(void);

	// constriants
	GK_INLINE ConstraintIterator    getConstraintIterator(void)  { return ConstraintIterator(m_constraints); }
	GK_INLINE bool                  hasConstraints(void)         { return !m_constraints.empty(); }
	void                            addConstraint(gkConstraint *c);
	void                            destroyConstraints(void);
	void                            applyConstraints(void);


	GK_INLINE LifeSpan &getLifeSpan(void)               {return m_life;}
	GK_INLINE void      setLifeSpan(const LifeSpan &v)  {m_life = v;}

	// Object level blending
	void blendTransform(gkScalar blend);

	GK_INLINE bool hasMoved(void) {return m_outOfDate;}

	// applies transformation to node & rigid body
	void applyTransformState(const gkTransformState &newstate);

	// layers
	GK_INLINE void setActiveLayer(bool truth)   {m_activeLayer = truth; }
	GK_INLINE bool isInActiveLayer(void)        {return m_activeLayer; }

	virtual gkObject *clone(const gkString &name);

	// grouping

	GK_INLINE bool isInstance(void)                             {return m_instance != 0;}
	GK_INLINE bool isGroupOwner(void)                           {return m_groupOwn != 0;}
	GK_INLINE void setGroupOwner(bool v)                        {m_groupOwn = v;}
	GK_INLINE gkGameObjectInstance *getGroupInstance(void)      {return m_instance;}
	GK_INLINE gkGameObjectGroup    *getGroup(void)              {return m_groupRef;}


	gkGameObject    *getGroupParent(void);
	void            attachToGroup(gkGameObjectGroup *g);
	void            detachFromGroup(void);
	void            attachToGroupInstance(gkGameObjectInstance *g);


	// physics
	GK_INLINE void attachRigidBody(gkRigidBody *body) {m_rigidBody = body;}
	GK_INLINE gkRigidBody *getAttachedBody() {return m_rigidBody;}
	GK_INLINE void attachCharacter(gkCharacter *character) {m_character = character;}
	GK_INLINE gkCharacter *getAttachedCharacter() {return m_character;}
	gkObject *getAttachedObject();
	btCollisionObject *getCollisionObject();


	// variables
	gkVariable *createVariable(const gkString &name, bool debug);
	gkVariable *getVariable(const gkString &name);
	bool        hasVariable(const gkString &name);
	void        clearVariables(void);

	// Gets cached local transform
	GK_INLINE const gkTransformState   &getLocalTransform(void)     {return m_cur;}
	// Gets cached local orientation as euler angles
	GK_INLINE gkEuler                   getRotation(void)           {return m_cur.rot;}
	// Gets cached local orientation
	GK_INLINE const gkQuaternion       &getOrientation(void)        {return m_cur.rot;}
	// Gets cached local position
	GK_INLINE const gkVector3          &getPosition(void)           {return m_cur.loc;}
	// Gets cached local scale
	GK_INLINE const gkVector3          &getScale(void)              {return m_cur.scl;}

	// world, with parent
	gkMatrix4       getWorldTransform(void);
	gkVector3       getWorldPosition(void);
	gkVector3       getWorldScale(void);
	gkQuaternion    getWorldOrientation(void);
	gkEuler         getWorldRotation(void);


	// set local transforms
	void setTransform(const gkMatrix4 &v);
	void setTransform(const gkTransformState &v);
	void setPosition(const gkVector3 &v);
	void setScale(const gkVector3 &v);

	void setOrientation(const gkQuaternion &q);
	void setOrientation(const gkEuler &v);

	void rotate(const gkEuler &drot, int tspace = TRANSFORM_PARENT);
	void rotate(const gkQuaternion &q, int tspace = TRANSFORM_PARENT);
	void translate(const gkVector3 &dloc, int tspace = TRANSFORM_PARENT);
	void scale(const gkVector3 &dscale);

	void yaw(const gkRadian &v, int  tspace = TRANSFORM_PARENT);
	void pitch(const gkRadian &v, int  tspace = TRANSFORM_PARENT);
	void roll(const gkRadian &v, int  tspace = TRANSFORM_PARENT);


	// rigid body methods

	void        applyTorque(const gkVector3 &t, int tspace = TRANSFORM_PARENT);
	void        applyForce(const gkVector3 &f, int tspace = TRANSFORM_PARENT);
	void        setLinearVelocity(const gkVector3 &linv, int tspace = TRANSFORM_PARENT);
	void        setAngularVelocity(const gkVector3 &angv, int tspace = TRANSFORM_PARENT);
	gkVector3   getLinearVelocity(void);
	gkVector3   getAngularVelocity(void);

	void attachLogic(gkLogicTree *tree);
	void attachLogic(gkLogicLink *bricks);

	// Ogre base class for movables
	Ogre::MovableObject *getMovable(void);

	virtual Ogre::AxisAlignedBox getAabb() const;

	struct NavMeshData
	{
		int triangleBaseIndex;
		int nIndex;
		float hmin;
		float hmax;

		NavMeshData() : triangleBaseIndex(-1), nIndex(0), hmin(0), hmax(0) {}
		NavMeshData(int i, int n, float hMin, float hMax) : triangleBaseIndex(i), nIndex(n), hmin(hMin), hmax(hMax) {}
		bool isEmpty() const { return !nIndex; }
	};

	GK_INLINE const NavMeshData &getNavData() const { return m_navMeshData; }

	GK_INLINE void setNavData(const NavMeshData &data) { m_navMeshData = data; }

	GK_INLINE void resetNavData() { m_navMeshData = NavMeshData(); }

	void GK_INLINE recalcNavData(int i, int n)
	{
		if(m_navMeshData.triangleBaseIndex > i)
		{
			m_navMeshData.triangleBaseIndex -= n;
		}
	}

	gkGameObject *getChildEntity();

	GK_INLINE void  setState(int v)     {m_state = v;}
	GK_INLINE int   getState(void)      {return m_state;}

protected:
	void cloneImpl(gkGameObject *clob);

	// Base class type
	gkGameObjectTypes           m_type;


	// Properties specific to the GameObject class
	gkGameObjectProperties      m_baseProps;

	// Parent object
	gkGameObject               *m_parent;

	GameObjects					m_children;

	// Parent scene
	gkScene                    *m_scene;

	// Ogre scenegraph node
	Ogre::SceneNode            *m_node;

	// Attached nodelogic trees
	gkLogicTree                *m_logic;

	// Attached logic bricks
	gkLogicLink                *m_bricks;


	// Current state of object transforms
	gkTransformState            m_prev, m_cur;

	// Physics body instance
	gkRigidBody                *m_rigidBody;
	gkCharacter                *m_character;

	VariableMap                 m_variables;
	ConstraintList              m_constraints;


	gkGameObjectGroup          *m_groupRef;     //is owner
	gkGameObjectInstance       *m_instance;     //is instance
	bool                        m_groupOwn;

	int                         m_state;
	bool                        m_activeLayer, m_outOfDate;
	bool                        m_isClone;

	// time to live 0 = forever
	LifeSpan                    m_life;


	virtual void loadImpl(void);
	virtual void unloadImpl(void);
	virtual void postLoadImpl(void);
	virtual void postUnloadImpl(void);

private:
	void loadPhysics(void);
	void destroyPhysics(void);

	NavMeshData m_navMeshData;
};

#endif//_gkGameObject_h_
