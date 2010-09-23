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

#include "OgreKit.h"
#include "SceneLogic.h"

class OgreKit : public gkCoreApplication
{
public:

	OgreKit(const gkString& blend, const gkString& startCfg)
		: m_blend(gkUtils::getFile(blend)), m_sceneLogic(0)
	{
		gkPath path = gkUtils::getFile(startCfg);

		if (path.isFile())
		{
			m_prefs.load(path.getPath());
		}
		else
		{
			gkLogMessage("ERROR: FILE NOT FOUND ---> " << startCfg);
		}
	}

	~OgreKit()
	{
		delete m_sceneLogic;
	}

	void tick(gkScalar rate)
	{
		m_sceneLogic->tick(rate);
	}

	bool setup()
	{
		gkBlendFile* pBlendFile = m_engine->loadBlendFile(m_blend);

		if (pBlendFile)
		{
			gkScene* pScene = pBlendFile->getMainScene();

			GK_ASSERT(pScene);

			pScene->createInstance();

			GK_ASSERT(!m_sceneLogic);

			m_sceneLogic = new SceneLogic(pScene);

			return true;
		}
		else
		{
			gkLogMessage("ERROR: FILE NOT FOUND ---> " << m_blend);
			return false;
		}
	}

private:

	gkString m_blend;

	SceneLogic* m_sceneLogic;
};

int main(int argc, char** argv)
{
	// See ReadMe.txt for how to use this demo

	TestMemory;

	// This demo only works with momo_ogre_plus.blend file because it contains
	// (Ogre) material definitions and (Ogre) particle definitions inside blender's TX blocks.
	// Also logic is making reference to concrete objects inside momo_ogre_plus.blend.

	// If you want to avoid blender's TX blocks in order to make (Ogre) materials and particles
	// generic to all blender files then move them outside and load as standard Ogre resources...
	// (Use OgreKitResource.cfg)

	gkLogger::enable("LogicDemo.log", true);

	OgreKit okit("momo_ogre_plus.blend", "OgreKitStartup.cfg");

	okit.run();

	return 0;
}
