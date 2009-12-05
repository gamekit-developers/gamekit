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
#ifndef _gkLoaderUtils_h_
#define _gkLoaderUtils_h_

#include "gkLoaderCommon.h"
#include "blender_Common.h" // forwards



//-----------------------------------------------------------------------------
class gkLoaderUtils
{
public:
	gkLoaderUtils(bParse::bBlenderFile *file);
	Blender::Material* getMaterial(Blender::Object *ob, int index);
	void getLayers(Blender::Mesh *mesh, Blender::MTFace **eightLayerArray, Blender::MCol **oneMCol, int &validLayers);

	// animation / skeleton utils
	int getDeformGroupIndex(Blender::Object *ob, const char *group);
	static Ogre::Real evaluateIpoCurve(Blender::IpoCurve *icu, const Ogre::Real& time);

	static bool findApproximateCurve(Ogre::Vector2 &P0,
									 Ogre::Vector2 &P1,
									 Ogre::Vector2 &P2,
									 Ogre::Vector2& P3,
									 Ogre::Real time,
									 Ogre::Real &cval);

	/// defined in scene->r.frs_sec
	static Ogre::Real blender_anim_rate;

private:
	bParse::bBlenderFile *mFile; // incase any file info is needed (try not to use for finding ptrs)
};




#endif//_gkLoaderUtils_h_
