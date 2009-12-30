/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
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
#include "blender.h"
#include "bAsciiMain.h"


// ----------------------------------------------------------------------------
void bAsciiMain::clearMain(void)
{
	freeObject();
	freeCamera();
	freeMesh();
	freeScene();
}

// ----------------------------------------------------------------------------
void bAsciiMain::freeObject(void)
{
	for (size_t i=0; i<object.size(); ++i)
		utMemUtils::tdealloc(object[i]);
}

// ----------------------------------------------------------------------------
void bAsciiMain::freeCamera(void)
{
	for (size_t i=0; i<camera.size(); ++i)
		utMemUtils::tdealloc(camera[i]);
}

// ----------------------------------------------------------------------------
void bAsciiMain::freeMesh(void)
{
	for (size_t i=0; i<mesh.size(); ++i)
	{
		Blender::Mesh *me = mesh[i];

		if (me->mvert)
			utMemUtils::tdealloc(me->mvert);
		if (me->mface)
			utMemUtils::tdealloc(me->mface);
		if (me->mat)
			utMemUtils::tdealloc(me->mat);
		utMemUtils::tdealloc(me);
	}
}


// ----------------------------------------------------------------------------
void bAsciiMain::freeScene(void)
{
	for (size_t i=0; i<scene.size(); ++i)
	{
		Blender::Scene *sc = scene[i];

		Blender::Base *base = (Blender::Base*)sc->base.first;
		while (base)
		{
			Blender::Base* next = base->next;
			utMemUtils::tdealloc(base);
			base = next;
		}

		utMemUtils::tdealloc(sc);
	}
}
