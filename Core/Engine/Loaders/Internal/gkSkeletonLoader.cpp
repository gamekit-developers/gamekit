/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "OgreAnimation.h"
#include "OgreBone.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonManager.h"

#include "gkSkeletonLoader.h"
#include "gkObjectLoader.h"
#include "gkEngine.h"
#include "gkBlendFile.h"
#include "gkBlenderDefines.h"
#include "gkLoaderUtils.h"
#include "gkLogger.h"
#include "gkUserDefs.h"

#include "blender.h"
#include "bBlenderFile.h"
#include "bMain.h"

using namespace Ogre;





//-----------------------------------------------------------------------------
struct LocalBone
{
	LocalBone(const gkHashedString& bn, unsigned short hndl) :
			name(bn), bbone(0), bpbone(0), obone(0),
			handle(hndl),
			parnmat(Matrix4::IDENTITY),
			bonemat(Matrix4::IDENTITY),
			posemat(Matrix4::IDENTITY),
			poseinv(Matrix4::IDENTITY),
			parent(0),
			old_poseloc(0,0,0),
			old_poseskl(1,1,1),
			old_poserot(1, 0,0, 0)


	{
	}

	const gkHashedString	  name;
	Blender::Bone		   *bbone, *bpbone;
	Ogre::Bone			  *obone;
	const unsigned short	handle;
	Matrix4				 parnmat, bonemat, posemat, poseinv;
	LocalBone*			  parent;

	// cache
	Vector3	 old_poseloc;
	Vector3	 old_poseskl;
	Quaternion  old_poserot;
};


//-----------------------------------------------------------------------------
class gkSkeletonLoaderPrivate
{
public:
	gkSkeletonLoaderPrivate(gkSkeletonLoader *sl);
	~gkSkeletonLoaderPrivate();


	void convertSkeleton(Skeleton *skel);
	void buildBoneTree(Blender::Bone *root, Blender::Bone *prev, LocalBone *parent);
	void actionToAnimation(Blender::bPose *pose, Blender::bAction *action);

	void setPoseMatrices(Blender::bPose *pose, Blender::bAction *action,  const Real& otime, const Real& time);
	void buildPoseTreeKeyFrame(Animation *anim, Blender::Bone *cur, LocalBone *parent, const Real& time);

	Blender::bActionChannel *findActionChannel(const char *name, Blender::bAction *action);

	gkSkeletonLoader *loader;
	Skeleton* skeleton;
	Matrix4 transform;
	Blender::bArmature *armature;

	typedef gkHashTable<gkHashedString, LocalBone*> LocalBoneMap;
	typedef gkHashTableIterator<LocalBoneMap> LocalBoneIterator;

	LocalBoneMap bones;
};





//-----------------------------------------------------------------------------
gkSkeletonLoaderPrivate::gkSkeletonLoaderPrivate(gkSkeletonLoader *sl) :
		loader(sl), transform(), armature(0)
{
	GK_ASSERT(loader);
}

//-----------------------------------------------------------------------------
gkSkeletonLoaderPrivate::~gkSkeletonLoaderPrivate()
{
	/// free temp bones
	LocalBoneIterator it(bones);
	while (it.hasMoreElements())
	{
		LocalBoneIterator::Pair p= it.getNext();
		delete p.second;
	}
}


