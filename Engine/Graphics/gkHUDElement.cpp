/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom).
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
#include "gkLogger.h"
#include "OgreOverlay.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayManager.h"
#include "OgrePanelOverlayElement.h"
#include "OgreTechnique.h"

gkHUDElement::gkHUDElement(const gkString& name)
	:    m_name(name), m_element(0), m_parent(0), m_alphaBlend(2.f)
{
	if (Ogre::OverlayManager::getSingleton().hasOverlayElement(name))
	{
		m_element = Ogre::OverlayManager::getSingleton().getOverlayElement(name);
		if (m_element)
		{
			m_position.x = m_element->getLeft();
			m_position.y = m_element->getTop();
		}
	}
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

gkString gkHUDElement::getValue()
{
	if (m_element)
		return m_element->getCaption();

	return "";
}

void gkHUDElement::setUvCoords(float u1, float v1, float u2, float v2)
{
	if (m_element)
	{
		Ogre::PanelOverlayElement* panel = dynamic_cast<Ogre::PanelOverlayElement*>(m_element);
		if (panel) panel->setUV(u1, v1, u2, v2);
	}
}

void gkHUDElement::setPosition(float x,float y)
{
	if (m_element)
	{
		Ogre::PanelOverlayElement* panel = dynamic_cast<Ogre::PanelOverlayElement*>(m_element);
		if (panel) panel->setPosition(x,y);
	}
}

gkString gkHUDElement::getMaterialName(void)
{
	if (m_element)
		return m_element->getMaterialName();

	return "";
}

void gkHUDElement::setMaterialName(const gkString& material)
{
	try
	{
		if (m_element)
		{
			m_alphaBlend = 2.f;
			return m_element->setMaterialName(material);
		}
	}
	catch (Ogre::Exception& e)
	{
		gkPrintf("HUD Error: %s", e.what());
	}
}

void gkHUDElement::setMaterialAlpha(float factor)
{
	if (!m_element) return;

	try
	{
		Ogre::MaterialPtr material = m_element->getMaterial();
		Ogre::Pass* pass = material->getTechnique(0)->getPass(0);		
		if (m_alphaBlend > 1.f)
		{
			pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);				
			m_alphaBlend = 1.f;
		}
		Ogre::TextureUnitState* tu = pass->getTextureUnitState(0);
		if (tu)
		{
			factor = gkClamp(factor, 0.f, 1.f);
			tu->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, factor);
			m_alphaBlend = factor;
		}
	}
	catch (Ogre::Exception& e)
	{
		gkPrintf("HUD Error: %s", e.what());
	}
}

void gkHUDElement::setMaterialAlphaRejectValue(int val, bool isGreater)
{
	if (!m_element) return;
	int alpha = gkClamp(val, 0, 255);
	try
	{
		Ogre::MaterialPtr material = m_element->getMaterial();
		Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
		if (isGreater)
			pass->setAlphaRejectSettings(Ogre::CMPF_GREATER, alpha);
		else
			pass->setAlphaRejectSettings(Ogre::CMPF_LESS_EQUAL, alpha);
	}
	catch (Ogre::Exception& e)
	{
		gkPrintf("HUD Error: %s", e.what());
	}
}

int gkHUDElement::getMaterialAlphaRejectValue()
{
	if (!m_element) return 0;

	try
	{
		Ogre::MaterialPtr material = m_element->getMaterial();
		Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
		return pass->getAlphaRejectValue();
	}
	catch (Ogre::Exception& e)
	{
		gkPrintf("HUD Error: %s", e.what());
	}

	return 0;
}


float gkHUDElement::getMaterialAlpha()
{
	return m_alphaBlend > 1.f ? 1.f : m_alphaBlend;
}

void gkHUDElement::setParameter(const gkString& name, const gkString& value)
{
	if (m_element)
		m_element->setParameter(name, value);
}

gkString gkHUDElement::getParameter(const gkString& name)
{
	return m_element ? m_element->getParameter(name) : "";
}


Ogre::OverlayElement* gkHUDElement::_getElement(void)
{
	return m_element;
}


void gkHUDElement::_setElement(Ogre::OverlayElement* ele)
{
	m_element = ele;
}

void gkHUDElement::show(bool v)
{
	if (!m_element) return;

	if (v)
		m_element->show();
	else
		m_element->hide();
}

gkVector2 gkHUDElement::getPosition()
{

	if (m_element)
	{
		m_position.x = m_element->getLeft();
		m_position.y = m_element->getTop();

	}

	return m_position;
}
