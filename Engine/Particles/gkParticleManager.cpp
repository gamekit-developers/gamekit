/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr.

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


#include "gkSerialize.h"
#include "gkParticleManager.h"
#include "gkParticleResource.h"
#include "Ogre/gkOgreParticleAffector.h"
#include "Ogre/gkOgreParticleEmitter.h"
#include "Ogre/gkOgreParticleRenderer.h"
#include "Ogre/gkOgreParticleResource.h"
#include "gkLogger.h"
#include "User/gkParticleTemplates.inl"

#include "Ogre.h"

#define DEFAULT_HALO_MAT	"<gkBuiltin/Halo>"
#define HALO_IMAGE_NAME		"<gkBuiltin/FlareAlpah64.png>"

//TODO: experiment with a bullet based particle system

#define gkOgreParticleManager gkParticleManager::Private

class gkOgreParticleManager
{
public:
	gkOgreEmitterFactory*			m_emitterFactory;
	gkOgreAffectorFactory*			m_affectorFactory;
	gkOgreParticleRendererFactory*	m_rendererFactory;

	Private()
	{
		m_emitterFactory  = new gkOgreEmitterFactory();
		m_affectorFactory = new gkOgreAffectorFactory();
		m_rendererFactory = new gkOgreParticleRendererFactory();

		Ogre::ParticleSystemManager::getSingleton().addEmitterFactory(m_emitterFactory);
		Ogre::ParticleSystemManager::getSingleton().addAffectorFactory(m_affectorFactory);
		Ogre::ParticleSystemManager::getSingleton().addRendererFactory(m_rendererFactory);
	}

	virtual ~Private()
	{
		delete m_emitterFactory;  m_emitterFactory  = 0;
		delete m_affectorFactory; m_affectorFactory = 0;
		delete m_rendererFactory; m_rendererFactory = 0;

		Ogre::ParticleSystemManager::getSingleton().removeAllTemplates();
	}

	void initialize(void)
	{
		try
		{
			const gkString imgName = HALO_IMAGE_NAME;

			Ogre::DataStreamPtr stream(OGRE_NEW Ogre::MemoryDataStream((void*)(FLARE_ALPHA_64), FLARE_ALPHA_64_SIZE));
			Ogre::Image img;
			img.load(stream);
			Ogre::TextureManager::getSingleton().loadImage(imgName, GK_BUILTIN_GROUP, img);

			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(DEFAULT_HALO_MAT, GK_BUILTIN_GROUP);
			Ogre::Pass *pass = mat->getTechnique(0)->getPass(0);

			pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
			pass->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 150);
			pass->setDepthWriteEnabled(false);
			Ogre::TextureUnitState* tu = pass->createTextureUnitState(imgName);		
		}
		catch(Ogre::Exception& e)
		{
			gkLogMessage("gkParticleManager: " << e.getDescription());
		}
	}

	gkString createOrRetrieveHaloMaterial(const gkString& baseMatName)
	{
		gkString matName = DEFAULT_HALO_MAT;

		try
		{
			gkString haloMatName = baseMatName + ".halo";
			Ogre::MaterialManager& mmgr = Ogre::MaterialManager::getSingleton();
		
			if (mmgr.resourceExists(haloMatName))
				matName = haloMatName;
			else
			{
				Ogre::MaterialPtr baseMat = mmgr.getByName(baseMatName);
				if (!baseMat.isNull())
				{
					Ogre::MaterialPtr mat = mmgr.create(haloMatName, baseMat->getGroup());
					baseMat->copyDetailsTo(mat);
					Ogre::Pass *pass = mat->getTechnique(0)->getPass(0);

					pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
					pass->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 150);
					pass->setDepthWriteEnabled(false);
					Ogre::TextureUnitState* tu = pass->createTextureUnitState(HALO_IMAGE_NAME);	

					matName = haloMatName;
				}
			}		
		}
		catch(Ogre::Exception& e)
		{
			gkLogMessage("gkParticleManager: " << e.getDescription());
		}
		return matName;
	}

	gkOgreParticleResource* createParticle(gkResourceManager* manager, const gkResourceName& name, const gkResourceHandle& handle)
	{
		return new gkOgreParticleResource(manager, name, handle);
	}
};

//--

gkParticleManager::gkParticleManager()
	:    gkResourceManager("ParticleManager", "Particles")
{
	m_private = new gkOgreParticleManager;
}

gkParticleManager::~gkParticleManager()
{
	destroyAll();

	delete m_private; m_private = 0;
}

void gkParticleManager::initialize(void)
{
	m_private->initialize();
}

gkString gkParticleManager::createOrRetrieveHaloMaterial(const gkString& baseMatName)
{
	return m_private->createOrRetrieveHaloMaterial(baseMatName);
}

gkResource* gkParticleManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	return m_private->createParticle(this, name, handle);	
}

gkParticleResource* gkParticleManager::createParticle(const gkResourceName& name, gkParticleSettingsProperties& pp)
{
	gkParticleResource* resource = create<gkParticleResource>(name);
	if (resource)
		resource->createParticle(pp);
	return resource;
}

gkParticleResource* gkParticleManager::createParticle(const gkResourceName& name)
{
	gkParticleResource* resource = create<gkParticleResource>(name);
	if (resource)
		resource->createParticle();
	return resource;
}


UT_IMPLEMENT_SINGLETON(gkParticleManager);
