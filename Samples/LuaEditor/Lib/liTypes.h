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

#ifndef _liTypes_h_
#define _liTypes_h_

template <typename T>
struct liArray
{
	typedef utArray<T> type;
};

template<typename T, typename V>
struct liMap
{
	typedef std::map<T, V> type;	//Later, std::map should be changed to utMap?
};

typedef liArray<gkString>::type		liStrVec;
typedef liArray<int>::type			liIntVec;
typedef liArray<UTuint32>::type		liUIntVec;
typedef liArray<float>::type		liFloatVec;
typedef liArray<gkScalar>::type		liScalarVec;
typedef liArray<double>::type		liDoubleVec;


typedef liMap<gkString, gkString>::type		liStrMap;
typedef liMap<gkString, liStrMap>::type		liStrGroups;
typedef liMap<gkString, liStrGroups>::type	liStrSections;

extern const gkString		Empty_liStr;
extern const liStrMap		Empty_liStrMap;
extern const liStrGroups	Empty_liStrGroups;

#endif //_liTypes_h_
