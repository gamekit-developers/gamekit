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
#ifndef _gkObjectLoader_h_
#define _gkObjectLoader_h_

#include "gkLoaderCommon.h"
#include "gkManualLoader.h"




// ----------------------------------------------------------------------------
class gkGameObjectLoader : public gkManualObjectLoader
{
public:
	gkGameObjectLoader(gkBlendFile *fp, Blender::Scene* sc, Blender::Object *ob);
	virtual ~gkGameObjectLoader();

	void loadObject(gkGameObject *ob);

protected:
	void setLight(gkGameObject *ob);
	void setCamera(gkGameObject *ob);
	void setEntity(gkGameObject *ob);

	gkBlendFile *mFile;
	Blender::Object*	mObject;
	Blender::Scene*	 mScene;
};





#endif//_gkObjectLoader_h_
