/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef AKANIMATIONLOADER_H
#define AKANIMATIONLOADER_H


#include "fbtBlend.h"
#include "akMathUtils.h"

class akDemoBase;
class akAnimationClip;

class akAnimationLoader
{
public:

	akAnimationLoader(akDemoBase* demo) : m_demo(demo) {}
	~akAnimationLoader() {}

	void convertAction(Blender::bAction* action, bool pre25compat, akScalar animfps);
	void convertActions(fbtList actions, bool pre25compat, akScalar animfps);

	void convertObject(class akEntity* obj, Blender::Object* bobj, bool pre25compat, akScalar animfps);
	
	
private:
	akDemoBase* m_demo;
	
	akAnimationClip * convertObjectIpoToAnimation(Blender::Ipo *bipo, akScalar animfps);
	void convertAction24(Blender::bAction *action, akScalar animfps);
	void convertAction25(Blender::bAction *action, akScalar animfps);
	void convert25AnimData(akEntity *obj, Blender::AnimData *adt, akScalar animfps);
};
#endif // AKANIMATIONLOADER_H
