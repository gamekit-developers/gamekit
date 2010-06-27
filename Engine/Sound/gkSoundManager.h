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



#include "gkStreamer.h"
#include "gkCommon.h"
#include "gkHashedString.h"
#include "OgreSingleton.h"
#include "gkTransformState.h"


class gkSound;
class gkSource;
class gkBuffer;
class gkCamera;



// Sound system manager
class gkSoundManager : public Ogre::Singleton<gkSoundManager>
{
public:
    typedef utHashTable<gkHashedString, gkSound *> ObjectMap;
    typedef utArray<gkSource *> Sources;


private:
    ObjectMap           m_objects;      // all loaded sounds
    ALCdevice           *m_device;      // OpenAL Device
    ALCcontext          *m_context;     // OpenAL Context
    gkStreamer          *m_stream;      // Playback stream
    Sources             m_sources;      // list of currently active sources
    Sources             m_gcSources;    // sources to remove when done playing
    bool                m_valid;


    void removePlayback(gkSound *sndToDelete);

public:

    gkSoundManager();
    virtual ~gkSoundManager();


    void stopAllSounds(void);

    // has playing sounds
    bool        hasSounds(void);
    void        playSound(gkSource *);
    void        stopSound(gkSource *);

    void        update(gkScene *scene);
    void        collectGarbage(void);


    void        notifySourceCreated(gkSource *);
    void        notifySourceDestroyed(gkSource *);


    // sound access

    gkSound *getSound(const gkHashedString &name);
    gkSound *createSound(const gkHashedString &name);

    void destroy(const gkHashedString &name);
    void destroy(gkSound *ob);
    void destroyAll(void);
    bool hasSound(const gkHashedString &name);


    static gkSoundManager &getSingleton(void);
    static gkSoundManager *getSingletonPtr(void);


    static ALCcontext *getCurrentContext(void)
    {
        return getSingleton().m_context;
    }


};

#endif//_gkSoundManager_h_
