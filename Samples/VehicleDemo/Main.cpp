/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier Thomas.

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
#include "gkLogic.h"

class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
public:

	OgreKit(const gkString& blend, const gkString& startCfg)
		: m_blend(gkUtils::getFile(blend)), m_logic(0)
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
		delete m_logic;
	}


	void tick(gkScalar rate)
	{
		m_logic->tick(rate);
	}

	bool setup()
	{
		gkBlendFile* pBlendFile = gkBlendLoader::getSingleton().loadFile(m_blend, "", VEHICLE_RESOURCE_GROUP);

		if (pBlendFile)
		{
			gkScene* pScene = pBlendFile->getMainScene();

			GK_ASSERT(pScene);

			pScene->createInstance();

			m_logic = new gkLogic(pScene);


			// add input hooks
			gkWindowSystem::getSingleton().addListener(this);
			return true;
		}
		else
		{
			gkLogMessage("ERROR: FILE NOT FOUND ---> " << m_blend);
			return false;
		}
	}

	void keyPressed(const gkKeyboard& key, const gkScanCode& sc)
	{
	}

	void keyReleased(const gkKeyboard& key, const gkScanCode& sc)
	{
		if (sc == KC_ESCKEY)
			m_engine->requestExit();
	}

private:

	gkString  m_blend;
	gkLogic*   m_logic;
};


int main(int argc, char** argv)
{
	// See ReadMe.txt for how to use this demo

	TestMemory;

	gkLogger::enable("VehicleDemo.log", true);

	// Do  not used the original clubsilo_packed but a cleaned version for better fps and no objects in the way.
//	OgreKit okit("../Runtime/Regression/clubsilo_packed.blend", "OgreKitStartup.cfg");
	OgreKit okit("clubsilo_demo.blend", "OgreKitStartup.cfg");

	okit.run();

	return 0;
}
