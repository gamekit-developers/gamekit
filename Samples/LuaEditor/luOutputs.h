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

#ifndef _luOutputs_h_
#define _luOutputs_h_

class luLogBox : public wxListBox
{
public:
	luLogBox(wxWindow* parent, int id = wxID_ANY);

	bool save(const wxString& fileName);
};

class luLogEdit : public wxTextCtrl
{
	void OnLButtonDBClick(wxMouseEvent& event);
public:
	luLogEdit(wxWindow* parent, int id = wxID_ANY);

	DECLARE_EVENT_TABLE()
};

class liLuaScript;

class luConsoleEdit : public wxTextCtrl, public utSingleton<luConsoleEdit>
{
	liLuaScript* m_script;
	wxString m_prompt;

	wxArrayString m_commands;
	int m_lastCmdIndex;
	void addCommand(const wxString& cmd);
	void pasteCommand(int move);
	void replaceCommand(const wxString& cmd);

	long getLastPromptPos();

	void OnKeyDown(wxKeyEvent& event);

	static void consoleLog(const char *log);
public:
	luConsoleEdit(wxWindow* parent, int id = wxID_ANY);
	~luConsoleEdit();
	
	void writeLine(const wxString& line, bool newLine=false);
	void runCmd(const wxString& cmd, bool prompt=true, bool echo=true);

	DECLARE_EVENT_TABLE()

	UT_DECLARE_SINGLETON(luConsoleEdit)
};


#endif//_luOutputs_h_