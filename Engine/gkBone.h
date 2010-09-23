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
#ifndef _gkBone_h_
#define _gkBone_h_

#include "gkSerialize.h"


class gkBone
{
public:
	typedef utArray<gkBone*> BoneList;

public:

	gkBone(const gkString& name);
	~gkBone();

	void setParent(gkBone* bone);

	void setRestPosition(const gkTransformState& st);

	void applyChannelTransform(const gkTransformState& channel, gkScalar weight);
	void applyPoseTransform(const gkTransformState& pose);
	void applyRootTransform(const gkTransformState& root);

	const gkTransformState&  getRest(void)      {return m_bind;}


	gkTransformState&        getPose(void)      {return m_pose;}

	gkBone*                  getParent(void)    {return m_parent;}
	BoneList&                getChildren(void)  {return m_children;}


	const gkString&          getName(void)      {return m_name;}


	// Internal use
	UTsize                  _getBoneIndex(void);
	void                    _setOgreBone(Ogre::Bone* bone);

private:

	const gkString m_name;

	Ogre::Bone* m_bone;

	gkBone*      m_parent;
	BoneList    m_children;

	gkTransformState m_bind;

	// The current pose matrix, calculated in applyChannelTransform
	gkTransformState m_pose;
};


#endif//_gkBone_h_
