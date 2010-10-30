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


#ifndef _luMainApp_h_
#define _luMainApp_h_

class okApp;
class luConfig;
class luProjTempList;

class luMainApp : public wxApp
{
	okApp*   m_okApp;
	wxString m_projFile;
	
	luConfig* m_config;
	luProjTempList* m_projTempList;

public:
    luMainApp();
	~luMainApp();

    virtual bool OnInit();
    virtual int  OnExit();

	virtual void OnInitCmdLine(wxCmdLineParser&  parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	okApp*   getOkApp() { return m_okApp; }
	wxString getProjFileToOpen() const { return m_projFile; }
	luProjTempList* getProjTempList() { return m_projTempList; }

	virtual void OnFatalException()
	{
		Exit();
	}

	virtual bool OnExceptionInMainLoop()  
	{
		return false;
	}
};

#endif //_luMainApp_h_