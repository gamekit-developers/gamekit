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
#ifndef _gkEngine_h_
#define _gkEngine_h_

#include "gkCommon.h"
#include "OgreSingleton.h"



// ---------------------------------------------------------------------------
class gkEnginePrivate;

// ---------------------------------------------------------------------------
class gkEngine : public Ogre::Singleton<gkEngine>
{
public:
	gkEngine(const Ogre::String& homeDir= "");
	~gkEngine();

	static gkEngine& getSingleton();
	static gkEngine* getSingletonPtr();

	void initialize(bool autoCreateWindow = true);
	void finalize(void);
	bool isInitialized(void) {return mInitialized;}
	void run(void);

	void initializeWindow(const Ogre::String& windowName, int w, int h, bool fullscreen = false);


	void setActiveScene(gkSceneObject *sc);

	// access to user defined parameters
	gkUserDefs& getUserDefs(void);
	void requestExit(void);

	gkBlendFile* loadBlendFile(const Ogre::String& blend, const Ogre::String& inResource=GK_DEF_GROUP);
	void loadResources(const Ogre::String &name);

	void addDebugProperty(gkVariable *prop);
	void removeDebugProperty(gkVariable *prop);

	static Ogre::Real getStepRate();
	static Ogre::Real getTickRate();
	static Ogre::Real getAnimRate();
private:

	Ogre::Root* mRoot;
	Ogre::RenderWindow *mWindow;
	bool mInitialized;
	gkUserDefs* mDefs;

	gkEnginePrivate* mPrivate;

	static Ogre::Real mTickRate;
	static Ogre::Real mAnimRate;
};



#endif//_gkEngine_h_
