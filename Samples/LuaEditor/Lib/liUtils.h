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

#ifndef _luUtils_h_
#define _luUtils_h_

//-- type
#include "liTypes.h"


//-- memory

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif

#define ARRAY_SIZE(a)        (int)(sizeof(a) / sizeof(a[0]))

//-- gkString

//ex) c:/temp/a.txt
gkString getFileName(const gkString& path);     //a.txt
gkString getDirName(const gkString& path);      //c:/temp
gkString getFileNameExt(const gkString& path);  //txt
gkString getFileNameBase(const gkString& path); //a
void   splitFileName(const gkString& path, gkString& dir, gkString& base, gkString& ext);


gkString strReplace(const gkString& str, const gkString& oldstr, const gkString& newstr);
gkString strToLower(const gkString& str);
gkString strToUpper(const gkString& str);

bool getTok(const gkString& buf, const gkString& dim, gkString& tok, UTuint32& start);

//if lowerCase then pattern must be lowercase.
GK_INLINE bool strCmp(const gkString& s1, const gkString& s2, bool caseSensitive=false)  { return Ogre::StringUtil::match(s1,s2,caseSensitive);       }
GK_INLINE bool strStart(const gkString& s, const gkString& pattern, bool lowerCase=true) { return Ogre::StringUtil::startsWith(s, pattern, lowerCase); }
GK_INLINE bool strEnd(const gkString& s, const gkString& pattern, bool lowerCase=true)   { return Ogre::StringUtil::endsWith(s, pattern, lowerCase);   }

GK_INLINE bool strFind(const gkString& s, const gkString& match, bool caseSensitive=false)
{
	if (caseSensitive) return s.find(match) != gkString::npos;

	gkString a(s), b(match);
	Ogre::StringUtil::toLowerCase(a); Ogre::StringUtil::toLowerCase(b);
	return a.find(b) != gkString::npos;
}

GK_INLINE int findStr(const gkString& s, const liStrVec &ss, bool caseSenstive=false)
{
	for (size_t i = 0; i < ss.size(); i++)
		if (strCmp(s, ss[i], caseSenstive)) return (int)i;

	return -1;
}

liStrVec strSplit(const gkString& s, const gkString& delims = ", ", UTuint32 maxSplits = 0);
//-- parser array

int parseIntArray(liIntVec &v, const gkString& str, const gkString& delims=", ");
int parseFloatArray(liFloatVec &v, const gkString& str, const gkString& delims=", ");
int parseScalarArray(liScalarVec &v, const gkString& str, const gkString& delims=", ");
GK_INLINE
int parseStrArray(liStrVec &v, const gkString& str, const gkString& delims=", ")
{
	if (str.empty()) return 0;

	v = strSplit(str, delims);

	return v.size();
}

//-- get array str

gkString getIntArrayStr(int arr[], size_t count, char delim=',');
gkString getFloatArrayStr(float arr[], size_t count, char delim=',');
gkString getStrArrayStr(gkString arr[], size_t count, char delim='\t');

gkString getIntArrayStr(liIntVec &arr, char delim=',');
gkString getFloatArrayStr(liFloatVec &arr, char delim=',');
gkString getStrArrayStr(liStrVec &arr, char delim='\t');


//-- enum

int str2enum(const gkString& str, const char **enums, int size, int def=0);

#define STR2ENUM(STR,ENUM)					 (ENUM)str2enum(STR, S_##ENUM, NUM_##ENUM)
#define STR2ENUM2(STR,ENUM,DEFAULT)			 (ENUM)str2enum(STR, S_##ENUM, NUM_##ENUM, DEFAULT)
#define ENUM2STR(VALUE,ENUM)				 S_##ENUM[VALUE]

#define STR2ENUMC(STR,CLASS,ENUM)            (CLASS::ENUM)str2enum(STR, CLASS::S_##ENUM, CLASS::NUM_##ENUM)
#define STR2ENUMC2(STR,CLASS,ENUM,DEFAULT)   (CLASS::ENUM)str2enum(STR, CLASS::S_##ENUM, CLASS::NUM_##ENUM, DEFAULT)
#define ENUM2STRC(VALUE,CLASS,ENUM)          CLASS::S_##ENUM[VALUE]

#define DECLARE_ENUM(ENUM) \
	extern const char *S_##ENUM[NUM_##ENUM];


#define DECLARE_ENUMC(ENUM) \
	static const char *S_##ENUM[NUM_##ENUM];

#define IMPLEMENT_ENUM(ENUM) \
	const char *S_##ENUM[NUM_##ENUM] =
#define IMPLEMENT_ENUMC(CLASS,ENUM) \
	const char *CLASS::S_##ENUM[CLASS::NUM_##ENUM] =


//-- encoding

enum BOM { BOM_NONE, BOM_UCS2, BOM_UTF8, NUM_BOM };
BOM getTextEncoding(const char *text, size_t len) ;
int getBOMLength(BOM bom);
BOM getTextBom(gkString& line, bool removeBom=false);


#endif //_luUtils_h_
