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
#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
# include <windows.h>
#else
# include <unistd.h>
#endif

#include "OgreResourceGroupManager.h"
#include "gkUtils.h"
#include "gkPath.h"

using namespace Ogre;

#ifdef __APPLE__
#define MAXPATHLEN 512
char* AppleGetBundleDirectory(void)
{
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
    return path;
}
#endif


gkString gkUtils::getFile(const gkString& in)
{
    char newName[1024];

#ifdef __APPLE__

    char* bundlePath = AppleGetBundleDirectory();

    //cut off the .app filename
    char* lastSlash = 0;
    if (lastSlash = strrchr(bundlePath, '/'))
        * lastSlash = '\0';

    sprintf(newName, "%s/%s", bundlePath, "game.blend");
	FILE* f = fopen(newName,"rb");
	
	if (f)
    {
		fclose(f);
    } else 
	{
		sprintf(newName,"%s/%s/%s",AppleGetBundleDirectory(),"Contents/Resources",in.c_str());
	}

#else

    sprintf(newName, "%s", in.c_str());

#endif

    return newName;
}


bool gkUtils::isResource(const gkString &name, const gkString &group)
{
    if (group.empty())
        return Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(name);
    return Ogre::ResourceGroupManager::getSingleton().resourceExists(group, name);
}



#include "zlib.h"

#define CHUNK       16384
#define GZSTREAM    31


int zpipe_inf(std::ifstream &source, std::ostream &dest)
{
    int ret;
    unsigned int have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    // allocate inflate state
    strm.zalloc     = Z_NULL;
    strm.zfree      = Z_NULL;
    strm.opaque     = Z_NULL;
    strm.avail_in   = 0;
    strm.next_in    = Z_NULL;

    ret = inflateInit2(&strm, GZSTREAM);
    if (ret != Z_OK)
        return ret;

    do
    {
        source.read((char *)in, CHUNK);
        strm.avail_in = source.gcount();

        if (strm.avail_in == 0)
            break;

        strm.next_in = in;

        do
        {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR); 

            switch (ret)
            {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR; 
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }

            have = CHUNK - strm.avail_out;
            dest.write((char *)out, have);
            if (!dest.good())
            {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        }
        while (strm.avail_out == 0);
    }
    while (ret != Z_STREAM_END);

    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


void gkUtils::inflate(const gkString &infile, gkString &out)
{
    std::ifstream fp;
    fp.open(infile.c_str(), std::ios::in | std::ios::binary);
    if (fp.is_open())
    {
        std::stringstream dest(std::ios::in | std::ios::out | std::ios::binary);
        if (zpipe_inf(fp, dest) == Z_OK)
            out = dest.str();
    }
}
