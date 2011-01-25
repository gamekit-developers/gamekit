/*
-------------------------------------------------------------------------------
    This file is part of GuiDemo.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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

#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include "rocket/RocketRenderListener.h"
#include "rocket/RenderInterfaceOgre3D.h"
#include "rocket/RocketEventListener.h"
#include "rocket/SystemInterfaceOgre3D.h"
#include "rocket/FileInterfaceOgre3D.h"

#define GUIDEMO_GROUP_NAME	"Rocket"
#define DEMO_BLEND_FILE		"logo_text.blend"
#define ASSETS_BLEND_FILE	"assets.blend"
#define ROCKET_CURSOR_PAGE	"cursor.rml"
#define ROCKET_DEMO_PAGE	"demo.rml"
#define DEMO_PAGE_BUTTON_ID "submit1"
#define DEMO_PAGE_INPUT_ID	"input1"

void installRocketFonts();	//in uifonts.cpp

class GuiDemo : 
	public gkCoreApplication, 
	public gkWindowSystem::Listener,
	public Rocket::Core::EventListener
{
protected:
	gkString    m_blend;
	gkScene*    m_scene;

	RocketRenderListener*	m_rkRenderListener;
	RocketEventListener*	m_rkEventListener;	

	Rocket::Core::Context*	m_rkContext;

	FileInterfaceOgre3D*	m_rkFileInterface;
	SystemInterfaceOgre3D*	m_rkOgreSystem;
	RenderInterfaceOgre3D*	m_rkOgreRenderer;

	Rocket::Core::ElementDocument* m_document;

	void initRocket();
	void uninitRocket();

public:
	GuiDemo();
	virtual ~GuiDemo();

	int setup(int argc, char** argv);

	virtual void keyReleased(const gkKeyboard& key, const gkScanCode& sc);

	virtual void ProcessEvent(Rocket::Core::Event& event);

private:

	virtual bool setup(void);
};



GuiDemo::GuiDemo()
	:	m_blend(DEMO_BLEND_FILE), 
		m_scene(0), 
		m_rkContext(0),
		m_rkFileInterface(0),
		m_rkOgreRenderer(0),
		m_rkRenderListener(0), 
		m_rkEventListener(0),
		m_document(0)
{
}


GuiDemo::~GuiDemo()
{
}

int GuiDemo::setup(int argc, char** argv)
{
	int winsize_x		= 800;
	int winsize_y		= 600;
	m_prefs.wintitle	= gkString("OgreKit GuiDemo (Press Escape to exit)[") + m_blend + gkString("]");
	//m_prefs.grabInput = false;

	return 0;
}



bool GuiDemo::setup(void)
{
	gkBlendFile* blend = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(m_blend), "", GUIDEMO_GROUP_NAME);
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

	gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(ASSETS_BLEND_FILE), "", GUIDEMO_GROUP_NAME);

	//--

	initRocket();


	return true;
}


void GuiDemo::initRocket()
{
	GK_ASSERT(m_scene && !m_rkContext);

	gkWindow* window = m_scene->getDisplayWindow();
	// Rocket initialisation.
	m_rkOgreRenderer = new RenderInterfaceOgre3D(window->getWidth(), window->getHeight());
	Rocket::Core::SetRenderInterface(m_rkOgreRenderer);

	m_rkOgreSystem = new SystemInterfaceOgre3D();
	Rocket::Core::SetSystemInterface(m_rkOgreSystem);

	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();


	installRocketFonts();

	m_rkContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(window->getWidth(), window->getHeight()));
	Rocket::Debugger::Initialise(m_rkContext);

	m_rkFileInterface = new FileInterfaceOgre3D();
	Rocket::Core::SetFileInterface(m_rkFileInterface);


	// Load the mouse cursor and release the caller's reference.
	Rocket::Core::ElementDocument* cursor = m_rkContext->LoadMouseCursor(ROCKET_CURSOR_PAGE);
	if (cursor)
		cursor->RemoveReference();

	m_document = m_rkContext->LoadDocument(ROCKET_DEMO_PAGE);
	if (m_document)
	{
		Rocket::Core::Element* button = m_document->GetElementById(DEMO_PAGE_BUTTON_ID);
		if (button)
			button->AddEventListener("click", this);
		m_document->Show();		
	}

	m_rkEventListener  = new RocketEventListener(window, m_rkContext);
	m_rkRenderListener = new RocketRenderListener(window->getRenderWindow(), m_scene->getManager(), m_rkContext);
}

void GuiDemo::uninitRocket()
{
	if (m_document) m_document->RemoveReference();
	m_document = 0;

	// Shutdown Rocket.
	if (m_rkContext) m_rkContext->RemoveReference();
	m_rkContext = 0;
	Rocket::Core::Shutdown();

	delete m_rkOgreSystem; m_rkOgreSystem = 0;
	delete m_rkOgreRenderer; m_rkOgreRenderer = 0;
	delete m_rkFileInterface; m_rkFileInterface = 0;

	delete m_rkEventListener; m_rkEventListener = 0;
	delete m_rkRenderListener; m_rkRenderListener = 0;
}

void GuiDemo::ProcessEvent(Rocket::Core::Event& event)
{
	GK_ASSERT(m_document);
	Rocket::Core::Element* elm = m_document->GetElementById(DEMO_PAGE_INPUT_ID);
	if (elm)
	{
		printf("button clicked: %s\n", static_cast<Rocket::Controls::ElementFormControl*>(elm)->GetValue().CString());
	}
	//printf("Processing event %s", event.GetType().CString());
}



void GuiDemo::keyReleased(const gkKeyboard& key, const gkScanCode& sc)
{
	if (sc == KC_ESCKEY)
	{
		uninitRocket();
		m_engine->requestExit();
	}
}


int main(int argc, char** argv)
{
	GuiDemo okit;
	if (okit.setup(argc, argv) != 0)
	{
		// error
		return -1;
	}

	// Launch runtime
	okit.run();
	return 0;
}
