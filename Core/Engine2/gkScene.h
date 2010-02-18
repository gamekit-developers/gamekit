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
#ifndef _gkSceneObject_h_
#define _gkSceneObject_h_

#include "gkObject.h"
#include "gkMathUtils.h"
#include "gkSerialize.h"

class gkDynamicsWorld;

// shorthand types
typedef utList<gkGameObject*>                       gkGameObjectList;
typedef utHashTable<gkHashedString, gkGameObject*>  gkGameObjectHashMap;
typedef utHashTableIterator<gkGameObjectHashMap>    gkGameObjectHashMapIterator;
typedef utArray<gkGameObject*>                      gkGameObjectArray;
typedef utArrayIterator<gkGameObjectArray>          gkGameObjectArrayIterator;


// Game scene
class gkScene : public gkObject
{
public:

    class Listener
    {
    public:
        virtual ~Listener() {}

        virtual void objectCollided(gkGameObject *A, gkGameObject *B) {};

        // Called when an object is moved / transformed
        virtual void objectUpdated(gkGameObject *ob) {};

        // Called when an object is added to the scene
        virtual void objectAdded(gkGameObject *ob) {};

        // Called when an object is removed from the scene
        // (before it's deleted)
        virtual void objectRemoved(gkGameObject *ob) {};

        // Called when the scene is updated
        virtual void update(gkScalar tick, gkScalar rate) {}
    };

public:

    gkScene(const gkString& name, gkObject::Loader* loader = 0);
    virtual ~gkScene();

    // Updates the game state
    void update(gkScalar tickRate);

    // Interpolates transformed objects,
    // for smoothing motion between game ticks
    void synchronizeMotion(gkScalar blend);

    // property access
    GK_INLINE gkSceneProperties &getProperties(void)    { return m_baseProps; }


    // The main viewport. Only support for one at the moment.
    GK_INLINE Ogre::Viewport* getViewport(void) { return m_viewport; }
    void applyViewport(Ogre::Viewport *vp);

    // The Ogre scene manager is only available
    // when this scene is loaded. Each scene has
    // it's own manager
    GK_INLINE Ogre::SceneManager* getManager(void)
    { GK_ASSERT(m_manager); return m_manager; }


    // The active camera
    GK_INLINE gkCamera* getMainCamera(void) { return m_startCam; }
    void setMainCamera(gkCamera *cam);

    GK_INLINE gkGameObjectList &getLoadedObjects(void)      { return m_loadedObjects; }

    // physics
    GK_INLINE gkDynamicsWorld* getDynamicsWorld(void)       { return m_physicsWorld; }
    gkDynamicsWorld* createWorld(gkObject::Loader *manual = 0);


    // notifications
    GK_INLINE void notifyObjectMoved(gkGameObject *gobject) { m_transformObjects.push_back(gobject); }
    void notifyObjectLoaded(gkGameObject *gobject) {}
    void notifyObjectUnloaded(gkGameObject *gobject) {}
    void notifyObjectUpdate(gkGameObject *gobject);


    bool            hasObject(const gkHashedString& ob);
    bool            hasObject(gkGameObject *ob);
    gkGameObject*   getObject(const gkHashedString& name);

    // Translates to a blank scene node or empty object.
    // loader is for creating custom loading schemes based
    // on callbacks to gather all object information.
    gkGameObject* createObject(const gkHashedString &name, gkObject::Loader *loader = 0);

    // Creates a game light
    // loader is for creating custom loading schemes based
    // on callbacks to gather all object information.
    gkLight* createLight(const gkHashedString &name, gkObject::Loader *loader = 0);


    // Creates a game camera
    // loader is for creating custom loading schemes based
    // on callbacks to gather all object information.
    gkCamera* createCamera(const gkHashedString &name, gkObject::Loader *loader = 0);


    // Creates a game entity
    // loader is for creating custom loading schemes based
    // on callbacks to gather all object information.
    gkEntity* createEntity(const gkHashedString &name, gkObject::Loader *loader = 0);

    // Creates a game skeleton
    // loader is for creating custom loading schemes based
    // on callbacks to gather all object information.
    gkSkeleton* createSkeleton(const gkHashedString &name, gkObject::Loader *loader = 0);


protected:


    // Ogre scene manager
    Ogre::SceneManager*     m_manager;

    // Main camera
    gkCamera*               m_startCam;

    // Main viewport 
    Ogre::Viewport*         m_viewport;

	// Properties for this scene
	gkSceneProperties       m_baseProps;


	gkGameObjectHashMap     m_objects;
    gkGameObjectArray       m_transformObjects;
    gkGameObjectList        m_loadedObjects;
    gkDynamicsWorld*        m_physicsWorld;

    virtual void loadImpl();
    virtual void unloadImpl();
};

#endif//_gkSceneObject_h_
