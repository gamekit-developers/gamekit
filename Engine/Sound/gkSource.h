/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira & Charlie C.

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
#ifndef _gkSource_h_
#define _gkSource_h_


#include "Thread/gkCriticalSection.h"
#include "gkCommon.h"
#include "gkSoundUtil.h"
#include "gkMathUtils.h"

class gkBuffer;
class gkSound;
class gkGameObject;
class gkSoundStream;

class gkSoundProperties
{
	// Properties that can be added to a playback buffer

public:
	gkSoundProperties()
		:   m_volume(1.f),
		    m_pitch(0.f),
		    m_loop(false),
		    m_3dSound(false),
		    m_refDistance(1.f),
		    m_maxDistance(GK_INFINITY),
		    m_gainClamp(0.f, 1.f),
		    m_rolloff(1.f),
		    m_coneOuterGain(0.f),
		    m_coneAngle(360.f, 360.f),
		    m_position(0.f, 0.f, 0.f),
		    m_velocity(0.f, 0.f, 0.f),
		    m_direction(0.f, 0.f, 0.f),
		    m_height(1.f)
	{
	}

	gkScalar    m_volume;
	gkScalar    m_pitch;
	bool        m_loop;


	bool        m_3dSound;

	gkScalar    m_refDistance;
	gkScalar    m_maxDistance;
	gkVector2   m_gainClamp;
	gkScalar    m_rolloff;
	gkScalar    m_coneOuterGain;
	gkVector2   m_coneAngle;

	// position params
	gkVector3       m_position;
	gkVector3       m_velocity;
	gkVector3       m_direction;
	gkQuaternion    m_orientation;
	gkScalar        m_height;

};


class gkSource
{
private:
	gkBuffer*            m_playback;    // active playback buffer
	gkSoundProperties    m_props;       // properties to attach to the sound stream
	gkSound*             m_reference;   // reference sound object

	// Force internal usage.

	friend class gkBuffer;
	friend class gkStreamer;

	// Bind / unbind a buffer to this object.
	// This object is only playable when a buffer is attached.
	void bind(gkBuffer* buf);

	// Playback buffer.
	gkBuffer* getBuffer(void) {return m_playback;}

	// Playback stream.
	gkSoundStream* getStream(void);



	mutable gkCriticalSection m_cs;
public:

	gkSource(gkSound* sound);
	virtual ~gkSource();


	GK_INLINE bool                  isStopped(void)   const  {return !isPlaying() || isPaused(); }
	GK_INLINE bool                  isPlaying(void)   const  {return isBound();}
	GK_INLINE bool                  isBound(void)     const  {return m_playback != 0;}
	GK_INLINE bool                  isLooped(void)    const  {return m_props.m_loop;}
	GK_INLINE gkSoundProperties&    getProperties(void)      {return m_props;}
	GK_INLINE gkSound*              getCreator(void)         {return m_reference;}

	GK_INLINE void setProperties(gkSoundProperties& props)  {m_props = props;}

	// Object state for 3D sounds
	void updatePropsForObject(gkGameObject* obj);


	bool isPaused(void) const;

	// playing states
	void play(void);
	void pause(void);
	void stop(void);
	void loop(bool v);

	void resetPlayable(void);
};


#endif//_gkSource_h_
