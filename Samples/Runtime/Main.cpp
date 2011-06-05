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


class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
public:
	gkString    m_blend;
	gkScene*    m_scene;
public:
	OgreKit();
	virtual ~OgreKit() {}

	int setup(int argc, char** argv);

	void keyReleased(const gkKeyboard& key, const gkScanCode& sc);
private:

	bool setup(void);
};



OgreKit::OgreKit()
	:   m_blend(gkDefaultBlend), m_scene(0)
{
}


int OgreKit::setup(int argc, char** argv)
{
	int winsize_x		= 800;
	int winsize_y		= 600;
	m_prefs.wintitle	= gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");


	gkString cfgfname;

	// Parse command line
	try
	{
		TCLAP::CmdLine cmdl("Ogrekit", ' ', "n/a");
		cmdl.setExceptionHandling(false);

		//cfg arguments

		TCLAP::ValueArg<std::string>	rendersystem_arg		("r", "rendersystem",			"Set rendering system. (gl, d3d9, d3d10, d3d11)", false, "", "string"); //default GL
		TCLAP::ValueArg<std::string>	viewportOrientation_arg	("",  "viewportorientation",	"Set viewport orientation.", false, m_prefs.viewportOrientation, "string"); 
		TCLAP::ValueArg<std::string>	log_arg					("",  "log",					"Set log file name.", false, m_prefs.log, "string"); 
		TCLAP::ValueArg<bool>			verbose_arg				("v", "verbose",				"Enable verbose log.", false, m_prefs.verbose, "bool");
		TCLAP::ValueArg<int>			winsize_x_arg			("",  "width",					"Set window width.", false, winsize_x, "int");
		TCLAP::ValueArg<int>			winsize_y_arg			("",  "height",					"Set window height.", false, winsize_y, "int");
		TCLAP::ValueArg<std::string>	wintitle_arg			("",  "wintitle",				"Set window title.", false, m_prefs.wintitle, "string"); 
		TCLAP::ValueArg<bool>			fullscreen_arg			("f", "fullscreen",				"Enable fullscreen mode.", false, m_prefs.fullscreen, "bool");
		TCLAP::ValueArg<std::string>	framingType_arg			("",  "framingtype",			"Set viewport framing type. (extend, crop, letterbox)", false, "", "string");
		TCLAP::ValueArg<std::string>	resources_arg			("",  "resources",				"Set resouces.", false, m_prefs.resources, "string");
		TCLAP::ValueArg<bool>			blendermat_arg			("",  "blendmat",				"Convert meshes using blender materials.", false, m_prefs.blendermat, "bool");
		TCLAP::ValueArg<bool>			matblending_arg			("",  "matblending",			"Enable material pass blending mode.", false, m_prefs.matblending, "bool");		
		TCLAP::ValueArg<bool>			grapInput_arg			("g", "grabinput",				"Grap mouse input.", false, m_prefs.grabInput, "bool");
		TCLAP::ValueArg<bool>			debugFps_arg			("d", "debugfps",				"Display debug fps.", false, m_prefs.debugFps, "bool");
		TCLAP::ValueArg<bool>			debugPhysics_arg		("p", "debugphysics",			"Display debug physics.", false, m_prefs.debugPhysics, "bool");
		TCLAP::ValueArg<bool>			debugPhysicsAabb_arg	("a", "debugphysicsaabb",		"Display debug physics aabb.", false, m_prefs.debugPhysicsAabb, "bool");
		TCLAP::ValueArg<bool>			buildStaticGeometry_arg	("",  "buildinstances",			"Build Static Geometry.", false, m_prefs.buildStaticGeometry, "bool");
		TCLAP::ValueArg<bool>			useBulletDbvt_arg		("",  "frustumculling",			"Enable view frustum culling by dbvt.", false, m_prefs.useBulletDbvt, "bool");
		TCLAP::ValueArg<bool>			showDebugProps_arg		("t", "showdebugprops",			"Show debug props.", false, m_prefs.showDebugProps, "bool");
		TCLAP::ValueArg<bool>			debugSounds_arg			("",  "debugsounds",			"Debug sounds.", false, m_prefs.debugSounds, "bool");
		TCLAP::ValueArg<bool>			disableSound_arg		("s", "disablesound",			"Disable sounds.", false, m_prefs.disableSound, "bool");
		TCLAP::ValueArg<bool>			fsaa_arg				("",  "fsaa",					"Enable fsaa.", false, m_prefs.fsaa, "bool");
		TCLAP::ValueArg<int>			fsaaSamples_arg			("",  "fsaasSamples",			"Set fsaa samples.", false, m_prefs.fsaaSamples, "int");
		TCLAP::ValueArg<bool>			enableshadows_arg		("",  "enableshadows",			"Enable Shadows.", false, m_prefs.enableshadows, "bool");
		TCLAP::ValueArg<int>			defaultMipMap_arg		("",  "defaultmipmap",			"Set default mipMap.", false, m_prefs.defaultMipMap, "int");
		TCLAP::ValueArg<std::string>	shadowtechnique_arg		("",  "shadowtechnique",		"Set shadow technique.", false, m_prefs.shadowtechnique, "string"); 
		TCLAP::ValueArg<std::string>	colourshadow_arg		("",  "colourshadow",			"Set shadow colour.", false, "", "string"); 
		TCLAP::ValueArg<float>			fardistanceshadow_arg	("",  "fardistanceshadow",		"Set far distance shadow.", false, m_prefs.fardistanceshadow, "float"); 
		TCLAP::ValueArg<std::string>	shaderCachePath_arg		("",  "shadercachepath",		"RTShaderSystem cache file path.", false, m_prefs.shaderCachePath, "string"); 
		

		cmdl.add(rendersystem_arg);
		cmdl.add(viewportOrientation_arg);
		cmdl.add(log_arg);
		cmdl.add(verbose_arg);
		cmdl.add(winsize_x_arg);
		cmdl.add(winsize_y_arg);
		cmdl.add(wintitle_arg);
		cmdl.add(fullscreen_arg);
		cmdl.add(framingType_arg);
		cmdl.add(resources_arg);
		cmdl.add(blendermat_arg);
		cmdl.add(matblending_arg);
		cmdl.add(grapInput_arg);
		cmdl.add(debugFps_arg);
		cmdl.add(debugPhysics_arg);	
		cmdl.add(debugPhysicsAabb_arg);	
		cmdl.add(buildStaticGeometry_arg);
		cmdl.add(useBulletDbvt_arg);
		cmdl.add(showDebugProps_arg);
		cmdl.add(debugSounds_arg);
		cmdl.add(disableSound_arg);
		cmdl.add(fsaa_arg);
		cmdl.add(fsaaSamples_arg);
		cmdl.add(enableshadows_arg);
		cmdl.add(defaultMipMap_arg);
		cmdl.add(shadowtechnique_arg);
		cmdl.add(colourshadow_arg);
		cmdl.add(fardistanceshadow_arg);
		cmdl.add(shaderCachePath_arg);

		//input file arguments
		
		TCLAP::ValueArg<std::string>			cfgfname_arg("c", "config-file", "Startup configuration file (.cfg) to use.", false, gkDefaultConfig, "string");
		TCLAP::UnlabeledValueArg<std::string>	bfname_arg("blender-file", "Blender file to launch as game.", false, gkDefaultBlend, "string");

		cmdl.add(cfgfname_arg);
		cmdl.add(bfname_arg);

		cmdl.parse( argc, argv );

		cfgfname						= cfgfname_arg.getValue();
		m_blend							= bfname_arg.getValue();

		m_prefs.rendersystem			= gkUserDefs::getOgreRenderSystem(rendersystem_arg.getValue());
		m_prefs.viewportOrientation		= viewportOrientation_arg.getValue();
		//m_prefs.sceneManager			= sceneManager_arg.getValue();
		m_prefs.log						= log_arg.getValue();
		m_prefs.verbose					= verbose_arg.getValue();

		m_prefs.winsize					= gkVector2(winsize_x_arg.getValue(), winsize_y_arg.getValue());
		m_prefs.wintitle				= wintitle_arg.getValue();

		m_prefs.fullscreen				= fullscreen_arg.getValue();
		m_prefs.framingType				= gkUserDefs::getViewportFramingType(framingType_arg.getValue());
		m_prefs.resources				= resources_arg.getValue();
		m_prefs.blendermat				= blendermat_arg.getValue();
		m_prefs.matblending				= matblending_arg.getValue();
		m_prefs.grabInput				= grapInput_arg.getValue();
		m_prefs.debugFps				= debugFps_arg.getValue();
		m_prefs.debugPhysics			= debugPhysics_arg.getValue();
		m_prefs.debugPhysicsAabb		= debugPhysicsAabb_arg.getValue();
		m_prefs.buildStaticGeometry		= buildStaticGeometry_arg.getValue();
		m_prefs.useBulletDbvt			= useBulletDbvt_arg.getValue();
		m_prefs.showDebugProps			= showDebugProps_arg.getValue();
		m_prefs.debugSounds				= debugSounds_arg.getValue();
		m_prefs.disableSound			= disableSound_arg.getValue();

		m_prefs.fsaa					= fsaa_arg.getValue();
		m_prefs.fsaaSamples				= fsaaSamples_arg.getValue();
		m_prefs.enableshadows			= enableshadows_arg.getValue();
		m_prefs.defaultMipMap			= defaultMipMap_arg.getValue();
		m_prefs.shadowtechnique			= shadowtechnique_arg.getValue();
		m_prefs.fardistanceshadow		= fardistanceshadow_arg.getValue();	
		m_prefs.shaderCachePath			= shaderCachePath_arg.getValue();

		if (colourshadow_arg.isSet())
			m_prefs.colourshadow		= Ogre::StringConverter::parseColourValue(colourshadow_arg.getValue());

#ifdef __APPLE__
		if (m_blend.find("-psn") != gkString::npos)
			m_blend = gkDefaultBlend;
#endif

	}
	catch (TCLAP::ArgException& e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return -1;
	}
	catch (TCLAP::ExitException&)
	{
		// just return and exit app
		return -1;
	}
	catch (...)
	{
		std::cerr << "Unknown exception." << std::endl;
		return -1;
	}



	gkPath path = cfgfname;

	// overide settings if found
	if (path.isFile())
		m_prefs.load(path.getPath());

	return 0;
}


bool OgreKit::setup(void)
{
	gkBlendFile* blend = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(m_blend), gkBlendLoader::LO_ALL_SCENES);
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


	m_scene->createInstance();

	// add input hooks
	gkWindowSystem::getSingleton().addListener(this);

#ifdef OGREKIT_USE_COMPOSITOR
	//gkCompositorManager::getSingleton().setCompositorChain(GK_COMPOSITOR_OP_ADD, GK_COMPOSITOR_BLOOM);
#endif

	return true;
}



void OgreKit::keyReleased(const gkKeyboard& key, const gkScanCode& sc)
{
	if (sc == KC_ESCKEY)
		m_engine->requestExit();
}

int main(int argc, char** argv)
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
