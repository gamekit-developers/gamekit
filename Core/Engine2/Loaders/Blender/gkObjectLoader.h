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
#ifndef _gkObjectLoader_h_
#define _gkObjectLoader_h_

#include "gkLoaderCommon.h"
#include "gkGameObject.h"


class btCollisionShape;
class btTriangleMesh;


class gkGameObjectLoader : public gkObject::Loader
{
public:
    gkGameObjectLoader(gkBlendFile *fp, Blender::Scene* sc, Blender::Object *ob);
    virtual ~gkGameObjectLoader();

    void load(gkObject *ob);

protected:
    void setLight(gkGameObject *ob);
    void setCamera(gkGameObject *ob);
    void setEntity(gkGameObject *ob);
    void setSkeleton(gkGameObject *ob);

    void setConstraints(gkGameObject *ob);
    void setProperties(gkGameObject *ob);


    gkBlendFile *m_file;
    Blender::Object*    m_object;
    Blender::Scene*     m_scene;
};


// Rigid body loader to support dynamic loading 
// and unloading at runtime of physics bodies
class gkRigidBodyLoader : public gkObject::Loader
{
public:

    typedef utArray<btCollisionShape *> ShapeArray;

public:

    gkRigidBodyLoader(  gkBlendFile *fp, 
                        Blender::Scene* sc, 
                        Blender::Object *ob,
                        gkGameObject *obj
                        );
    virtual ~gkRigidBodyLoader();

    void load(gkObject *ob);

protected:

    void freeResources(void);

    gkBlendFile*        m_file;
    Blender::Object*    m_object;
    Blender::Scene*     m_scene;
    gkGameObject*       m_gameObj;

    ShapeArray          m_shapes;
    btTriangleMesh*     m_triMesh;

};




#endif//_gkObjectLoader_h_
