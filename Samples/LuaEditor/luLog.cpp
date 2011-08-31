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
#include "luDefs.h"
#include "luLog.h"
#include "luUtils.h"
#include "luMainFrame.h"

luLog::luLog(wxListBox *lb) 
	: m_logBox(lb) 
{
}

void luLog::log(const wxString &msg)
{
	if (!m_logBox) return;
	m_logBox->Select(m_logBox->Append(msg));
}

#if OGRE_VERSION >= 0x10800
void luLog::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName, bool& skipThisMessage)
#else
void luLog::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName)
#endif
{
	log(message.c_str());
}

void luLog::DoLogText(const wxString &msg) 
{		
	log(msg);
}

//--

luLogFile::luLogFile(const wxString& fileName) :
	m_fileName(fileName)
{
	reset();
}

void luLogFile::reset()
{
	m_modifiedTime = wxDateTime::Now();
	m_fileOffset = 0;
}

void luLogFile::watch(const wxString& fileName)
{
	m_fileName = fileName;
	reset();
}

bool luLogFile::isModified()
{
	if (!wxFileName::FileExists(m_fileName)) return false;
	
	wxDateTime time = wxFileName(m_fileName).GetModificationTime();
	return m_modifiedTime < time;
}

wxString luLogFile::readLog()
{			
	wxFile file;
	if (!file.Open(m_fileName, wxFile::read)) return "";

	wxDateTime time = wxFileName(m_fileName).GetModificationTime();

	wxString text;
	
	file.Seek(m_fileOffset);

	char buf[256+1];

	while (!file.Eof())
	{
		ssize_t len = file.Read(buf, sizeof(buf)-1);
		buf[len] = 0;

		text += buf;
	} 

	m_fileOffset = file.Tell();
	if (m_fileOffset != 0)
		m_modifiedTime = time;
	
	return text;
}

//--
