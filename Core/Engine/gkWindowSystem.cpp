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
#include "OgrePrerequisites.h"
#include "gkWindowSystem.h"
#include "gkInternalWindowSystem.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
static gkWindowSystem *gsWindowSystem= 0;

// ----------------------------------------------------------------------------
gkWindowSystem::gkWindowSystem() :
		mExitReq(true), mEscDisable(false), mArea(0,0,1,1)
{
}

// ----------------------------------------------------------------------------
gkWindowSystem::~gkWindowSystem()
{
}


// ----------------------------------------------------------------------------
gkWindowSystem *gkWindowSystem::initialize(gkWindowSystem::WindowBackend backend)
{
	if (gsWindowSystem)
		return gsWindowSystem;


	switch (backend)
	{
	case OGRE_BACKEND:
	default:
		gsWindowSystem= new gkInternalWindowSystem();
		break;
	}

	GK_ASSERT(gsWindowSystem);
	gsWindowSystem->setup();

	return gsWindowSystem;
}


// ----------------------------------------------------------------------------
void gkWindowSystem::finalize(void)
{
	if (gsWindowSystem)
	{
		delete gsWindowSystem;
		gsWindowSystem= 0;
	}
}

// ----------------------------------------------------------------------------
bool gkWindowSystem::exitRequest(void)
{
	return mExitReq;
}

// ----------------------------------------------------------------------------
void gkWindowSystem::exit(void)
{
	mExitReq= true;
}

// ----------------------------------------------------------------------------
gkWindowSystem& gkWindowSystem::getSingleton(void)
{
	GK_ASSERT(gsWindowSystem);
	return *gsWindowSystem;
}

// ----------------------------------------------------------------------------
gkWindowSystem* gkWindowSystem::getSingletonPtr(void)
{
	return gsWindowSystem;
}
