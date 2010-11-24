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
#ifndef _gkEngine_h_
#define _gkEngine_h_

#include "gkCommon.h"
#include "gkMathUtils.h"

#include "OgreSingleton.h"


class gkEngine : public Ogre::Singleton<gkEngine>
{
public:


	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void tick(gkScalar rate) = 0;
	};

	typedef utArray<Listener*> Listeners;

public:
	gkEngine(gkUserDefs* otherDefs = 0);
	~gkEngine();

	static gkEngine& getSingleton();
	static gkEngine* getSingletonPtr();


	void initialize();
	void finalize(void);
	void run(void);

	bool initializeStepLoop(void);
	bool stepOneFrame(void);
	void finalizeStepLoop(void);

	bool isInitialized(void)  {return m_initialized;}
	bool isRunning(void)      {return m_running;}

	void initializeWindow(void);


	bool hasActiveScene(void);
	void setActiveScene(gkScene* sc);

	gkUserDefs& getUserDefs(void);
	void requestExit(void);

	gkBlendFile* loadBlendFile(const gkString& blend, int options = 0, const gkString& inResource = GK_DEF_GROUP);
	void loadResources(const gkString& name);

	void addDebugProperty(gkVariable* prop);
	void removeDebugProperty(gkVariable* prop);

	static gkScalar getStepRate();
	static gkScalar getTickRate();

	gkScene* getActiveScene(void);

	void addListener(Listener* listener);
	void removeListener(Listener* listener);

private:

	class Private;
	Private*                m_private;
	friend class Private;


	Ogre::Root*             m_root;
	Ogre::RenderWindow*     m_window;
	bool                    m_initialized;
	bool                    m_ownsDefs;
	bool                    m_running;
	gkUserDefs*             m_defs;
	Listeners               m_listeners;

	static gkScalar         m_tickRate;
};



#endif//_gkEngine_h_
