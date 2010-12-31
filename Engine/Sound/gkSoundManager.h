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
#ifndef _gkSoundManager_h_
#define _gkSoundManager_h_


#include "gkCommon.h"
#include "gkResourceManager.h"
#include "gkStreamer.h"
#include "gkHashedString.h"
#include "gkSerialize.h"
#include "utSingleton.h"
#include "gkTransformState.h"


class gkSound;
class gkSource;
class gkBuffer;
class gkCamera;



// Sound system manager
class gkSoundManager : public gkResourceManager, public utSingleton<gkSoundManager>
{
public:
	typedef utArray<gkSource*> Sources;

private:
	ALCdevice*          m_device;			// OpenAL Device
	ALCcontext*         m_context;			// OpenAL Context
	gkStreamer*         m_stream;			// Playback stream
	Sources             m_playingSources;	// list of currently active sources
	Sources             m_gcSources;		// sources to remove when done playing
	bool                m_valid;
	bool				m_disabled;


	void initialize(void);
	gkSoundSceneProperties m_props;			// conversion properties
	void removePlayback(gkSound* sndToDelete);

public:

	gkSoundManager();
	virtual ~gkSoundManager();

	gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle);


	void stopAllSounds(void);

	bool hasSounds(void);
	void playSound(gkSource*);
	void stopSound(gkSource*);

	void update(gkScene* scene);
	void collectGarbage(void);


	void notifySourceCreated(gkSource*);
	void notifySourceDestroyed(gkSource*);

	bool isValidContext(void);

	gkSoundSceneProperties& getProperties(void) {return m_props;}
	void updateSoundProperties(void);

	static ALCcontext* getCurrentContext(void)
	{
		return getSingleton().m_context;
	}

protected:
	virtual void notifyDestroyAllImpl(void);

	virtual void notifyResourceCreatedImpl(gkResource* res);
	virtual void notifyResourceDestroyedImpl(gkResource* res);

	UT_DECLARE_SINGLETON(gkSoundManager);
};

#endif//_gkSoundManager_h_
