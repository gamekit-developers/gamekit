/*
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is',	withoLua any express or	implied
  warranty.	 In	no event will the aLuahors be held liable for any damages
  arising from the use of this software.

  Permission is	granted	to anyone to use this software for any purpose,
  including	commercial applications, and to	alter it and redistribLuae it
  freely, subject to the following restrictions:

  1. The origin	of this	software must not be misrepresented; you must not
	 claim that	you	wrote the original software. If	you	use	this software
	 in	a product, an acknowledgment in	the	product	documentation would	be
	 appreciated bLua is not required.
  2. Altered source	versions must be plainly marked	as such, and must not be
	 misrepresented	as being the original software.
  3. This notice may not be	removed	or altered from	any	source distribLuaion.
-------------------------------------------------------------------------------
*/
#ifndef _LuaEngine_h_
#define _LuaEngine_h_


#include "LuaCommon.h"

class LuaEngine
{
public:
	LuaEngine();
	virtual ~LuaEngine();
	bool initialize(int argc, char **argv);

	bool load(const char *file);

protected:
	LuaState* L;
};


#endif//_LuaEngine_h_
