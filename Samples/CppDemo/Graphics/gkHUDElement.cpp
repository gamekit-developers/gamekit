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
    This is a replacement candidate for Engine/Graphics
    - For now it wraps Ogre::Overlays to gk usable objects.
    - Later it will be superseded by custom Ogre vertex buffer usage,
    - abstracted for external GameKit external rendering.
*/
#include "gkHUDElement.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayManager.h"



gkHUDElement::gkHUDElement(const gkString& name)
	:    m_name(name), m_element(0)
{
	if (Ogre::OverlayManager::getSingleton().hasOverlayElement(name))
		m_element = Ogre::OverlayManager::getSingleton().getOverlayElement(name);
}



gkHUDElement::~gkHUDElement()
{
}



void gkHUDElement::setValue(const gkVariable& v)
{
	if (m_element)
		m_element->setCaption(v.getValueString());
}

void gkHUDElement::setValue(const gkString& v)
{
	if (m_element)
		m_element->setCaption(v);
}


Ogre::OverlayElement* gkHUDElement::_getElement(void)
{
	return m_element;
}


void gkHUDElement::_setElement(Ogre::OverlayElement* ele)
{
	m_element = ele;
}
