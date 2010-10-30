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

#include <wx/filename.h>
#include <wx/textfile.h>

#include "luFile.h"
#include "Lib/liLuaUtils.h"
#include "Lib/liUtils.h"
#include "Lib/liXmlConfig.h"

#define PROJECT_VER				"Version"
#define PROJECT_NAME			"ProjectName"
#define PROJECT_START_LUA_FILE	"StartLuaFile"
#define PROJECT_FILES			"Files"

//--

gkString luFile::getCwd()
{
	return (const char*)wxFileName::GetCwd();
}

bool luFile::setCwd(const gkString& dirName)
{
	return wxFileName::SetCwd(dirName);
}

bool luFile::isRelativePath(const gkString& path)
{
	return wxFileName(path).IsRelative();
}

bool luFile::isAbsolutePath(const gkString& path)
{
	return wxFileName(path).IsAbsolute();
}

bool luFile::isFileExist(const gkString& path)
{
	return wxFile::Exists(path);
}

gkString luFile::getDirName(const gkString& fileName)
{
	wxFileName fn(fileName);
	return (const char*)fn.GetPath();
}

gkString luFile::getFileNameBase(const gkString& fileName)
{
	wxFileName fn(fileName);
	return (const char*)fn.GetName();
}

gkString luFile::getRelativePath(const gkString& fileName, const gkString& dirName)
{
	wxFileName fn(fileName);
	if (!fn.IsRelative())
		fn.MakeRelativeTo(dirName);

	return (const char*)fn.GetFullPath();
}

gkString luFile::getAbsolutePath(const gkString& fileName, const gkString& dirName)
{
	wxFileName fn(fileName);
	if (fn.IsRelative())
		fn.MakeAbsolute(dirName);

	return (const char*)fn.GetFullPath();
}

bool luFile::isSameFile(const gkString& file1,const gkString& file2)
{
	return wxFileName(file1.c_str()).SameAs(wxFileName(file2));
}


gkString luFile::convPathStyle(const gkString& fileName, bool unixStyle)
{
	return unixStyle ? strReplace(fileName, "\\", "/") : strReplace(fileName, "/", "\\");
}

//--

bool luHhkFile::load(const gkString& fileName)
{
	wxTextFile file;
	if (!file.Open(fileName))
		return false;

	//<LI><OBJECT type="text/sitemap"><param name="Local" value="_about.html"><param name="Name" value="About"></OBJECT>	
	for (size_t i = 0; i < file.GetLineCount(); i++)
	{
		wxString line = file.GetLine(i);
		if (line.Find("text/sitemap") == wxNOT_FOUND) continue;

		liStrVec values;

		size_t sp = 0;
		for (int k = 0; k < 2; k++)
		{
			size_t s = line.find("value=", sp);
			if (s == wxString::npos) continue;

			size_t t = line.find('\"', s+6);
			if (t == wxString::npos) continue;

			size_t u = line.find('\"', t+1);
			if (u == wxString::npos) continue;

			values.push_back((const char*)line.SubString(t+1, u-1));
			sp = u + 1;
		}

		if (values.size() != 2) continue;

		m_siteMap[values[1]] = values[0];
	}

	return true;
}

gkString luHhkFile::getHtmlFile(const gkString& keyword)
{
	return m_siteMap.find(keyword) != m_siteMap.end() ?
		m_siteMap[keyword] : "";
}

//--

luProjectFile::luProjectFile()
	: m_projVer(VERSION), m_flags(F_NORMAL)
{
}

luProjectFile::~luProjectFile()
{

}

bool luProjectFile::create(const gkString& fileName, const gkString& projName)
{
	m_projFileName = luFile::getAbsolutePath(fileName);
	m_projDir = luFile::getDirName(m_projFileName);
	

	if (projName.empty())
		m_projName = luFile::getFileNameBase(fileName);
	else
		m_projName = projName;

	m_projVer = VERSION;
	m_startLuaFile = "";
	m_files.clear();	

	setModified();

	return true;
}

