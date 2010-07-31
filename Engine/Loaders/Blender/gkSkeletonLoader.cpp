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
#include "OgreBone.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonManager.h"

#include "gkSkeletonLoader.h"
#include "gkBlenderDefines.h"
#include "gkMathUtils.h"

#include "blender.h"
#include "bBlenderFile.h"
#include "bMain.h"


class gkSkeletonLoaderPrivate
{
public:
	gkSkeletonLoaderPrivate(gkSkeletonLoader *sl);
	~gkSkeletonLoaderPrivate();


	void convertSkeleton(Ogre::Skeleton *skel);
	void buildBoneTree(Blender::Bone *cur, Blender::Bone *prev, Ogre::Bone *parent);

	gkSkeletonLoader *loader;
	Ogre::Skeleton *skeleton;
	Blender::bArmature *armature;
};


gkSkeletonLoaderPrivate::gkSkeletonLoaderPrivate(gkSkeletonLoader *sl) :
	loader(sl), armature(0)
{
	GK_ASSERT(loader);
}


gkSkeletonLoaderPrivate::~gkSkeletonLoaderPrivate()
{
}



void gkSkeletonLoaderPrivate::buildBoneTree(Blender::Bone *cur, Blender::Bone *prev, Ogre::Bone *parent)
{
	// This is the normal resposition
	GK_ASSERT(cur);
	GK_ASSERT(skeleton);

	gkMatrix4 parBind = gkMatrix4::IDENTITY;
	if (prev != 0 && parent != 0)
		parBind = gkMathUtils::getFromFloatNorm(prev->arm_mat).inverse();

	// create the ogre bone
	Ogre::Bone *bone = skeleton->createBone(cur->name);
	if (parent)
		parent->addChild(bone);

	gkMatrix4 bind = parBind * gkMathUtils::getFromFloatNorm(cur->arm_mat);

	gkQuaternion rot; gkVector3 loc, scl;
	gkMathUtils::extractTransformFast(bind, loc, rot, scl);

	bone->setPosition(loc);
	bone->setOrientation(rot);
	bone->setScale(scl);
	bone->setBindingPose();

	Blender::Bone *chi = static_cast<Blender::Bone *>(cur->childbase.first);
	while (chi)
	{
		// recurse
		buildBoneTree(chi, cur, bone);
		chi = chi->next;
	}
}


void gkSkeletonLoaderPrivate::convertSkeleton(Ogre::Skeleton *skel)
{
	GK_ASSERT(skel);
	GK_ASSERT(loader);
	GK_ASSERT(loader->m_object);
	GK_ASSERT(loader->m_object->data);

	armature = static_cast<Blender::bArmature *>(loader->m_object->data);
	skeleton = skel;

	// create bone lists && transforms
	Blender::Bone *bone = static_cast<Blender::Bone *>(armature->bonebase.first);
	while (bone)
	{
		// only process root bones
		if (bone->parent == 0)
			buildBoneTree(bone, NULL, NULL);
		bone = bone->next;
	}
}


gkSkeletonLoader::gkSkeletonLoader(gkBlendFile *fp, Blender::Object *arma) :
	m_file(fp),
	m_object(arma)
{
	GK_ASSERT(m_file);
}


gkSkeletonLoader::~gkSkeletonLoader()
{
}


void gkSkeletonLoader::loadResource(Ogre::Resource *resource)
{
	GK_ASSERT(m_file);
	GK_ASSERT(m_object);
	GK_ASSERT(resource);

	gkSkeletonLoaderPrivate converter(this);
	converter.convertSkeleton(static_cast<Ogre::Skeleton *>(resource));
}
