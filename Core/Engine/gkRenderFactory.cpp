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
#include "gkRenderFactory.h"
#include "OgreRoot.h"

#ifdef OGREKIT_GLRS
#include "OgreGLPlugin.h"
#endif

#ifdef OGREKIT_D3D9RS
#include "OgreD3D9Plugin.h"
#endif

#ifdef OGREKIT_D3D10RS
#include "OgreD3D10Plugin.h"
#endif


OgreRenderSystem gkFindRenderSystem(OgreRenderSystem wanted)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
# ifdef OGREKIT_D3D9RS
	if (wanted == OGRE_RS_D3D9) return OGRE_RS_D3D9;
# endif
# ifdef OGREKIT_D3D10RS
	if (wanted == OGRE_RS_D3D10) return OGRE_RS_D3D10;
# endif
#endif
	// TODO setup OpenGL ES
#ifdef OGREKIT_GLRS
	// OpenGL is the default 
	return OGRE_RS_GL;
#endif
	return OGRE_RS_UNKNOWN;
}


// ----------------------------------------------------------------------------
gkRenderFactoryPrivate::gkRenderFactoryPrivate() :
	mRenderSystem(0)
{
}

// ----------------------------------------------------------------------------
gkRenderFactoryPrivate::~gkRenderFactoryPrivate()
{
	if (mRenderSystem)
	{
		delete mRenderSystem;
		mRenderSystem= 0;
	}
}

// ----------------------------------------------------------------------------
void gkRenderFactoryPrivate::createRenderSystem(Ogre::Root* r, OgreRenderSystem backend)
{
	if (mRenderSystem != 0)
		return;

	switch (gkFindRenderSystem(backend))
	{
	case OGRE_RS_D3D10:
#if OGREKIT_D3D10RS
		mRenderSystem= new Ogre::D3D10Plugin();
		r->installPlugin(mRenderSystem);
#endif
		break;
	case OGRE_RS_D3D9:
#ifdef OGREKIT_D3D9RS
		mRenderSystem= new Ogre::D3D9Plugin();
		r->installPlugin(mRenderSystem);
#endif
		break;
	case OGRE_RS_GLES:
	case OGRE_RS_GL:
#ifdef OGREKIT_GLRS
		mRenderSystem= new Ogre::GLPlugin();
		r->installPlugin(mRenderSystem);
#endif
		break;
	case OGRE_RS_UNKNOWN:
	default:
		break;
	}

	GK_ASSERT(mRenderSystem);
}
