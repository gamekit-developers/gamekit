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
#include "luUtils.h"
#include "luMainFrame.h"
#include "luOutputs.h"
#include "Lib/liUtils.h"
#include "Lib/liLuaUtils.h"
#include "okUtils.h"

#define MAX_CMD_SIZE 20

luLogBox::luLogBox(wxWindow* parent, int id) :
	wxListBox(parent, id, wxDefaultPosition, wxDefaultSize)
{

}

bool luLogBox::save(const wxString& fileName)
{
	 wxTextFile file(fileName);
		 
	bool ok = wxFile::Exists(fileName) ? file.Open() : file.Create();

	if (ok)
	{
		file.Clear();
		wxArrayString lines = GetStrings();
		for (size_t i = 0; i < lines.GetCount(); i++)
		{
			file.AddLine(lines[i]);
		}
		ok = file.Write();
		wxLogMessage("Log file is saved: %s", fileName);
	}
	else
	{
		wxLogMessage("Can't open log file: %s", fileName);
	}

	return ok;
}


//--

BEGIN_EVENT_TABLE(luLogEdit, wxTextCtrl)
	EVT_LEFT_DCLICK (luLogEdit::OnLButtonDBClick)
END_EVENT_TABLE()

luLogEdit::luLogEdit(wxWindow* parent, int id) :
	wxTextCtrl(parent, id, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)
{

}


void luLogEdit::OnLButtonDBClick(wxMouseEvent& event)
{
	long pos = GetInsertionPoint();
	long x = 0, y = 0;
	PositionToXY(pos, &x, &y);
	wxString line = GetLineText(y);

	//01:15:55: TestError.lua:49: attempt to index field 'object' (a nil value)
	size_t i = wxString(line).MakeLower().find(".lua:");
	if (i != wxString::npos)
	{
		size_t k = line.rfind(':', i);
		size_t p = i+4;
		size_t s = line.find(':', p+1);

		if (k != wxString::npos && p != wxString::npos && s != wxString::npos)
		{
			wxString luaFile = line.SubString(k+1, p-1).Trim().Trim(false);
			wxString lineNo = line.SubString(p+1, s-1).Trim().Trim(false);
			long line = 0;

			if (lineNo.ToLong(&line))
			{
				luMainFrame* frame = getLuMainFrame();
				if (frame) frame->gotoLuaSource(luaFile, line);
			}
		}
	}
	

	event.Skip();
}

//--

UT_IMPLEMENT_SINGLETON(luConsoleEdit)


BEGIN_EVENT_TABLE(luConsoleEdit, wxTextCtrl)
	EVT_KEY_DOWN(luConsoleEdit::OnKeyDown)
END_EVENT_TABLE()

void luConsoleEdit::consoleLog(const char *log)
{
	if (!luConsoleEdit::getSingletonPtr() || !log) return;

	luConsoleEdit::getSingleton().writeLine(log, true);
}

luConsoleEdit::luConsoleEdit(wxWindow* parent, int id) :
	wxTextCtrl(parent, id, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE),
	m_lastCmdIndex(-1),
	m_prompt(wxT("> ")),
	m_script(NULL)
{
	m_script = new liLuaScript(true, consoleLog);
	writeLine(m_prompt);	
}

luConsoleEdit::~luConsoleEdit()
{
	SAFE_DELETE(m_script);
}

void luConsoleEdit::addCommand(const wxString& cmd)
{
	if (cmd.IsEmpty()) return;

	if (m_lastCmdIndex >= 0 && m_lastCmdIndex < (int)m_commands.GetCount() &&
		m_commands[m_lastCmdIndex] == cmd)
		m_commands.RemoveAt(m_lastCmdIndex);


	if (m_commands.IsEmpty() || !m_commands[m_commands.GetCount()-1] != cmd)
		m_commands.Add(cmd);

	if (m_commands.GetCount() > MAX_CMD_SIZE)
		m_commands.RemoveAt(0);

	m_lastCmdIndex = -1; //reset

}

void luConsoleEdit::pasteCommand(int move)
{
	if (move == 0) 
	{
		m_lastCmdIndex = -1;
		return;
	}

	int last = m_commands.GetCount() - 1;

	if (last < 0) return;

	if (move < 0)	//up
	{ 
		if (m_lastCmdIndex == 0) 
			return;
		 else if (m_lastCmdIndex < 0) 
			m_lastCmdIndex = last;
		 else 
			m_lastCmdIndex--;
		
	} 
	else			//down
	{ 
		if (m_lastCmdIndex == last) 
			return;
		else if (m_lastCmdIndex < 0)
			return;
		else
			m_lastCmdIndex++;		
	}

	replaceCommand(m_commands[m_lastCmdIndex]);
}

long luConsoleEdit::getLastPromptPos()
{
	long x = 0, y = 0;
	long last = GetLastPosition();
	if (!PositionToXY(last, &x, &y))
		return 0;

	return XYToPosition(0, y);
}

void luConsoleEdit::replaceCommand(const wxString& cmd)
{
	long pos = getLastPromptPos();
	Replace(pos + m_prompt.Length(), GetLastPosition(), cmd);
}

void luConsoleEdit::writeLine(const wxString& line, bool newLine)
{
	AppendText(line);
	//long pos = GetLastPosition();
	//Replace(pos, pos, line);
	if (newLine) AppendText("\n");
}

void luConsoleEdit::runCmd(const wxString& cmd, bool prompt, bool echo)
{
	if (echo) writeLine(cmd + "\n");
	if (prompt) writeLine(m_prompt);

	if (m_script)
	{
		m_script->call(WX2GK(cmd), "console");
	}
}

void luConsoleEdit::OnKeyDown(wxKeyEvent& event)
{
	int ch = event.GetKeyCode();
	wxString str;
	
	if (ch == WXK_UP) 
	{
		pasteCommand(-1);
		return;
	} 
	else if (ch == WXK_DOWN) 
	{
		pasteCommand(+1);
		return;
	} 
	else if (ch == WXK_ESCAPE) 
	{
		pasteCommand(0);
		replaceCommand("");
		return;
	} 
	else if (ch == WXK_LEFT || ch == WXK_BACK) 
	{
		long x = 0, y = 0;
		if (PositionToXY(GetInsertionPoint(), &x, &y))
		{
			if (x <= (int)m_prompt.Length()) 
				return;
		}		
	} 
	else if (ch == WXK_HOME) 
	{
		long x = 0, y = 0;
		if (PositionToXY(GetInsertionPoint(), &x, &y))
		{
			x = m_prompt.Length();
			long pos = XYToPosition(x, y);
			SetSelection(pos, pos);
		}

		return;
	} 
	
	
	if (ch == WXK_RETURN) 
	{
		str = GetLineText(GetNumberOfLines() - 1);
		str.Replace(m_prompt, "");
	}
	else
	{
		event.Skip();
	}

	if (ch == WXK_RETURN) 
	{
		writeLine("\n");

		str.Trim().Trim(false);

		if (!str.IsEmpty()) 
		{
			runCmd(str, false, false);
			addCommand(str);
		} 
				
		writeLine(m_prompt);
		pasteCommand(0);
	}
}

