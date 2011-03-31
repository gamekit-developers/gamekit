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
#ifndef _gkAnimationConverter_h_
#define _gkAnimationConverter_h_

#include "Loaders/Blender2/gkBlendInternalFile.h"
#include "gkMathUtils.h"

class gkAnimationLoader
{
	const gkResourceNameString	m_groupName;

	gkAnimation* convertObjectIpoToAnimation(Blender::Ipo* bipo, gkScalar animfps);
	void convertAction24(Blender::bAction* action, gkScalar animfps);
	void convertAction25(Blender::bAction* action, gkScalar animfps);
	void convert25AnimData(gkGameObject* obj, Blender::AnimData* adt, gkScalar animfps);

public:

	gkAnimationLoader(const gkResourceNameString& groupName="") : m_groupName(groupName) {}
	~gkAnimationLoader() {}

	void convertAction(Blender::bAction* action, bool pre25compat, gkScalar animfps);
	void convertActions(gkBlendListIterator& actions, bool pre25compat, gkScalar animfps);

	void convertObject(class gkGameObject* obj, Blender::Object* bobj, bool pre25compat, gkScalar animfps);
	void convertLamp(class gkGameObject* obj, Blender::Object* bobj, bool pre25compat, gkScalar animfps);
	void convertCamera(class gkGameObject* obj, Blender::Object* bobj, bool pre25compat, gkScalar animfps);
	void convertMesh(class gkGameObject* obj, Blender::Object* bobj, bool pre25compat, gkScalar animfps);
	void convertArmature(class gkGameObject* obj, Blender::Object* bobj, bool pre25compat, gkScalar animfps);

};

#endif//_gkAnimationConverter_h_
