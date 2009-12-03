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


using namespace Ogre;


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
void gkRenderFactoryPrivate::createRenderSystem(Root * r, OgreRenderSystem backend)
{
	if (mRenderSystem != 0)
		return;

	switch (backend)
	{
	case OGRE_RS_GL:
#ifdef OGREKIT_GLRS
		mRenderSystem= new GLPlugin();
		r->installPlugin(mRenderSystem);
#endif
	default:
		break;
	}

}
