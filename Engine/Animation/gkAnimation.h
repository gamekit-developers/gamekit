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

#ifndef GKANIMATION_H
#define GKANIMATION_H

#include "gkCommon.h"
#include "gkTransformState.h"
#include "gkResource.h"

#include "AnimKit.h"

//#include "akAnimationChannel.h"
//#include "akKeyedAnimation.h"
//#include "akAnimationSequence.h"
//#include "akAnimationPlayer.h"

typedef akAnimationBlender gkAnimationBlender;
//typedef akAnimation gkAnimation;

class gkAnimation : public gkResource
{
public:
	gkAnimation(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle) : gkResource(creator, name, handle) {}
	virtual ~gkAnimation() {}

	virtual akAnimation* getInternal(void) =0;

};


class gkKeyedAnimation : public gkAnimation
{
public:
	gkKeyedAnimation(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkKeyedAnimation() {}
	
	virtual akAnimation* getInternal(void) { return &m_animation;}
	
	UT_INLINE akScalar         getLength(void) const       { return m_animation.getLength(); }
	UT_INLINE void             setLength(akScalar v)       { m_animation.setLength(v); }
	
	UT_INLINE akKeyedAnimation::Channels::ConstPointer getChannels(void) const    { return m_animation.getChannels(); }
	UT_INLINE int                                      getNumChannels(void) const { return m_animation.getNumChannels(); }
	
	UT_INLINE void                addChannel(akAnimationChannel* chan) { m_animation.addChannel(chan); }
	UT_INLINE akAnimationChannel* getChannel(const utString& name)     { return m_animation.getChannel(name); }
	
private:
	akKeyedAnimation m_animation;
};


class gkAnimationSequence : public gkAnimation
{
public:
	gkAnimationSequence(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkAnimationSequence() {}
	
	virtual akAnimation* getInternal(void) { return &m_animation;}
	
	UT_INLINE akScalar         getLength(void) const       { return m_animation.getLength(); }
	UT_INLINE void             setLength(akScalar v)       { m_animation.setLength(v); }
	
	UT_INLINE void addItem(akAnimation* act, const akScalar& start, const akScalar& end, const akScalar& blendin = 0, const akScalar& blendout = 0, bool fitLength = true)
	 { m_animation.addItem(act, start, end, blendin, blendout, fitLength); }
	
	void addItem(const gkHashedString& animation, const akScalar& start, const akScalar& end, const akScalar& blendin = 0, const akScalar& blendout = 0);

private:
	akAnimationSequence m_animation;
};


class gkAnimationPlayer : public akAnimationPlayer
{
protected:
	gkGameObject*        m_object;
	
public:
	gkAnimationPlayer() : akAnimationPlayer(), m_object(0) {}
	gkAnimationPlayer(gkAnimation* resource, gkGameObject* object) : akAnimationPlayer(resource->getInternal()), m_object(object) {}
	~gkAnimationPlayer() {}
	
	GK_INLINE gkGameObject*    getObject(void) const       { return m_object; }
	GK_INLINE void             setObject(gkGameObject * v) { m_object = v; }
	
private:
	virtual void evaluateImpl(gkScalar time) { m_action->evaluate(time, m_weight, m_object); }
};


class gkTransformChannel : public akAnimationChannel
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
	gkTransformChannel(const utString& name, gkAnimation* parent);
	virtual ~gkTransformChannel() {}
		
	GK_INLINE bool                    isEulerRotation(void) const { return m_isEulerRotation; }
	GK_INLINE void                    setEulerRotation(bool v)    { m_isEulerRotation = v; }

protected:
	virtual void evaluateImpl(const gkScalar& time, const gkScalar& delta, const gkScalar& weight, void* object) const;
	
	virtual void applyTransform(void* object, const gkTransformState* transform, const gkScalar& weight) const = 0;
	
};



class gkObjectChannel : public gkTransformChannel
{
public:
	gkObjectChannel(const gkString& name, gkAnimation* parent) : gkTransformChannel(name, parent) {}
	virtual ~gkObjectChannel() {}

protected:
	virtual void applyTransform(void* object, const gkTransformState* transform, const gkScalar& weight) const;
};



class gkBoneChannel : public gkTransformChannel
{
public:
	gkBoneChannel(const gkString& name, gkAnimation* parent) : gkTransformChannel(name, parent) {}
	virtual ~gkBoneChannel() {}

protected:
	virtual void applyTransform(void* object, const gkTransformState* transform, const gkScalar& weight) const;
};



#endif // GKANIMATION_H
