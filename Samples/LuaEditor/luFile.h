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

#ifndef _luFile_h_
#define _luFile_h_

#include "Lib/liTypes.h"
class liLuaScript;

class luFile 
{
public:
	static gkString getCwd();
	static bool setCwd(const gkString& dirName);
	static gkString getDirName(const gkString& fileName);
	static gkString getFileNameBase(const gkString& fileName);
	static gkString getRelativePath(const gkString& fileName, const gkString& dirName = "");
	static gkString getAbsolutePath(const gkString& fileName, const gkString& dirName = "");
	static bool isSameFile(const gkString& file1, const gkString& file2);
	static bool isRelativePath(const gkString& path);
	static bool isAbsolutePath(const gkString& path);
	static bool isFileExist(const gkString& path);
	static gkString convPathStyle(const gkString& fileName, bool unixStyle=true);	
};

class luHhkFile
{
	liStrMap m_siteMap;
public:
	luHhkFile() {}

	bool load(const gkString& fileName);
	gkString getHtmlFile(const gkString& keyword);
};

class luProjectFile
{
	int m_projVer;

	gkString m_projName;
	gkString m_projFileName;
	gkString m_projDir;
	
	liStrVec m_files; //saved with relative path from projdir
	gkString m_startLuaFile;

	bool loadV1(liLuaScript& script);

	int getFileIndex(const gkString& fileName);
	
	enum FLAGS { F_NORMAL, F_MODIFIED = 1 << 0 };
	int m_flags;
public:
	luProjectFile();	
	~luProjectFile();

	enum  { VERSION = 1 };

	bool isModified() { return (m_flags & F_MODIFIED) != 0; }
	void setModified(bool modified = true) { modified ? m_flags |= F_MODIFIED : m_flags &= ~F_MODIFIED; }

	GK_INLINE int getProjVersion() { return m_projVer; }

	GK_INLINE const gkString& getProjFileName() const { return m_projFileName; }
	GK_INLINE const gkString& getProjDir() const { return m_projDir; }

	GK_INLINE const gkString& getProjName() const { return m_projName; }
	GK_INLINE void setProjName(const gkString& name) { m_projName = name; setModified(); }

	bool create(const gkString& fileName, const gkString& projName = "");
	bool save(const gkString& fileName = "");
	bool load(const gkString& fileName);

	bool setStartLuaFile(const gkString& fileName);
	GK_INLINE const gkString& getStartLuaFile() const { return m_startLuaFile; }

	bool addFile(const gkString& fileName);
	bool removeFile(const gkString& fileName);

	GK_INLINE size_t getFileSize() { return m_files.size(); }
	gkString getFile(size_t i, bool relativePath = true);

	bool hasFile(const gkString& fileName);

	GK_INLINE gkString getRelativePath(const gkString& fileName) { return luFile::getRelativePath(fileName, m_projDir); }
	GK_INLINE gkString getAbsolutePath(const gkString& fileName) { return luFile::getAbsolutePath(fileName, m_projDir); }
};

struct luProjTemplate 
{
	gkString name;
	gkString title;
	gkString desc;
	gkString preview;
	liStrVec files;
	gkString startLuaFile;
};

class luProjTempList : public utArray<luProjTemplate>
{
public:
	bool load(const gkString& fileName);
};

#endif //_luFile_h_