/* Sample.okproj
Version = 1
ProjectName = "MyFirstProject"
StartLuaFile = "Test0.lua"
Files = {
	"Test0.lua",
	"Test0.blend"
}
*/
bool luProjectFile::save(const gkString& fileName)
{
	gkString fn = fileName;
	if (fn.empty())
		fn = m_projFileName;

	FILE *fp = fopen(fn.c_str(), "wt");
	if (!fp) return false;

	fprintf(fp, "%s = %d\n",		PROJECT_VER,			m_projVer);
	fprintf(fp, "%s = \"%s\"\n",	PROJECT_NAME,			m_projName.c_str());
	fprintf(fp, "%s = \"%s\"\n",	PROJECT_START_LUA_FILE,	m_startLuaFile.c_str());
	
	fprintf(fp, "%s = {\n",	PROJECT_FILES);
	for (size_t i = 0; i < m_files.size(); i++) 
	{
		fprintf(fp, "\t\"%s\"%s\n", 
			strReplace(m_files[i].c_str(), "\\", "/").c_str(), 
			(i == m_files.size()-1) ? "" : ","
		);
	}
	fprintf(fp, "}\n");

	fclose(fp); fp = NULL;

	if (m_startLuaFile.empty())
		gkPrintf("Warning - Start Lua File is empty.");

	setModified(false);

	return true;
}

bool luProjectFile::loadV1(liLuaScript& script)
{
	m_projName = script.getGlobalStr(PROJECT_NAME);
	m_startLuaFile = script.getGlobalStr(PROJECT_START_LUA_FILE);
	gkString table = PROJECT_FILES;
	
	int size = script.getTableSize(table);
	for (int i = 1; i <= size; i++)
		m_files.push_back(script.getTableStr(table, i));

	return true;
}

bool luProjectFile::load(const gkString& fileName)
{
	liLuaScript script;
	if (!script.call(fileName))
		return false;

	m_projFileName = luFile::getAbsolutePath(fileName);
	m_projDir = luFile::getDirName(m_projFileName);

	gkString ver = script.getGlobalStr(PROJECT_VER);
	m_projVer = atoi(ver.c_str());

	bool ok = false;
	if (m_projVer == 1)
		ok = loadV1(script);
	else
		gkPrintf("Error - Unknown project file: version=%d", m_projVer);

	setModified(false);
	
	return ok;
}

gkString luProjectFile::getFile(size_t i, bool relativePath) 
{ 
	if (i >= m_files.size()) return ""; 

	return relativePath ? m_files[i] : luFile::getAbsolutePath(m_files[i], m_projDir);
} 


int luProjectFile::getFileIndex(const gkString& fileName)
{	
	for (size_t i = 0; i < m_files.size(); i++)
		if (luFile::isSameFile(m_files[i], fileName))
			return (int)i;

	return -1;
}

bool luProjectFile::hasFile(const gkString& fileName) 
{ 
	gkString fn = getRelativePath(fileName);
	return getFileIndex(fn) >= 0; 
}

bool luProjectFile::setStartLuaFile(const gkString& fileName)
{
	gkString fn = getRelativePath(fileName);
	if (getFileIndex(fn) < 0) return false;
	if (luFile::isSameFile(m_startLuaFile, fileName))
		return true;
	
	m_startLuaFile = fn;
	setModified();

	return true;
}


bool luProjectFile::addFile(const gkString& fileName)
{
	gkString fn = getRelativePath(fileName);
	if (getFileIndex(fn) >= 0) return false;

	m_files.push_back(fn);

	setModified();

	return true;
}

bool luProjectFile::removeFile(const gkString& fileName)
{
	gkString fn = getRelativePath(fileName);
	int i = getFileIndex(fn);
	if (i < 0) return false;

	if (luFile::isSameFile(fn, m_startLuaFile))
		m_startLuaFile = "";

	m_files.erase(i);

	setModified();
	return true;
}

//--

bool luProjTempList::load(const gkString& fileName)
{
	using namespace xml;
	xmlConfig config;
	if (!config.load(fileName))
		return false;

	liStrVec names = config.getStrVec("template.info.names");

	luProjTemplate templ;

	for (size_t i = 0; i < names.size(); i++)
	{
		gkString gkey = gkString("template.") + names[i];

		templ.name = names[i];
		templ.desc = config.getString(gkey + ".desc");
		templ.preview = config.getString(gkey + ".preview");
		templ.title = config.getString(gkey + ".title");
		templ.files = config.getStrVec(gkey + ".files");
		templ.startLuaFile = config.getString(gkey + ".start_lua");

		if (templ.name.empty())
			continue;

		push_back(templ);
	}

	return true;
}
