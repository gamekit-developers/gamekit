/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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

#include "gkCommon.h"
#include "gkViewport.h"
#include "gkLogger.h"
#include "gkWindowSystem.h"
#include "gkWindow.h"
#include "External/Ogre/gkOgreCompositorManager.h"
#include "External/Ogre/gkOgreCompositorHelper.h"

#include "Ogre.h"
#include "OgreCompositorChain.h"
#include "OgreCompositorManager.h"
#include "OgreCompositorLogic.h"
#include "OgreCompositorInstance.h"
#include "OgreCompositionPass.h"
#include "OgreCompositionTargetPass.h"

UT_IMPLEMENT_SINGLETON(gkCompositorManager)

gkCompositorManager::gkCompositorManager()
	:	m_halftonInited(false), 
		m_ditherInited(false), 
		m_heatVisionInited(false),
		m_motionBlurInited(false)
{
}

gkCompositorManager::~gkCompositorManager()
{
	Ogre::CompositorManager& mgr = Ogre::CompositorManager::getSingleton();

	mgr.unregisterCompositorLogic(GK_COMPOSITOR_HDR);
	mgr.unregisterCompositorLogic(GK_COMPOSITOR_GAUSSIAN_BLUR);
#if GK_USE_COMPOSITOR_HEAT_VISION
	mgr.unregisterCompositorLogic(GK_COMPOSITOR_HEAT_VISION);
#endif

	UTsize i;
	for (i = 0; i < m_logics.size(); i++)
		delete m_logics[i];

	m_logics.clear();
}

void gkCompositorManager::initialize()
{
	Ogre::CompositorManager& mgr = Ogre::CompositorManager::getSingleton();

	gkOgreCompositorListenerFactoryLogic *logic = 0;
	
	logic = new gkOgreCompositorHDRLogic;			
	mgr.registerCompositorLogic(GK_COMPOSITOR_HDR, logic);
	m_logics.push_back(logic);

	logic = new gkOgreCompositorGaussianBlurLogic;		
	mgr.registerCompositorLogic(GK_COMPOSITOR_GAUSSIAN_BLUR, logic); //"GaussianBlur"
	m_logics.push_back(logic);

#if GK_USE_COMPOSITOR_HEAT_VISION
	logic = new gkOgreCompositorHeatVisionLogic;	
	mgr.registerCompositorLogic(GK_COMPOSITOR_HEAT_VISION, logic);
	m_logics.push_back(logic);
#endif

}

void gkCompositorManager::removeCompositorChain(gkViewport* viewport)
{
	GK_ASSERT(viewport);

	Ogre::Viewport *vp = viewport->getViewport(); 
	if (vp)
		Ogre::CompositorManager::getSingleton().removeCompositorChain(vp);
}

bool gkCompositorManager::setCompositorChain(gkCompositorOp op, const gkString& compositorName, int viewportIndex)
{
	gkViewport *viewport = gkWindowSystem::getSingleton().getMainWindow()->getViewport(viewportIndex);
	if (!viewport) return false;

	return setCompositorChain(op, compositorName, viewport);
}

bool gkCompositorManager::setCompositorChain(gkCompositorOp op, const gkString& compositorName, gkViewport *viewport)
{
	GK_ASSERT(viewport && viewport->getViewport());

	bool found = false;

	Ogre::Viewport *vp = viewport->getViewport(); 

	int width = vp->getActualWidth(), height = vp->getActualHeight();

	Ogre::CompositorChain *chain = Ogre::CompositorManager::getSingleton().getCompositorChain(vp); assert(chain);
	for (size_t i = 0; i < chain->getNumCompositors(); i++) 
	{
		Ogre::CompositorInstance *ci = chain->getCompositor(i); assert(ci);
		bool match = (compositorName == ci->getCompositor()->getName());
		
		if (op == GK_COMPOSITOR_OP_REPLACE) 
			ci->setEnabled(match);
		else if 
			(op == GK_COMPOSITOR_OP_RESET) ci->setEnabled(false);
		else if 
			(match) ci->setEnabled(op == GK_COMPOSITOR_OP_ADD);
		
		if (!found && match) found = true;
	}

	if (op == GK_COMPOSITOR_OP_DEL || op == GK_COMPOSITOR_OP_RESET) 
		return true;

	if (!compositorName.empty() && !found) //create new compositor
	{ 
		if (compositorName == GK_COMPOSITOR_HEAT_VISION && !m_heatVisionInited) 
			m_heatVisionInited = gkOgreCompositorHelper::createHeatVisionCompositor();
		else if (compositorName == GK_COMPOSITOR_MOTION_BLUR && !m_motionBlurInited)
			m_motionBlurInited = gkOgreCompositorHelper::createMotionBlurCompositor();

		Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();

		Ogre::CompositorInstance* instance = compMgr.addCompositor(vp, compositorName, 0);
		if (instance) 
		{
			if (compositorName == GK_COMPOSITOR_HALFTONE && !m_halftonInited) 
				m_halftonInited = gkOgreCompositorHelper::createHalftoneTexture();
			else if (compositorName == GK_COMPOSITOR_DITHER && !m_ditherInited) 
				m_ditherInited = gkOgreCompositorHelper::createDitherTexture(width, height);			

			instance->setEnabled(true);

			gkPrintf("[COMP] add new compositor: %s", compositorName.c_str());

			return true;
		} 
		else 
		{
			gkPrintf("[COMP] %s - FAILED. check compositor name.", compositorName.c_str());
			return false;
		}
	}

	return false;
}
