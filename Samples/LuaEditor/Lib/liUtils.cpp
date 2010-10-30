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
#include "liUtils.h"


//-- gkString
//from ogre3d StringUtil
liStrVec strSplit( const gkString& str, const gkString& delims, UTuint32 maxSplits)
{
	liStrVec ret;
    // Pre-allocate some space for performance
    ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

    unsigned int numSplits = 0;

    // Use STL methods 
    size_t start, pos;
    start = 0;
    do 
    {
        pos = str.find_first_of(delims, start);
        if (pos == start)
        {
            // Do nothing
            start = pos + 1;
        }
        else if (pos == gkString::npos || (maxSplits && numSplits == maxSplits))
        {
            // Copy the rest of the gkString
            ret.push_back( str.substr(start) );
            break;
        }
        else
        {
            // Copy up to delimiter
            ret.push_back( str.substr(start, pos - start) );
            start = pos + 1;
        }
        // parse up to next real data
        start = str.find_first_not_of(delims, start);
        ++numSplits;

    } while (pos != gkString::npos);



    return ret;
}

gkString getFileName(const gkString& path)
{
	gkString dir, base, ext;
	splitFileName(path, dir, base, ext);
	return ext.empty() ? base : base + "." + ext;
}

gkString getDirName(const gkString& path)
{
	gkString dir, base, ext;
	splitFileName(path, dir, base, ext);
	return dir;
}

gkString getFileNameBase(const gkString& path)
{
	gkString dir, base, ext;
	splitFileName(path, dir, base, ext);
	return base;
}

gkString getFileNameExt(const gkString& path)
{
	gkString dir, base, ext;
	splitFileName(path, dir, base, ext);
	return ext;
}

void splitFileName(const gkString& path, gkString& dir, gkString& base, gkString& ext)
{
	if (path.empty()) return;
	if (path == "." || path == "..")
	{
		dir = path;
		return;
	}

	gkString fpath = path, fname;

	char delim = '/';

	bool bdelim = fpath.find_last_of('\\') != gkString::npos;
	bool fdelim = fpath.find_last_of('/')  != gkString::npos;

	if (bdelim) 
	{
		if (fdelim) //normalize to \ to /
			std::replace(fpath.begin(), fpath.end(), '\\', '/');
		else
			delim = '\\';
	} 

	size_t i = fpath.find_last_of(delim);

	if (i != gkString::npos) 
	{
		dir = fpath.substr(0, i);
		fname = fpath.substr(i+1, path.size()-1);
	} 
	else 
	{
		dir.clear();
		fname = fpath;
	}

	i = fname.find_last_of('.');
	if (i != gkString::npos) 
	{
		base = fname.substr(0, i);
		ext = fname.substr(i+1, fname.size()-1);
	} 
	else 
	{
		base = fname;
		ext.clear();
	}
}


gkString strReplace(const gkString& str, const gkString& oldstr, const gkString& newstr)
{
	return Ogre::StringUtil::replaceAll(str, oldstr, newstr);
}

gkString strToLower(const gkString& str)
{
	gkString ss(str);
    std::transform(str.begin(), str.end(), ss.begin(), tolower);
	return ss;
}


gkString strToUpper(const gkString& str) 
{
    gkString ss(str);
    std::transform(str.begin(), str.end(), ss.begin(), toupper);
	return ss;
}

int str2enum(const gkString& str, const char **enums, int size, int def)
{
	for (int i = 0; i < size; i++)
		if (str == enums[i]) return i;
	return def;
}

