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
#ifndef _gkAnimationManager_h_
#define _gkAnimationManager_h_

#include "utSingleton.h"
#include "gkResourceManager.h"

#include "gkAnimation.h"

///Manage all the actions/animations
class gkAnimationManager : public gkResourceManager, public utSingleton<gkAnimationManager>
{
public:
	enum gkAnimationTypes
	{
		GK_ACT_NULL = 0,
		GK_ACT_KEYED,
		GK_ACT_SEQ,
	};

private:
	int m_currentType;

public:
	gkAnimationManager();
	virtual ~gkAnimationManager();
	
	gkKeyedAnimation*    createKeyedAnimation(const gkResourceName& name);
	gkAnimationSequence* createAnimationSequence(const gkResourceName& name);

	gkKeyedAnimation*    getKeyedAnimation(const gkResourceName& name);
	gkAnimationSequence* getAnimationSequence(const gkResourceName& name);
	
	gkAnimation*         getAnimation(const gkResourceName& name);
	
	UT_DECLARE_SINGLETON(gkAnimationManager);
	
private:
	virtual gkResource* createImpl(const gkResourceName &name, const gkResourceHandle &handle);
	

};

#endif//_gkAnimationManager_h_
