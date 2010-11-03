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
#include "gkActionManager.h"
#include "gkAction.h"
#include "gkActionSequence.h"
#include "gkKeyedAction.h"


gkActionManager::gkActionManager()
	: gkResourceManager("ActionManager", "Action")
{
}


gkActionManager::~gkActionManager()
{
	destroyAll();
}


gkKeyedAction* gkActionManager::createKeyedAction(const gkResourceName &name)
{
	m_currentType = GK_ACT_KEYED;
	return create<gkKeyedAction>(name);
}


gkActionSequence* gkActionManager::createActionSequence(const gkResourceName &name)
{
	m_currentType = GK_ACT_SEQ;
	return create<gkActionSequence>(name);
}


gkKeyedAction* gkActionManager::getKeyedAction(const gkResourceName &name)
{
	return getByName<gkKeyedAction>(name);
}


gkActionSequence* gkActionManager::getActionSequence(const gkResourceName &name)
{
	return getByName<gkActionSequence>(name);
}


gkResource* gkActionManager::createImpl(const gkResourceName &name, const gkResourceHandle &handle)
{

	int curType = m_currentType;
	m_currentType = GK_ACT_KEYED;

	switch (curType)
	{
	case GK_ACT_SEQ:     return new gkActionSequence(this, name, handle);
	}
	return new gkKeyedAction(this, name, handle);
}


UT_IMPLEMENT_SINGLETON(gkActionManager);
