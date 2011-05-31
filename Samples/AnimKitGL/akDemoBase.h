/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef AKDEMOBASE_H
#define AKDEMOBASE_H

#include "utTypes.h"
#include "utString.h"

#include "akCommon.h"
#include "akTransformState.h"

class akEntity;

typedef struct akCamera
{
	akTransformState m_transform;
	akScalar m_fov;
	akScalar m_clipStart, m_clipEnd;
}akCamera;

class akDemoBase
{
public:
	typedef utHashTable<utIntHashKey, akAnimationClip*> AnimationClips;
	typedef utHashTable<utIntHashKey, akSkeleton*> Skeletons;
	typedef utHashTable<utIntHashKey, akMesh*> Meshes;
	typedef utHashTable<utIntHashKey, akEntity*> Entities;
	
public:
	akDemoBase();
	virtual ~akDemoBase();
	
	void drawString(float x, float y, const char *s);
	
	void displayCallback(void);
	void reshapeCallback(int w, int h);
	void idleCallback(void);
	void keyboardCallback(unsigned char key,int x,int y);
	
	void render(void);
	
	int getFps(void);
	
	void             addAnimation(const utHashedString& name, akAnimationClip* anim);
	akAnimationClip* getAnimation(const utHashedString& name);
	
	AnimationClips::ConstPointer getAnimations(void);
	UTsize                       getNumAnimations(void);
	
	void        addMesh(const utHashedString& name, akMesh* mesh);
	akMesh*     getMesh(const utHashedString& name);
	
	Meshes::ConstPointer         getMeshes(void);
	UTsize                       getNumMeshes(void);
	
	void        addSkeleton(const utHashedString& name, akSkeleton* skel);
	akSkeleton* getSkeleton(const utHashedString& name);
	
	Skeletons::ConstPointer      getSkeletons(void);
	UTsize                       getNumSkeletonss(void);
	
	void        addEntity(const utHashedString& name, akEntity* ent);
	akEntity*   getEntity(const utHashedString& name);
	
	Entities::ConstPointer       getEntities(void) const    { return m_objects.ptr(); }
	UTsize                       getNumEntities(void) const { return m_objects.size(); }
	
	UT_INLINE akCamera* getCamera(void)
	{
                return m_camera;
	}
	
	void start(void);
	void step(akScalar seconds);
	virtual void init(void) =0;
	
protected:
	int m_windowx, m_windowy;
	unsigned int m_frame, m_time, m_fpsLastTime, m_stepLastTime, m_lastfps;
	bool m_canUseVbo;
	bool m_useVbo;
	bool m_drawNormals;
	bool m_wireframe;
	bool m_textured;
	bool m_shaded;
	bool m_drawColor;
	int m_dualQuatUse;
	int m_normalMethod;
	
	// Ressources
	AnimationClips m_animations;
	Skeletons      m_skeletons;
	Meshes         m_meshes;
	Entities       m_objects;
	
	akCamera* m_camera;
	
};

#endif // AKDEMOBASE_H
