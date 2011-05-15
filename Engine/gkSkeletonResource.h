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
#ifndef _gkSkeletonResource_h_
#define _gkSkeletonResource_h_


#include "gkSerialize.h"
#include "gkResource.h"
#include "gkBone.h"


class gkSkeletonResource : public gkResource
{
public:
	typedef utHashTable<gkHashedString, gkBone*>    Bones;

public:
	gkSkeletonResource(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkSkeletonResource();

	gkBone*         createBone(const gkString& name);
	gkBone*         getBone(const gkHashedString& name);

	gkBone::BoneList& getRootBoneList(void);
	gkBone::BoneList& getBoneList(void) {return m_boneList;}


	GK_INLINE bool hasBone(const gkHashedString& name)        { return m_bones.find(name) != GK_NPOS; }

	gkSkeletonLoader* getExternalLoader(void) {return m_externalLoader;}
	void makeManual(gkEntity* ent);

	gkSkeletonResource* clone();

private:
	Bones               m_bones;
	gkBone::BoneList    m_boneList, m_rootBoneList;

	gkSkeletonLoader*   m_externalLoader;

	void copyBones(gkSkeletonResource& other);
};


#endif//_gkSkeletonResource_h_
