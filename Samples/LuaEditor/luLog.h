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
#ifndef _luLog_h_
#define _luLog_h_


class luLog : public wxLog, public Ogre::LogListener
{
	wxListBox *m_logBox;
public:
	luLog(wxListBox *lb);

	void log(const wxString &msg);

#if OGRE_VERSION >= 0x10800
	virtual void messageLogged (const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName, bool& skipThisMessage);
#else
	virtual void messageLogged (const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName);
#endif
	virtual void DoLogText(const wxString &msg); //wxlog
};

class luLogFile
{
	wxString m_fileName;
	wxDateTime m_modifiedTime;
	wxFileOffset  m_fileOffset;
public:
	luLogFile(const wxString& fileName = "");

	wxString getFileName() { return m_fileName; }
	void watch(const wxString& fileName);
	void reset();

	bool isModified();
	wxString readLog();
};

#endif //_luLog_h_
