/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
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


#include "StdAfx.h"
#include "okApp.h"
#include "luMainApp.h"
#include "luMainFrame.h"
#include "luDefs.h"
#include "luConfig.h"
#include "luUtils.h"
#include "luFile.h"

#if defined(_MSC_VER) && defined(_DEBUG) //DEBUG:test memory leaks
#include <malloc.h>
#include <crtdbg.h>
#define _VC_DEBUG
#endif

IMPLEMENT_APP(luMainApp)

luMainApp::luMainApp() : 
	m_okApp(NULL),
	m_config(NULL),
	m_projTempList(NULL)
{ 
#ifdef _VC_DEBUG
 //_CrtSetBreakAlloc(142974); 
#endif
}

luMainApp::~luMainApp()
{
	SAFE_DELETE(m_config);
	SAFE_DELETE(m_projTempList);
}

void luMainApp::OnInitCmdLine(wxCmdLineParser&  parser)
{
	static const wxCmdLineEntryDesc cmdLineDesc[] =
	{
		{ wxCMD_LINE_OPTION, "f", "", "open the project file.", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

		{ wxCMD_LINE_NONE }
	};

	parser.SetDesc(cmdLineDesc);
	parser.SetSwitchChars("-");
}

bool luMainApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	wxString file;
	if (parser.Found("f", &file))
	{
		m_projFile = file;
		gkPrintf("args: %s", (const char*)file);
	}

	return true;
}



bool luMainApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

#if wxUSE_LIBPNG
   // wxImage::AddHandler( new wxPNGHandler ); //FIXME: crash with ogre freeimage library in win32		
#endif
	wxImage::AddHandler( new wxXPMHandler ); 

	m_config = new luConfig;
	if (!m_config->load(LU_CONFIG_FILE))
	{
		alertDlg(wxString::Format("Error - Can't load config file: %s", LU_CONFIG_FILE));
		return false;
	}

	m_projTempList = new luProjTempList;
	gkString templFile = m_config->getProjTemplateFile();
	if (!m_projTempList->load(templFile))
	{
		alertDlg(wxString::Format("Error - Can't load template file: %s",templFile.c_str()));
		return false;
	}


#if 0
	//-- setup htmlhelp
    //wxHelpControllerHelpProvider* provider = new wxHelpControllerHelpProvider;
	wxSimpleHelpProvider* provider = new wxSimpleHelpProvider;
    wxHelpProvider::Set(provider);
	wxFileSystem::AddHandler(new wxZipFSHandler);
	wxImage::AddHandler(new wxPNGHandler);
#endif

	m_okApp = new okApp;

    luMainFrame *frame = new luMainFrame();
	SetTopWindow(frame);

    return true;
}

int luMainApp::OnExit()
{
	SAFE_DELETE(m_okApp);	

    return wxApp::OnExit();
}
