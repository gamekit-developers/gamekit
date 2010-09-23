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
#include "gkDebugProperty.h"
#include "gkLogger.h"
#include "gkWindowSystem.h"
#include "gkEngine.h"

#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayContainer.h"
#include "OgreFont.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"

#define PROP_SIZE 14


gkDebugPropertyPage::gkDebugPropertyPage()
	:   m_isInit(false), m_isShown(false),
	    m_over(0), m_cont(0), m_key(0), m_val(0), m_props()
{
}


gkDebugPropertyPage::~gkDebugPropertyPage()
{
}


void gkDebugPropertyPage::initialize(void)
{
	if (m_isInit)
		return;

	try
	{
		// always initialize after gkDebugScreen!


		Ogre::OverlayManager& mgr = Ogre::OverlayManager::getSingleton();
		m_over  = mgr.create("<gkBuiltin/gkDebugPropertyPage>");
		m_key   = mgr.createOverlayElement("TextArea", "<gkBuiltin/gkDebugPropertyPage/Keys>");
		m_val   = mgr.createOverlayElement("TextArea", "<gkBuiltin/gkDebugPropertyPage/Vals>");
		m_cont  = (Ogre::OverlayContainer*)mgr.createOverlayElement("Panel", "<gkBuiltin/gkDebugPropertyPage/Containter1>");


		m_cont->setMetricsMode(Ogre::GMM_PIXELS);
		m_cont->setVerticalAlignment(Ogre::GVA_TOP);
		m_cont->setHorizontalAlignment(Ogre::GHA_LEFT);

		m_key->setMetricsMode(Ogre::GMM_PIXELS);
		m_key->setVerticalAlignment(Ogre::GVA_TOP);
		m_key->setHorizontalAlignment(Ogre::GHA_LEFT);

		m_val->setMetricsMode(Ogre::GMM_PIXELS);
		m_val->setVerticalAlignment(Ogre::GVA_TOP);
		m_val->setHorizontalAlignment(Ogre::GHA_LEFT);

		Ogre::TextAreaOverlayElement* textArea;

		textArea = static_cast<Ogre::TextAreaOverlayElement*>(m_key);
		textArea->setFontName("<gkBuiltin/Font>");
		textArea->setCharHeight(PROP_SIZE);
		textArea->setColour(Ogre::ColourValue::White);

		textArea = static_cast<Ogre::TextAreaOverlayElement*>(m_val);
		textArea->setFontName("<gkBuiltin/Font>");
		textArea->setCharHeight(PROP_SIZE);
		textArea->setColour(Ogre::ColourValue::White);



		m_over->setZOrder(500);
		m_cont->addChild(m_key);
		m_cont->addChild(m_val);
		m_over->add2D(m_cont);
	}
	catch (Ogre::Exception& e)
	{
		gkPrintf("%s", e.getDescription().c_str());
		return;
	}

	m_isInit = true;

}


void gkDebugPropertyPage::show(bool v)
{
	if (m_over != 0 && m_isShown != v)
	{
		m_isShown = v;
		if (m_isShown)
			m_over->show();
		else
			m_over->hide();
	}
}



void gkDebugPropertyPage::addVariable(gkVariable* prop)
{
	if (hasVariable(prop))
		return;
	m_props.push_back(prop);


	gkScalar strWidth = PROP_SIZE * prop->getName().size();
	gkScalar nh = m_cont->getHeight() + PROP_SIZE;
	m_cont->setHeight(nh);

	if (m_key->getWidth() < strWidth || m_props.size() == 1)
	{
		gkScalar diffw = strWidth - m_key->getWidth();
		m_cont->setWidth(m_cont->getWidth() + diffw);

		gkScalar half = m_cont->getWidth() / 2.f;
		m_key->setWidth(half);
		m_val->setWidth(half);
		m_val->setPosition(m_props.size() == 1 ? strWidth : half, 0);

	}
}



void gkDebugPropertyPage::removeVariable(gkVariable* prop)
{
	VariableList::Pointer p = m_props.find(prop);
	if (p != 0)
		m_props.erase(p);
}



bool gkDebugPropertyPage::hasVariable(gkVariable* prop)
{
	return m_props.find(prop) != 0;
}


void gkDebugPropertyPage::clearProps(void)
{
	m_props.clear();
	show(false);
}



void gkDebugPropertyPage::draw(void)
{
	if (!m_over || !m_key || m_props.empty())
		return;


	utListIterator<VariableList> iter(m_props);

	gkString keys = "", vals = "";

	while (iter.hasMoreElements())
	{
		gkVariable* prop = iter.getNext();
		keys += prop->getName() + ":\n";
		vals += prop->getValueString() + '\n';
	}

	if (!keys.empty() && !vals.empty())
	{
		m_key->setCaption(keys);
		m_val->setCaption(vals);
	}
}
