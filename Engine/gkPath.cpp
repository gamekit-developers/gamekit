/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "gkPath.h"

#include <sys/stat.h>
#include <stdio.h>
#include <limits.h>

#if GK_PLATFORM == GK_PLATFORM_LINUX
#include <linux/limits.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 240
#endif

#ifndef S_ISREG
# define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif
#ifndef S_ISDIR
# define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif


#if GK_PLATFORM == GK_PLATFORM_WIN32
# include <direct.h>
#else
# include <unistd.h>
#endif


#if GK_PLATFORM == GK_PLATFORM_WIN32
const gkString gkPath::SEPERATOR = "\\";
#else
const gkString gkPath::SEPERATOR = "/";
#endif

void gkGetCurrentDir(gkString& buf)
{
	char buffer[PATH_MAX];
	if(getcwd(buffer, sizeof(buffer)) != NULL)
		buf = gkString(buffer);
	else
		buf = gkString("");
}



gkPath::gkPath()
{
}


gkPath::gkPath(const gkString& file) :
	m_path(file)
{
}


gkPath::gkPath(const char* file) :
	m_path(file)
{
}


gkPath::~gkPath()
{
}


const gkString& gkPath::getPath(void) const
{
	return m_path;
}


gkString gkPath::getAbsPath(void) const
{
	if (!exists())
	{
		// cannot get path
		return gkStringUtils::BLANK;
	}

	if (isAbs())
		return m_path;

	gkString curDir;
	gkGetCurrentDir(curDir);
	return curDir + SEPERATOR + m_path;
}


int gkPath::getFileSize(void) const
{
	struct stat st;
	if (stat(m_path.c_str(), &st) == 0)
		return (int)st.st_size;
	return -1;
}


void gkPath::append(const gkString& v)
{
	if (m_path.at(m_path.size() - 1) != SEPERATOR[0])
		m_path += SEPERATOR;

	m_path += v;
}



gkString gkPath::getAsString(void) const
{
	gkString ret;
	getAsString(ret);
	return ret;
}


void gkPath::getAsString(gkString& dest) const
{
	if (!isFile())
		return;

	int fileSize = getFileSize();

	// empty file
	if (fileSize <= 0)
		return;

	char* data = new char[fileSize+1];


	FILE* fp = fopen(m_path.c_str(), "rb");  // always in binary
	GK_ASSERT(fp);

	if (fp)
	{
		fread(data, fileSize, 1, fp);
		fclose(fp);
		// term
		data[fileSize] = 0;
		dest = data;
	}

	delete [] data;
}



gkString gkPath::directory(void) const
{
	if (isDir())
		return m_path;

	normalizePlatform();

	gkStringVector sp;
	split(sp);

	gkString dir;
	if (!sp.empty())
	{
		size_t size = sp.size() - 1;
		for (size_t i = 0; i < size; i++)
		{
			dir += sp[i];
			if ((i + 1) != size)
				dir += SEPERATOR;
		}
	}
	return dir;
}


gkString gkPath::base(void) const
{
	normalizePlatform();

	gkStringVector arr;
	split(arr);

	if (arr.empty())
		return gkStringUtils::BLANK;
	return arr.at(arr.size() - 1);
}


gkString gkPath::extension(void) const
{
	// split base on .
	gkString bn = base();

	if (bn.empty())
		return gkStringUtils::BLANK;

	gkStringVector arr;
	utStringUtils::split(arr, bn, ".");
	if (arr.empty())
		return gkStringUtils::BLANK;
	return gkString(".") + arr.at(arr.size() - 1);
}


void gkPath::normalize(void) const
{
	utStringUtils::replace(m_path, "\\\\", "/");
}


void gkPath::normalizePlatform(void) const
{
#if GK_PLATFORM == GK_PLATFORM_WIN32
	utStringUtils::replace(m_path, "/", "\\");
#else
	utStringUtils::replace(m_path, "\\\\", "/");
#endif
}


bool gkPath::isAbs(void) const
{
	// assumes (Drive:) || or root /
	if (m_path.empty())
		return false;
#if GK_PLATFORM == GK_PLATFORM_WIN32
	if (m_path.size() > 2)
		return m_path[1] == ':';
#else
	if (m_path.size() >= 1)
		return m_path[0] == '/';
#endif
	return false;
}

gkString gkPath::getBundlePath(void) const
{
#ifdef __APPLE__
#ifndef MAXPATHLEN
#define MAXPATHLEN 512
#endif
	CFURLRef bundleURL;
	CFStringRef pathStr;
	static char path[MAXPATHLEN];
	memset(path, MAXPATHLEN, 0);
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	bundleURL = CFBundleCopyBundleURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	CFRelease(pathStr);
	CFRelease(bundleURL);

	char* lastSlash = 0;
	if (lastSlash = strrchr((char*)path, '/'))
		* lastSlash = '\0';

	return path;
#endif
	return "";
}


bool gkPath::isRel(void) const
{
	return !isAbs();
}


bool gkPath::exists(void) const
{
	return isFile() || isDir();
}


bool gkPath::isFile(void) const
{
	// skip blender relative paths.
	// stat can sometimes be extremely slow!
	// (not sure if fopen() == NULL would be any better.)
	if (m_path.size() < 2 || (m_path[0] == '/' && m_path[1] == '/'))
		return false;

	struct stat st;
	return stat(m_path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}


bool gkPath::isDir(void) const
{
	// skip blender relative paths.
	if (m_path.size() < 2 || (m_path[0] == '/' && m_path[1] == '/'))
		return false;

	struct stat st;
	return stat(m_path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

bool gkPath::isFileInBundle(void) const
{
#ifdef __APPLE__
	char newName[1024];

#ifdef OGREKIT_BUILD_IPHONE
	sprintf(newName, "%s/%s", getBundlePath().c_str(), m_path.c_str());
#else
	sprintf(newName, "%s/%s/%s", getBundlePath().c_str(), "Contents/Resources", m_path.c_str());
#endif

	struct stat st;
	return stat(newName, &st) == 0 && S_ISREG(st.st_mode);
#else
	return isFile();
#endif

}


void gkPath::split(gkStringVector& arr) const
{
	utStringUtils::split(arr, m_path, SEPERATOR);
}
