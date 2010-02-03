/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include <sys/stat.h>
#include <stdio.h>

#ifndef S_ISREG
# define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif
#ifndef S_ISDIR
# define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif

#include "gkPath.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
# include <direct.h>
#else
# include <unistd.h>
#endif

using namespace Ogre;



// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
const String gkPath::SEPERATOR= "\\";
#else
const String gkPath::SEPERATOR= "/";
#endif


// ----------------------------------------------------------------------------
void gkGetCurrentDir(String &buf)
{
	char buffer[240];
	getcwd(buffer, 240);
	buf= String(buffer);
}


// ----------------------------------------------------------------------------
gkPath::gkPath()
{
}

// ----------------------------------------------------------------------------
gkPath::gkPath(const String& file) :
		m_path(file)
{
}

// ----------------------------------------------------------------------------
gkPath::gkPath(const char* file) :
		m_path(file)
{
}

// ----------------------------------------------------------------------------
gkPath::~gkPath()
{
}

// ----------------------------------------------------------------------------
const String& gkPath::getPath(void) const
{
	return m_path;
}

// ----------------------------------------------------------------------------
String gkPath::getAbsPath(void) const
{
	if (!exists())
	{
		/// cannot get path
		return StringUtil::BLANK;
	}

	if (isAbs())
		return m_path;

	String curDir;
	gkGetCurrentDir(curDir);
	return curDir + SEPERATOR + m_path;
}

// ----------------------------------------------------------------------------
int gkPath::getFileSize(void) const
{
	struct stat st;
	if (stat(m_path.c_str(), &st) == 0)
		return (int)st.st_size;
	return -1;
}

// ----------------------------------------------------------------------------
void gkPath::append(const String &v)
{
	if (m_path.at(m_path.size()-1) != SEPERATOR[0])
		m_path += SEPERATOR;

	m_path += v;
}


// ----------------------------------------------------------------------------
String gkPath::getAsString(void) const
{
	String ret;
	getAsString(ret);
	return ret;
}

// ----------------------------------------------------------------------------
void gkPath::getAsString(String &dest) const
{
	if (!isFile())
		return;

	int fileSize= getFileSize();

	/// empty file
	if (fileSize <= 0)
		return;

	char *data= new char[fileSize+1];


	FILE *fp= fopen(m_path.c_str(), "rb");   // always in binary
	GK_ASSERT(fp);

	fread(data, fileSize, 1, fp);
	fclose(fp);

	/// term
	data[fileSize]= 0;
	dest= data;
	delete [] data;
}


// ----------------------------------------------------------------------------
String gkPath::directory(void) const
{
	if (isDir())
		return m_path;

	normalizePlatform();

	StringVector sp;
	split(sp);

	String dir;
    if (!sp.empty())
    {
        size_t size = sp.size() - 1;
	    for (size_t i=0; i<size; i++)
        {
		    dir += sp[i];
            if ((i + 1) != size)
                dir += SEPERATOR;
        }
    }
	return dir;
}

// ----------------------------------------------------------------------------
String gkPath::base(void) const
{
	normalizePlatform();

	StringVector arr;
	split(arr);

	if (arr.empty())
		return StringUtil::BLANK;
	return arr.at(arr.size() - 1);
}

// ----------------------------------------------------------------------------
String gkPath::extension(void) const
{
	// split base on .
	String bn= base();

	if (bn.empty())
		return StringUtil::BLANK;

	StringVector arr= StringUtil::split(bn, ".");

	if (arr.empty())
		return StringUtil::BLANK;
	return String(".") + arr.at(arr.size() - 1);
}

// ----------------------------------------------------------------------------
void gkPath::normalize(void) const
{
	StringUtil::replaceAll(m_path, "\\\\", "/");
}

// ----------------------------------------------------------------------------
void gkPath::normalizePlatform(void) const
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	StringUtil::replaceAll(m_path, "/", "\\");
#else
	StringUtil::replaceAll(m_path, "\\\\", "/");
#endif
}

// ----------------------------------------------------------------------------
bool gkPath::isAbs(void) const
{
	/// assumes (Drive:) || or root /
	if (m_path.empty())
		return false;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	if (m_path.size() > 2)
		return m_path[1] == ':';
#else
	if (m_path.size() >= 1)
		return m_path[0] == '/';
#endif
	return false;
}

// ----------------------------------------------------------------------------
bool gkPath::isRel(void) const
{
	return !isAbs();
}

// ----------------------------------------------------------------------------
bool gkPath::exists(void) const
{
	return isFile() || isDir();
}

// ----------------------------------------------------------------------------
bool gkPath::isFile(void) const
{
	struct stat st;
	return stat(m_path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

// ----------------------------------------------------------------------------
bool gkPath::isDir(void) const
{
	struct stat st;
	return stat(m_path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

// ----------------------------------------------------------------------------
void gkPath::split(StringVector &arr) const
{
	arr= StringUtil::split(m_path, SEPERATOR);
}