//-----------------------------------------------------------------------------
void gkSkeletonLoaderPrivate::buildBoneTree(Blender::Bone *root, Blender::Bone *prev, LocalBone *parent)
{
	/// This is the normal resposition
	GK_ASSERT(root);
	GK_ASSERT(skeleton);

	LocalBone* lbone= new LocalBone(root->name, bones.size());
	bones.insert(lbone->name, lbone);

	lbone->bbone= root;
	lbone->bpbone= prev;

	// matrix is relitave to parent bone
	Matrix4 parmat= Matrix4::IDENTITY;
	if (prev != 0 && parent != 0)
	{
		/// find and save parent
		size_t pos= bones.find(prev->name);
		GK_ASSERT(pos != GK_NPOS);

		lbone->parent= bones.at(pos);
		parmat= lbone->parent->parnmat;
	}

	Matrix4 bonemat= gkMathUtils::getFromFloat(root->arm_mat);

	/// apply relitave to local transform
	bonemat= transform * bonemat;
	// save it's inverse for next iteration
	lbone->parnmat= bonemat.inverse();

	// apply relitave to previous bone
	bonemat= parmat * bonemat;
	lbone->bonemat= bonemat;

	/// create the ogre bone
	lbone->obone= skeleton->createBone(lbone->name, lbone->handle);
	if (parent)
	{
		GK_ASSERT(parent->obone);
		parent->obone->addChild(lbone->obone);
	}

	/// save this transform
	lbone->obone->setPosition(bonemat.getTrans());
	Quaternion quat= bonemat.extractQuaternion();
	quat.normalise();
	lbone->obone->setOrientation(quat);
	lbone->obone->setScale(Vector3::UNIT_SCALE);
	lbone->obone->setInitialState();

	Blender::Bone *chi= static_cast<Blender::Bone*>(root->childbase.first);
	while (chi)
	{
		// recurse
		buildBoneTree(chi, root, lbone);
		chi= chi->next;
	}
}

