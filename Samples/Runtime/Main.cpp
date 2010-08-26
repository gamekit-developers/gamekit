/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor, xavier.thomas.1980.
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
#include "tclap/CmdLine.h"
#include "OgreKit.h"


const gkString gkDefaultBlend   = "momo_ogre.blend";
const gkString gkDefaultConfig  = "OgreKitStartup.cfg";



// ----------------------------------------------------------------------------
class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
public:
	gkString    m_blend;
	gkScene    *m_scene;
public:
	OgreKit();
	virtual ~OgreKit() {}

	int setup(int argc, char **argv);

	void keyReleased(const gkKeyboard &key, const gkScanCode &sc);

private:

	bool setup(void);
};


// ----------------------------------------------------------------------------
OgreKit::OgreKit()
	:   m_blend(gkDefaultBlend), m_scene(0)
{
}


// ----------------------------------------------------------------------------
int OgreKit::setup(int argc, char **argv)
{
	gkString cfgfname;

	// Parse command line
	try
	{
		TCLAP::CmdLine cmdl("Ogrekit", ' ', "n/a");
		cmdl.setExceptionHandling(false);

		TCLAP::ValueArg<std::string> cfgfname_arg("c", "config-file", "Startup configuration file (.cfg) to use.", false, gkDefaultConfig, "string");
		cmdl.add(cfgfname_arg);

		TCLAP::UnlabeledValueArg<std::string> bfname_arg("blender-file", "Blender file to launch as game.", false, gkDefaultBlend, "string");
		cmdl.add(bfname_arg);

		cmdl.parse( argc, argv );

		cfgfname = cfgfname_arg.getValue();
		m_blend = bfname_arg.getValue();


#ifdef __APPLE__
		if (m_blend.find("-psn") != gkString::npos)
			m_blend = gkDefaultBlend;
#endif

	}
	catch (TCLAP::ArgException &e)
	{
		std::cerr <<"error: " <<e.error() <<" for arg " <<e.argId() <<std::endl;
		return -1;
	}
	catch (TCLAP::ExitException &)
	{
		// just return and exit app
		return -1;
	}
	catch (...)
	{
		std::cerr <<"Unknown exception." <<std::endl;
		return -1;
	}


	m_prefs.winsize.x        = 800;
	m_prefs.winsize.y        = 600;
	m_prefs.wintitle         = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");

	gkPath path = cfgfname;

	// overide settings if found
	if (path.isFileInBundle())
		m_prefs.load(path.getPath());

	return 0;
}



// ----------------------------------------------------------------------------
bool OgreKit::setup(void)
{
	gkBlendFile *blend = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(m_blend), gkBlendLoader::LO_ALL_SCENES);
	if (!blend)
	{
		gkPrintf("File loading failed.\n");
		return false;
	}

	m_scene = blend->getMainScene();
	if (!m_scene)
	{
		gkPrintf("No usable scenes found in blend.\n");
		return false;
	}

	m_scene->initialize();

	// add input hooks
	gkWindowSystem::getSingleton().addListener(this);
	return true;
}


// ----------------------------------------------------------------------------
void OgreKit::keyReleased(const gkKeyboard &key, const gkScanCode &sc)
{
	if (sc == KC_ESCKEY)
		m_engine->requestExit();
}



// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{

	TestMemory;

	OgreKit okit;
	if (okit.setup(argc, argv) != 0)
	{
		// error
		return -1;
	}

	// Launch runtime
	okit.run();
	return 0;
}
