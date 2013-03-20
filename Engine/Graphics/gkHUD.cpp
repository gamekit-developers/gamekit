/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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
    This is a replacement candidate for Engine/Graphics
    - For now it wraps Ogre::Overlays to gk usable objects.
    - Later it will be superseded by custom Ogre vertex buffer usage,
    - abstracted for external GameKit external rendering.
*/
#include "gkHUD.h"
#include "gkHUDElement.h"
#include "gkLogger.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayContainer.h"
#include "OgreOverlayManager.h"



gkHUD::gkHUD(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkResource(creator, name, handle), m_overlay(0)
{
	newImpl();
}


gkHUD::~gkHUD()
{
	if (m_overlay)
	{
		m_overlay->clear();
	}

	ChildNodes::Iterator it = m_children.iterator();
	while (it.hasMoreElements())
		delete it.getNext();

	m_children.clear();
}


void gkHUD::show(bool v)
{
	if (m_overlay)
	{
		if (v)
			m_overlay->show();
		else
			m_overlay->hide();
	}
}



void gkHUD::addChild(gkHUDElement* hud)
{
	GK_ASSERT(hud && getChildIndex(hud->getName()) == UT_NPOS);

	m_children.push_back(hud);
}

void gkHUD::removeChild(gkHUDElement* hud)
{
	GK_ASSERT(hud);

	UTsize i = getChildIndex(hud->getName());
	if (i != UT_NPOS)
	{
		m_children.erase(i);
	}
}


gkHUDElement* gkHUD::getChild(const gkString& name)
{
	UTsize i = getChildIndex(name);
	return i != UT_NPOS ? m_children[i] : 0;
}


Ogre::Overlay* gkHUD::_getOverlay(void)
{
	return m_overlay;
}


void gkHUD::_setOverlay(Ogre::Overlay* over)
{
	m_overlay = over;
}

UTsize gkHUD::getChildIndex(const gkString& name)
{
	for (UTsize i = 0; i < m_children.size(); i++)
		if (m_children[i]->getName() == name)
			return i;

	return UT_NPOS;
}


void gkHUD::newImpl(void)
{

	try
	{

		m_overlay = Ogre::OverlayManager::getSingleton().getByName(m_name.getName());

		// Auto create if it's here
		if (m_overlay)
		{
			Ogre::Overlay::Overlay2DElementsIterator elements = m_overlay->get2DElementsIterator();

			while (elements.hasMoreElements())
			{
				Ogre::OverlayContainer* cont = elements.getNext();
				addChild(new gkHUDElement(cont->getName()));

				Ogre::OverlayContainer::ChildIterator childs = cont->getChildIterator();

				while (childs.hasMoreElements())
				{
					Ogre::OverlayElement* elm = childs.getNext();
					gkHUDElement* hele = new gkHUDElement(elm->getName());
					addChild(hele);
				}
			}

		}
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("HUD: " << e.getDescription());
		m_overlay = 0;
	}
}