//-----------------------------------------------------------------------------
Blender::bActionChannel *gkSkeletonLoaderPrivate::findActionChannel(const char *name, Blender::bAction *action)
{
	Blender::bActionChannel *achan= static_cast<Blender::bActionChannel*>(action->chanbase.first);
	while (achan)
	{
		if (*name == achan->name[0] && strcmp(name, achan->name)==0)
			return achan;
		achan= achan->next;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void gkSkeletonLoaderPrivate::buildPoseTreeKeyFrame(Animation *anim, Blender::Bone *cur, LocalBone *parent, const Real& time)
{
	GK_ASSERT(anim);
	GK_ASSERT(cur);

	size_t pos= bones.find(cur->name);
	if (pos == GK_NPOS)
	{
		gkPrintf("Warnning: missing bone for pose channel '%s'", cur->name);
		return;
	}

	LocalBone *bone= bones.at(pos);
	Matrix4 posemat= bone->posemat;

	posemat= transform * posemat;

	// store inverse for next iteration
	bone->poseinv= posemat.inverse();
	if (bone->parent != 0)
		posemat= parent->poseinv * posemat;


	/// build key frame transform
	Matrix4 invbone= bone->bonemat.inverse();

	Vector3 loc= posemat.getTrans();

	posemat= invbone * posemat;
	Quaternion rot= posemat.extractQuaternion();
	rot.normalise();

	Matrix4 invrot(rot.Inverse());
	posemat= invrot * posemat;
	Vector3 skl= Vector3(posemat[0][0], posemat[1][1], posemat[2][2]);


	/// create the ogre keyframe
	GK_ASSERT(bone->obone);
	NodeAnimationTrack *nat= NULL;
	if (anim->hasNodeTrack(bone->handle))
		nat= anim->getNodeTrack(bone->handle);
	else
		nat= anim->createNodeTrack(bone->handle, bone->obone);

	GK_ASSERT(nat);

	/// Optimise Node Tracks
	if (!loc.positionEquals(bone->old_poseloc, GK_EPSILON) ||
			!rot.equals(bone->old_poserot, Radian(GK_EPSILON)) ||
			!skl.positionEquals(bone->old_poseskl, GK_EPSILON))
	{
		/// add a new time key
		TransformKeyFrame *tkf= nat->createNodeKeyFrame(time);
		tkf->setTranslate(loc);
		tkf->setRotation(rot);
		tkf->setScale(skl);

		bone->old_poseloc= loc;
		bone->old_poserot= rot;
		bone->old_poseskl= skl;
	}

	Blender::Bone *chi= static_cast<Blender::Bone*>(cur->childbase.first);
	while (chi)
	{
		buildPoseTreeKeyFrame(anim, chi, bone, time);
		chi= chi->next;
	}
}



//-----------------------------------------------------------------------------
void gkSkeletonLoaderPrivate::setPoseMatrices(Blender::bPose *pose, Blender::bAction *action, const Real& otime, const Real& time)
{
	GK_ASSERT(pose);
	GK_ASSERT(action);

	Blender::bPoseChannel *pchan;
	Blender::bActionChannel *achan;
	Blender::Ipo *ipo;
	Blender::IpoCurve *icu;

	pchan= (Blender::bPoseChannel*)pose->chanbase.first;
	while (pchan)
	{
		if (!pchan->bone)
		{
			/// skip this channel
			pchan= pchan->next;
			continue;
		}

		LocalBone *bone= 0;
		size_t pos= bones.find(pchan->name);

		if (pos == GK_NPOS)
		{
			// something is wrong
			pchan= pchan->next;
			continue;
		}

		bone= bones.at(pos);

		/// We now need to calculate the pose & channel matrix
		Matrix4 chanmat= Matrix4::IDENTITY;

		achan= findActionChannel(pchan->name, action);
		if (achan && achan->ipo)
		{
			ipo= achan->ipo;
			Vector3 loc=Vector3::ZERO, skl= Vector3::UNIT_SCALE;
			Quaternion rot= Quaternion::IDENTITY;

			icu= static_cast<Blender::IpoCurve*>(ipo->curve.first);
			while (icu)
			{
				Real curval= 0.f;
				if (icu->bezt)
					curval= gkLoaderUtils::evaluateIpoCurve(icu, time);

				switch (icu->adrcode)
				{
				case AC_QUAT_W: { rot.w= curval;  break; }
				case AC_QUAT_X: { rot.x= curval;  break; }
				case AC_QUAT_Y: { rot.y= curval;  break; }
				case AC_QUAT_Z: { rot.z= curval;  break; }
				case AC_LOC_X:  { loc.x= curval;  break; }
				case AC_LOC_Y:  { loc.y= curval;  break; }
				case AC_LOC_Z:  { loc.z= curval;  break; }
				case AC_SIZE_X: { skl.x= curval;  break; }
				case AC_SIZE_Y: { skl.y= curval;  break; }
				case AC_SIZE_Z: { skl.z= curval;  break; }
				}
				icu= icu->next;
			}

			/// combine curve results
			gkMathUtils::makeLocQuatScale(chanmat, loc, rot, skl);
		}

		// update posemat for the new chanmat
		Blender::Bone *bbone= bone->bbone;
		Blender::Bone *bpbone= bone->bpbone;

		if (bone->parent)
		{
			Matrix4 offs= gkMathUtils::getFromFloat3(bbone->bone_mat);

			/// NOTE: leaving out bone->head && bpbone->length
			/// in offs (Ogre does not need it)
			// Vector3 len= Vector3((float*)&bbone->head.x);
			// len.y += bpbone->length;
			// offs.setTrans(len);

			// Todo ==> bbone->flag & BONE_HINGE;
			// Todo ==> bbone->flag & BONE_NO_SCALE

			bone->posemat= (bone->parent->posemat * offs) * chanmat;
		}
		else
		{
			/// use the armature matrix
			Matrix4 armamat= gkMathUtils::getFromFloat(bbone->arm_mat);
			bone->posemat= armamat * chanmat;
		}


		pchan= pchan->next;
	}
}

//-----------------------------------------------------------------------------
void gkSkeletonLoaderPrivate::actionToAnimation(Blender::bPose *pose, Blender::bAction *action)
{
	GK_ASSERT(pose);
	GK_ASSERT(action);

	Blender::bPoseChannel* pchan;
	Blender::bActionChannel *achan;
	Blender::Ipo *ipo;
	Blender::IpoCurve *icu;
	Blender::BezTriple *bezt;
	Blender::Bone *bone;

	/// find the total length
	Real trange[2]= {Real(FLT_MAX), -Real(FLT_MAX)};
	pchan= static_cast<Blender::bPoseChannel*>(pose->chanbase.first);
	while (pchan)
	{
		achan= findActionChannel(pchan->name, action);
		if (achan && achan->ipo)
		{
			ipo= achan->ipo;
			icu= (Blender::IpoCurve*)ipo->curve.first;

			while (icu)
			{
				if (icu->bezt)
				{
					bezt= icu->bezt;
					for (int c=0; c<icu->totvert; c++, bezt++)
					{
						if (trange[0] > bezt->vec[1][0])
							trange[0]= bezt->vec[1][0];
						if (trange[1] < bezt->vec[1][0])
							trange[1]= bezt->vec[1][0];
					}
				}
				icu= icu->next;
			}
		}
		pchan= pchan->next;
	}


	const Real lfps= gkEngine::getSingleton().getUserDefs().animspeed;
	const Real ifps= 1.f / lfps;
	const Real len= (trange[1] - trange[0]) * ifps;
	if (len <= 0)
	{
		gkPrintf("Warning: Skipped action %s it contains no data!", GKB_IDNAME(action));
		return;
	}

	gkPrintf("Creating animation '%s' (%i, %i) at %i frames per second",
			 GKB_IDNAME(action), int(trange[0]), int(trange[1]), int(lfps));

	Animation *anim= skeleton->createAnimation(GKB_IDNAME(action), len);

	int ctime= int(trange[0]);
	while (ctime < int(trange[1]))
	{
		const Real otime= (Real(ctime) - trange[0]) * ifps;

		/// update curve points for this frame
		/// This involves re-calculating the pose matrix!
		/// @todo add IK-tree support
		/// @todo add constraint channel support
		/// @todo ipo drivers, what are they for ?
		setPoseMatrices(pose, action, otime, Real(ctime));

		// Apply pose keyframes
		bone= static_cast<Blender::Bone*>(armature->bonebase.first);
		while (bone)
		{
			/// this is recursive from roots
			if (bone->parent == 0)
				buildPoseTreeKeyFrame(anim, bone, NULL, otime);
			bone= bone->next;
		}

		// next frame
		++ctime;
	}

	// TODO: fix key frame loop and make this obsolete
	anim->optimise();


}

//-----------------------------------------------------------------------------
void gkSkeletonLoaderPrivate::convertSkeleton(Skeleton *skel)
{
	GK_ASSERT(skel);
	GK_ASSERT(loader);
	GK_ASSERT(loader->mObject);
	GK_ASSERT(loader->mObject->data);


	armature= static_cast<Blender::bArmature *>(loader->mObject->data);
	skeleton= skel;

	Matrix4 armainv, armmat;
	armainv= armmat= gkMathUtils::getFromFloat(loader->mObject->obmat);

	// load root transform
	transform= armainv.inverse() * armmat;

	// create bone lists && transforms
	Blender::Bone *bone= static_cast<Blender::Bone*>(armature->bonebase.first);
	while (bone)
	{
		// only process root bones
		if (bone->parent == 0)
			buildBoneTree(bone, NULL, NULL);
		bone= bone->next;
	}

	// shortcut (no animation data assigned)
	if (!loader->mObject->pose)
		return;

	// create animations
	GK_ASSERT(loader->mFile);
	bParse::bMain *mp= loader->mFile->getInternalFile()->getMain();

	// Loop all actions and find ones that contain this skeleton's bone names
	// FIXME: It would really be nice to convert only wanted animations. but you
	// cannot tell beforehand if 1 of 100 are needed, or all 100!

	bParse::bListBasePtr* actions= mp->getAction();
	if (actions)
	{
		size_t i= 0;
		for (i=0; i<actions->size(); i++)
		{
			Blender::bAction *act= reinterpret_cast<Blender::bAction*>(actions->at(i));
			Blender::bActionChannel *ac= static_cast<Blender::bActionChannel*>(act->chanbase.first);
			if (ac)
			{
				/// find if the channel belongs to this armature (whats the proper way ?)
				if (bones.find(ac->name) != GK_NPOS)
				{
					// handle this action
					actionToAnimation(loader->mObject->pose, act);
				}
			}
		}
	}

}

//-----------------------------------------------------------------------------
gkSkeletonLoader::gkSkeletonLoader(gkBlendFile *fp, Blender::Object *arma) :
		mFile(fp),
		mObject(arma)
{
	GK_ASSERT(mFile);
}

//-----------------------------------------------------------------------------
gkSkeletonLoader::~gkSkeletonLoader()
{
}

//-----------------------------------------------------------------------------
void gkSkeletonLoader::loadResource(Resource* resource)
{
	GK_ASSERT(mFile);
	GK_ASSERT(mObject);
	GK_ASSERT(resource);

	Animation::setDefaultInterpolationMode(Animation::IM_LINEAR);
	Animation::setDefaultRotationInterpolationMode(Animation::RIM_LINEAR);


	gkSkeletonLoaderPrivate converter(this);
	converter.convertSkeleton(static_cast<Skeleton*>(resource));
}
