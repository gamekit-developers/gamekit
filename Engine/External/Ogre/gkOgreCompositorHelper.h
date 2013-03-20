/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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

/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
conditions of the standard open source license.
-----------------------------------------------------------------------------
*/

#ifndef _gkOgreCompositorHelper_h__
#define _gkOgreCompositorHelper_h__

//Demo note :
//This file contains three compositor logics for the HDR, Heat Vision and Gaussian Blur compositors.
//If you wish to use these compositors in your application, make sure to add this code as well.
//This code fits the plugin architecture pretty well, but was not exported to a plugin for simplification
//of the SDK package
//@see CompositorDemo::loadResources

#include "OgrePrerequisites.h"
#include "OgreCompositorLogic.h"
#include "OgreCompositorInstance.h"

class gkOgreCompositorHelper
{
public:
	static bool createHalftoneTexture();
	static bool createDitherTexture(int width, int height);
	static bool createHeatVisionCompositor();
	static bool createMotionBlurCompositor();
};


//The simple types of compositor logics will all do the same thing -
//Attach a listener to the created compositor
class gkOgreCompositorListenerFactoryLogic : public Ogre::CompositorLogic
{
public:
	/** @copydoc CompositorLogic::compositorInstanceCreated */
	virtual void compositorInstanceCreated(Ogre::CompositorInstance* newInstance) 
	{
		Ogre::CompositorInstance::Listener* listener = createListener(newInstance);
		newInstance->addListener(listener);
		mListeners[newInstance] = listener;
	}
	
	/** @copydoc CompositorLogic::compositorInstanceDestroyed */
	virtual void compositorInstanceDestroyed(Ogre::CompositorInstance* destroyedInstance)
	{
		delete mListeners[destroyedInstance];
		mListeners.erase(destroyedInstance);
	}

protected:
	//This is the method that implementations will need to override
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance) = 0;
private:
	typedef std::map<Ogre::CompositorInstance*, Ogre::CompositorInstance::Listener*> ListenerMap;
	ListenerMap mListeners;

};

//The compositor logic for the heat vision compositor
class gkOgreCompositorHeatVisionLogic : public gkOgreCompositorListenerFactoryLogic
{
protected:
	/** @copydoc gkOgreCompositorListenerFactoryLogic::createListener */
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
};

//The compositor logic for the hdr compositor
class gkOgreCompositorHDRLogic : public gkOgreCompositorListenerFactoryLogic
{
protected:
	/** @copydoc gkOgreCompositorListenerFactoryLogic::createListener */
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
};

//The compositor logic for the gaussian blur compositor
class gkOgreCompositorGaussianBlurLogic : public gkOgreCompositorListenerFactoryLogic
{
protected:
	/** @copydoc gkOgreCompositorListenerFactoryLogic::createListener */
	virtual Ogre::CompositorInstance::Listener* createListener(Ogre::CompositorInstance* instance);
};

class gkOgreCompositorHeatVisionListener : public Ogre::CompositorInstance::Listener
{
public:
    gkOgreCompositorHeatVisionListener();
    virtual ~gkOgreCompositorHeatVisionListener();
    virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
    virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
protected:
    Ogre::GpuProgramParametersSharedPtr fpParams;
    float start, end, curr;
    Ogre::Timer *timer;
};

class gkOgreCompositorHDRListener : public Ogre::CompositorInstance::Listener
{
protected:
	int mVpWidth, mVpHeight;
	int mBloomSize;
	// Array params - have to pack in groups of 4 since this is how Cg generates them
	// also prevents dependent texture read problems if ops don't require swizzle
	float mBloomTexWeights[15][4];
	float mBloomTexOffsetsHorz[15][4];
	float mBloomTexOffsetsVert[15][4];
public:
	gkOgreCompositorHDRListener();
	virtual ~gkOgreCompositorHDRListener();
	void notifyViewportSize(int width, int height);
	void notifyCompositor(Ogre::CompositorInstance* instance);
	virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
	virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
};

class gkOgreCompositorGaussianListener : public Ogre::CompositorInstance::Listener
{
protected:
	int mVpWidth, mVpHeight;
	// Array params - have to pack in groups of 4 since this is how Cg generates them
	// also prevents dependent texture read problems if ops don't require swizzle
	float mBloomTexWeights[15][4];
	float mBloomTexOffsetsHorz[15][4];
	float mBloomTexOffsetsVert[15][4];
public:
	gkOgreCompositorGaussianListener();
	virtual ~gkOgreCompositorGaussianListener();
	void notifyViewportSize(int width, int height);
	virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
	virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
};


#endif//_gkOgreCompositorHelper_h__
