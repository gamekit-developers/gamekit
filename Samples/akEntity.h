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

#ifndef AKENTITY_H
#define AKENTITY_H

#include "akDemoBase.h"

#include "akCommon.h"
#include "akTransformState.h"
#include "akMesh.h"


#include "btAlignedAllocator.h"
#include "btAlignedObjectArray.h"


class akEntity
{
private:
	utHashedString m_name;
	// object's world transform
	akTransformState m_transform;
	
	// pointers to shared data (resources)
	akMesh*     m_mesh;
	akSkeleton* m_skeleton;
	akAnimatedObject* m_animatedObject;
	
	btAlignedObjectArray<akMatrix4>  m_matrixPalette;
	btAlignedObjectArray<akDualQuat> m_dualquatPalette;
	bool                             m_useDualQuatSkinning;
	bool                             m_posAnimated, m_morphAnimated;
	
	bool m_useVbo;
	
	utArray<GLuint> m_posnoVertexVboIds, m_staticVertexVboIds, m_staticIndexVboIds;
	utArray<GLuint> m_textures;

public:
	akEntity(const utHashedString& name);
	~akEntity();
	
	void init(bool useVbo, akDemoBase* demo);
	void update(int dualQuat, int normalsMethod);
	void draw(bool drawNormal, bool drawColor, bool textured, bool useVbo, bool shaded, bool drawskel);
	void updateVBO(void);
	
	UT_INLINE const utHashedString& getName(void) const
	{
		return m_name;
	}
	
	UT_INLINE akAnimatedObject* getAnimatedObject(void)
	{
		return m_animatedObject;
	}
	
	UT_INLINE void setAnimatedObject(akAnimatedObject* obj)
	{
		m_animatedObject = obj;
	}
	
	UT_INLINE akMesh* getMesh(void)
	{
		return m_mesh;
	}
	
	UT_INLINE void setMesh(akMesh* mesh)
	{
		m_mesh = mesh;
	}
	
	UT_INLINE akTransformState getTransformState(void)
	{
		return m_transform;
	}
	
	UT_INLINE void setTransformState(const akTransformState& v)
	{
		m_transform = v;
	}
	
	UT_INLINE void setSkeleton(akSkeleton* skel)
	{
		m_skeleton = skel;
	}
	
	UT_INLINE akSkeleton* getSkeleton(void)
	{
		return m_skeleton;
	}
	
	UT_INLINE bool isPositionAnimated(void)
	{
		return m_posAnimated;
	}
	
	UT_INLINE void setPositionAnimated(bool v)
	{
		m_posAnimated = v;
	}
	
	UT_INLINE bool isMeshDeformedBySkeleton(void)
	{
		return m_skeleton? true:false;
	}
	
	UT_INLINE bool isMeshDeformedByMorphing(void)
	{
		return m_mesh->hasMorphTargets();
	}
	
	UT_INLINE bool isMeshDeformed(void)
	{
		return isMeshDeformedBySkeleton() || isMeshDeformedByMorphing();
	}
	
	UT_INLINE bool getUseDualQuatSkinning(void)
	{
		return m_useDualQuatSkinning;
	}
	
	UT_INLINE void setUseDualQuatSkinning(const bool v)
	{
		m_useDualQuatSkinning = v;
	}
	
	UT_INLINE btAlignedObjectArray<akMatrix4>& getMatrixPalette(void)
	{
		return m_matrixPalette;
	}

	UT_INLINE btAlignedObjectArray<akDualQuat>& getDualQuatPalette(void)
	{
		return m_dualquatPalette;
	}
	
	BT_DECLARE_ALIGNED_ALLOCATOR();
};

#endif // AKENTITY_H
