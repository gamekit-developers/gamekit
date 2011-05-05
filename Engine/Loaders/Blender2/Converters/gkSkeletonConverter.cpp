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
#include "gkSkeletonConverter.h"
#include "gkSkeletonResource.h"
#include "Blender.h"


void gkSkeletonConverter::buildBoneTree(Blender::Bone* cur, Blender::Bone* prev, gkBone* parent)
{
	// This is the normal resposition
	GK_ASSERT(cur);
	GK_ASSERT(m_skeleton);

	gkMatrix4 parBind = gkMatrix4::IDENTITY;
	if (prev != 0 && parent != 0)
		parBind = gkMathUtils::getFromFloatNorm(prev->arm_mat).inverse();

	// create the ogre bone
	gkBone* bone = m_skeleton->createBone(cur->name);
	if (parent)
		bone->setParent(parent);


	gkMatrix4 bind = parBind * gkMathUtils::getFromFloatNorm(cur->arm_mat);

	gkQuaternion rot; gkVector3 loc, scl;
	gkMathUtils::extractTransformFast(bind, loc, rot, scl);

	if (rot.isNaN())
	{
		rot = gkQuaternion();
		scl = gkVector3(1, 1, 1);
	}

	bone->setRestPosition(gkTransformState(loc, rot, scl));


	Blender::Bone* chi = static_cast<Blender::Bone*>(cur->childbase.first);
	while (chi)
	{
		// recurse
		buildBoneTree(chi, cur, bone);
		chi = chi->next;
	}
}