/*
UTF-16 LE 	FF FE
UTF-16 BE 	FE FF
UTF-32 LE 	FF FE 00 00
UTF-32 BE 	00 00 FE FF
UTF-8 	    EF BB BF
*/
BOM getTextEncoding(const char *text, size_t len) 
{
	BOM bom[NUM_BOM] = { BOM_UCS2, BOM_UTF8, BOM_NONE };
	static const char *BOM_SIG[NUM_BOM] = {
		"\xFF\xFE",         // UCS-2
		"\xEF\xBB\xBF",     // UTF8
		NULL
	};

	if (!text) return BOM_NONE;
	if (len == 0) len = strlen(text);

	size_t i = 0;

	for (i = 0; BOM_SIG[i] != NULL; i++) {
		size_t blen = strlen(BOM_SIG[i]);
		if (len >= blen && strncmp(text, BOM_SIG[i], blen) == 0) break;
	}

	return bom[i];
}

int getBOMLength(BOM bom) 
{
	static int blen[NUM_BOM] = { 0, 2, 3 };
	return blen[bom];
}


BOM getTextBom(gkString& line, bool removeBom) 
{
	size_t len = line.size();
	BOM bom = getTextEncoding(line.c_str(), len);
	if (bom != BOM_NONE && removeBom) {
		//strip bom
		line = line.substr(getBOMLength(bom));
	}
	return bom;
}


bool getTok(const gkString& buf, const gkString& dim, gkString& tok, size_t &start)
{
	const size_t size = buf.size();
	const size_t dimsize = dim.size();

	size_t pos = gkString::npos;

	while (start < size) 
	{
		pos = buf.find(dim, start);
		
		if (pos == gkString::npos) 
			pos = size;
		
		if (start == pos)
			start += dimsize; 
		else
			break;
	}

	if (start >= size) return false;

	tok.assign(&buf[start], pos-start);
	start = pos + dimsize;

	return true;
}

//-- parse array

int parseIntArray(liIntVec &v, const gkString& str, const gkString& delims)
{
	if (str.empty()) return 0;
	
	liStrVec sv = strSplit(str, delims);
	v.clear(); v.reserve(sv.size());
	for (size_t i = 0; i < sv.size(); i++)
		v.push_back(Ogre::StringConverter::parseInt(sv[i]));

	return v.size();
}


int parseScalarArray(liScalarVec &v, const gkString& str, const gkString& delims)
{
	if (str.empty()) return 0;

	liStrVec sv = strSplit(str, delims);
	v.clear(); v.reserve(sv.size());
	for (size_t i = 0; i < sv.size(); i++)
		v.push_back(Ogre::StringConverter::parseReal(sv[i]));

	return v.size();
}

//--

gkString getIntArrayStr(int arr[], size_t count, char delim)
{
	gkString s;
	for (size_t i = 0; i < count; i++) 
	{
		if (i < count - 1)
			s += utStringFormat("%d%c", arr[i], delim);
		else
			s += utStringFormat("%d", arr[i]);
	}
	return s;
}

gkString getRealArrayStr(gkScalar arr[], size_t count, char delim)
{
	gkString s;
	for (size_t i = 0; i < count; i++) 
	{
		if (i < count - 1)
			s += utStringFormat("%.3f%c", arr[i], delim);
		else
			s += utStringFormat("%.3f", arr[i]);
	}
	return s;
}


gkString getStrArrayStr(gkString arr[], size_t count, char delim)
{
	gkString s;
	for (size_t i = 0; i < count; i++) 
	{
		if (i < count - 1)
			s += utStringFormat("%s%c", arr[i].c_str(), delim);
		else
			s += utStringFormat("%s", arr[i].c_str());
	}
	return s;
}

gkString getIntArrayStr(liIntVec &arr, char delim)
{
	if (arr.empty()) return "";

	return getIntArrayStr(&arr[0], arr.size(), delim);
}

gkString getRealArrayStr(liScalarVec &arr, char delim)
{
	if (arr.empty()) return "";

	return getRealArrayStr(&arr[0], arr.size(), delim);
}


gkString getStrArrayStr(liStrVec &arr, char delim)
{
	if (arr.empty()) return "";

	return getStrArrayStr(&arr[0], arr.size(), delim);
}



//-- end string
