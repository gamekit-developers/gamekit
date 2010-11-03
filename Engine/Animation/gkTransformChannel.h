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

#ifndef GKTRANSFORMCHANNEL_H
#define GKTRANSFORMCHANNEL_H

#include "Animation/gkAnimationChannel.h"
#include "gkCommon.h"

#include "gkGameObject.h"
#include "gkEntity.h"
#include "gkSkeleton.h"
#include "gkBone.h"

class gkTransformChannel : public gkAnimationChannel
{
public:
	enum gkTransformChannelCodes
	{
		SC_LOC_X,
		SC_LOC_Y,
		SC_LOC_Z,
		SC_SCL_X,
		SC_SCL_Y,
		SC_SCL_Z,
		SC_ROT_QUAT_X,
		SC_ROT_QUAT_Y,
		SC_ROT_QUAT_Z,
		SC_ROT_QUAT_W,
		SC_ROT_EULER_X,
		SC_ROT_EULER_Y,
		SC_ROT_EULER_Z,
	};
	
protected:
	bool                 m_isEulerRotation;
	
public:
	gkTransformChannel(const gkString& name, gkAction* parent);
	virtual ~gkTransformChannel() {}
		
	GK_INLINE bool                    isEulerRotation(void) const { return m_isEulerRotation; }
	GK_INLINE void                    setEulerRotation(bool v)    { m_isEulerRotation = v; }

protected:
	virtual void evaluateImpl(const gkScalar& time, const gkScalar& delta, const gkScalar& weight, gkGameObject* object) const;
	
	virtual void applyTransform(gkGameObject* object, const gkTransformState* transform, const gkScalar& weight) const = 0;
	
};


class gkObjectChannel : public gkTransformChannel
{
public:
	gkObjectChannel(const gkString& name, gkAction* parent) : gkTransformChannel(name, parent) {}
	virtual ~gkObjectChannel() {}

protected:
	virtual void applyTransform(gkGameObject* object, const gkTransformState* transform, const gkScalar& weight) const
	{
		if(!object)
			return;
	
		object->applyTransformState(*transform, weight);
	}
};


class gkBoneChannel : public gkTransformChannel
{
public:
	gkBoneChannel(const gkString& name, gkAction* parent) : gkTransformChannel(name, parent) {}
	virtual ~gkBoneChannel() {}

protected:
	virtual void applyTransform(gkGameObject* object, const gkTransformState* transform, const gkScalar& weight) const
	{
		gkSkeleton* skel = 0;
		
		if(!object)
			return;

		switch (object->getType())
		{
		case GK_ENTITY: skel = object->getEntity()->getSkeleton(); break;
		case GK_SKELETON: skel = object->getSkeleton(); break;
		}
		
		if(skel)
		{
			gkBone* bone = skel->getBone(m_name);
			if(bone)
				bone->applyChannelTransform(*transform, weight);
		}
	}
};

#endif // GKTRANSFORMCHANNEL_H
