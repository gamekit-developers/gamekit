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
#include "gkRenderFactory.h"
#include "OgreRoot.h"

#ifdef OGRE_BUILD_RENDERSYSTEM_GL
#include "OgreGLPlugin.h"
#endif

#ifdef OGRE_BUILD_RENDERSYSTEM_GLES
#include "OgreGLESPlugin.h"
#endif

#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
#include "OgreGLES2Plugin.h"
#endif

#ifdef OGREKIT_BUILD_D3D9RS
#include "OgreD3D9Plugin.h"
#endif


#ifdef OGREKIT_BUILD_D3D11RS
#include "OgreD3D11Plugin.h"
#endif

#include "OgreParticleFXPlugin.h"

OgreRenderSystem gkFindRenderSystem(OgreRenderSystem wanted)
{
#if GK_PLATFORM == GK_PLATFORM_WIN32
# ifdef OGREKIT_BUILD_D3D9RS
	if (wanted == OGRE_RS_D3D9) return OGRE_RS_D3D9;
# endif
# ifdef OGREKIT_BUILD_D3D11RS
	if (wanted == OGRE_RS_D3D11) return OGRE_RS_D3D11;
# endif
#endif

#ifdef OGRE_BUILD_RENDERSYSTEM_GL
	// OpenGL is the default
	return OGRE_RS_GL;
#endif
#ifdef OGRE_BUILD_RENDERSYSTEM_GLES
	return OGRE_RS_GLES;
#endif
#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
	return OGRE_RS_GLES2;
#endif
	return OGRE_RS_UNKNOWN;
}



gkRenderFactoryPrivate::gkRenderFactoryPrivate() :
	m_renderSystem(0), m_particleSystem(0), m_shaderLang()
{
}


gkRenderFactoryPrivate::~gkRenderFactoryPrivate()
{
	if (m_particleSystem)
	{
		delete m_particleSystem;
		m_particleSystem = 0;
	}

	if (m_renderSystem)
	{
		delete m_renderSystem;
		m_renderSystem = 0;
	}
}


void gkRenderFactoryPrivate::createRenderSystem(Ogre::Root* r, OgreRenderSystem backend)
{
	if (m_renderSystem != 0)
		return;

	switch (gkFindRenderSystem(backend))
	{
	case OGRE_RS_D3D9:
#ifdef OGREKIT_BUILD_D3D9RS
		m_renderSystem = new Ogre::D3D9Plugin();
		r->installPlugin(m_renderSystem);
		m_shaderLang = "hlsl";
#endif
		break;
	case OGRE_RS_D3D11:
#ifdef OGREKIT_BUILD_D3D11RS
		m_renderSystem = new Ogre::D3D11Plugin();
		r->installPlugin(m_renderSystem);
		m_shaderLang = "hlsl";
#endif
		break;
	case OGRE_RS_GLES:
#ifdef OGRE_BUILD_RENDERSYSTEM_GLES
		m_renderSystem = new Ogre::GLESPlugin();
		r->installPlugin(m_renderSystem);
		m_shaderLang = "";
#endif
		break;
	case OGRE_RS_GLES2:
#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
		m_renderSystem = new Ogre::GLES2Plugin();
		r->installPlugin(m_renderSystem);
		m_shaderLang = "glsles";
#endif
		break;
	case OGRE_RS_GL:
#ifdef OGRE_BUILD_RENDERSYSTEM_GL
		m_renderSystem = new Ogre::GLPlugin();
		r->installPlugin(m_renderSystem);
		m_shaderLang = "glsl";
#endif
		break;
	case OGRE_RS_UNKNOWN:
	default:
		m_shaderLang = "";
		break;
	}

	GK_ASSERT(m_renderSystem);
}

void gkRenderFactoryPrivate::createParticleSystem(Ogre::Root* r)
{
#ifdef OGREKIT_USE_PARTICLE
	m_particleSystem = new Ogre::ParticleFXPlugin();
	r->installPlugin(m_particleSystem);
#endif
}
