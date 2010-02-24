/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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

#include "gkCursorNode.h"
#include "OgreOverlayManager.h"
#include "OgrePanelOverlayElement.h"

using namespace Ogre;

gkCursorNode::gkCursorNode(gkLogicTree *parent, size_t id)
: gkLogicNode(parent, id),
m_overlay(0),
m_panelContainer(0),
m_panelElement(0)
{
	ADD_ISOCK(*getUpdate(), this, gkLogicSocket::ST_BOOL);

	ADD_ISOCK(*getMaterialName(), this, gkLogicSocket::ST_STRING);

	ADD_ISOCK(*getX(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getY(), this, gkLogicSocket::ST_REAL);

	ADD_ISOCK(*getWidth(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getHeight(), this, gkLogicSocket::ST_REAL);

	static int idx = 0;

	Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();

	m_overlay = om.create("gkCursorNodeOverlay" + Ogre::StringConverter::toString(idx));

	m_overlay->setZOrder(400);

	Ogre::PanelOverlayElement* pPanelContainer = static_cast<Ogre::PanelOverlayElement*>(om.createOverlayElement("Panel", "gkCursorNodePanelContainer"));

	pPanelContainer->setMetricsMode(GMM_PIXELS);
	pPanelContainer->setTransparent(true);

	m_panelContainer = pPanelContainer;

	Ogre::PanelOverlayElement* pPanelElement = static_cast<Ogre::PanelOverlayElement*>(om.createOverlayElement("Panel", "gkCursorNodePanelElement"));

	pPanelElement->setMetricsMode(GMM_PIXELS);

	m_panelElement = pPanelElement;

	const gkScalar width = 32;
	getWidth()->setValue(width);
	
	const gkScalar height = 32;
	getHeight()->setValue(height);
	
	m_panelElement->setWidth(width);
	m_panelElement->setHeight(height);

	m_panelContainer->addChild(m_panelElement);

	m_overlay->add2D(m_panelContainer);
	
	m_overlay->show();

	++idx;
}

gkCursorNode::~gkCursorNode()
{
	Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();

	om.destroy(m_overlay);
}

bool gkCursorNode::evaluate(Real tick)
{
	return getUpdate()->getValueBool();
}

void gkCursorNode::update(Real tick)
{
	if(m_panelElement->getMaterialName() != getMaterialName()->getValueString())
	{
		m_panelElement->setMaterialName(getMaterialName()->getValueString());
	}

	gkScalar width = getWidth()->getValueReal();
	
	if(m_panelElement->getWidth() != width)
	{
		m_panelElement->setWidth(width);
	}

	gkScalar height = getHeight()->getValueReal();
	
	if(m_panelElement->getHeight() != height)
	{
		m_panelElement->setHeight(height);
	}

	m_panelContainer->setPosition(getX()->getValueReal(), getY()->getValueReal());
}



