/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): Charlie C.
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
#include "gkDebugFps.h"
#include "gkLogger.h"
#include "gkWindowSystem.h"
#include "gkWindow.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkStats.h"

#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayContainer.h"
#include "OgreFont.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreRenderWindow.h"
#include "OgreRenderTarget.h"

#define PROP_SIZE 14


gkDebugFps::gkDebugFps()
	:   m_isInit(false), m_isShown(false),
	    m_over(0), m_cont(0), m_key(0), m_val(0)
{
	m_keys = "";
	m_keys += "FPS:\n";
	m_keys += "Average:\n";
	m_keys += "Best:\n";
	m_keys += "Worst:\n";
	m_keys += "\n";
	m_keys += "Triangles:\n";
	m_keys += "Batch count:\n";
	m_keys += "\n";
	m_keys += "DBVT:\n";
	m_keys += "\n";
	m_keys += "Total:\n";
	m_keys += "Render:\n";
	m_keys += "Physics:\n";
	m_keys += "LogicBricks:\n";
	m_keys += "LogicNodes:\n";
	m_keys += "Sound:\n";
	m_keys += "DBVT:\n";
	m_keys += "Bufferswap&LOD:\n";
	m_keys += "Animations:\n";
}


gkDebugFps::~gkDebugFps()
{
}


void gkDebugFps::initialize(void)
{
	if (m_isInit)
		return;

	try
	{
		// always initialize after gkDebugScreen!


		Ogre::OverlayManager& mgr = Ogre::OverlayManager::getSingleton();
		m_over  = mgr.create("<gkBuiltin/gkDebugFps>");
		m_key   = mgr.createOverlayElement("TextArea", "<gkBuiltin/gkDebugFps/Keys>");
		m_val   = mgr.createOverlayElement("TextArea", "<gkBuiltin/gkDebugFps/Vals>");
		m_cont  = (Ogre::OverlayContainer*)mgr.createOverlayElement("Panel", "<gkBuiltin/gkDebugFps/Containter1>");


		m_cont->setMetricsMode(Ogre::GMM_PIXELS);
		m_cont->setVerticalAlignment(Ogre::GVA_TOP);
		m_cont->setHorizontalAlignment(Ogre::GHA_RIGHT);
		m_cont->setLeft(-16 * PROP_SIZE);
		m_cont->setTop(10);

		m_key->setMetricsMode(Ogre::GMM_PIXELS);
		m_key->setVerticalAlignment(Ogre::GVA_TOP);
		m_key->setHorizontalAlignment(Ogre::GHA_LEFT);

		m_val->setMetricsMode(Ogre::GMM_PIXELS);
		m_val->setVerticalAlignment(Ogre::GVA_TOP);
		m_val->setHorizontalAlignment(Ogre::GHA_LEFT);
		m_val->setLeft(8 * PROP_SIZE);

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


void gkDebugFps::show(bool v)
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


void gkDebugFps::draw(void)
{
	if (!m_over || !m_key || !m_val)
		return;

	Ogre::RenderWindow* window = gkWindowSystem::getSingleton().getMainWindow()->getRenderWindow();
	const Ogre::RenderTarget::FrameStats& ogrestats = window->getStatistics();

	gkVariable* dbvtVal = 0;
	gkDynamicsWorld* wo = gkEngine::getSingleton().getActiveScene()->getDynamicsWorld();
	if (wo) dbvtVal = wo->getDBVTInfo();


	float swap = gkStats::getSingleton().getLastTotalMicroSeconds() / 1000.0f;
	float render = gkStats::getSingleton().getLastRenderMicroSeconds() / 1000.0f;
	float phys = gkStats::getSingleton().getLastPhysicsMicroSeconds() / 1000.0f;
	float logicb = gkStats::getSingleton().getLastLogicBricksMicroSeconds() / 1000.0f;
	float logicn = gkStats::getSingleton().getLastLogicNodesMicroSeconds() / 1000.0f;
	float sound = gkStats::getSingleton().getLastSoundMicroSeconds() / 1000.0f;
	float dbvt = gkStats::getSingleton().getLastDbvtMicroSeconds() / 1000.0f;
	float bufswaplod = gkStats::getSingleton().getLastBufSwapLodMicroSeconds() / 1000.0f;
	float animations = gkStats::getSingleton().getLastAnimationsMicroSeconds() / 1000.0f;

	gkString vals = "";

	vals += Ogre::StringConverter::toString(ogrestats.lastFPS) + '\n';
	vals += Ogre::StringConverter::toString(ogrestats.avgFPS) + '\n';
	vals += Ogre::StringConverter::toString(ogrestats.bestFPS) + '\n';
	vals += Ogre::StringConverter::toString(ogrestats.worstFPS) + '\n';
	vals += '\n';

	vals += Ogre::StringConverter::toString(ogrestats.triangleCount) + '\n';
	vals += Ogre::StringConverter::toString(ogrestats.batchCount) + '\n';
	vals += '\n';

	if (dbvtVal) vals += dbvtVal->getValueString();
	else  vals += "Not Enabled\n";
	vals += '\n';

	vals += Ogre::StringConverter::toString(swap, 3, 7, '0', std::ios::fixed) + "ms 100%\n";

	vals += Ogre::StringConverter::toString(render, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * render / swap), 3 ) + "%\n";

	vals += Ogre::StringConverter::toString(phys, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * phys / swap), 3 ) + "%\n";

	vals += Ogre::StringConverter::toString(logicb, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * logicb / swap), 3 ) + "%\n";

	vals += Ogre::StringConverter::toString(logicn, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * logicn / swap), 3 ) + "%\n";

	vals += Ogre::StringConverter::toString(sound, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * sound / swap), 3 ) + "%\n";

	vals += Ogre::StringConverter::toString(dbvt, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * dbvt / swap), 3 ) + "%\n";

	vals += Ogre::StringConverter::toString(bufswaplod, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * bufswaplod / swap), 3 ) + "%\n";

	vals += Ogre::StringConverter::toString(animations, 3, 7, '0', std::ios::fixed) + "ms ";
	vals += Ogre::StringConverter::toString( int(100 * animations / swap), 3 ) + "%\n";

	if (!m_keys.empty() && !vals.empty())
	{
		m_key->setCaption(m_keys);
		m_val->setCaption(vals);
	}
}
