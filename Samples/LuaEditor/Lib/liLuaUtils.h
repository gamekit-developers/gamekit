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


#ifndef _liLuaUtils_h_
#define _liLuaUtils_h_

struct lua_State;

class liLuaScript {
	lua_State* m_L;
	gkString m_error;

public:
	liLuaScript(bool useLibs=true);
	virtual ~liLuaScript();

	bool isInited() { return m_L != NULL; }

	const gkString& getLastError() const { return m_error; }

	//pop stack top gkString, if stack is empty, return ""
	gkString popStr();
	//return gkString from global table
	gkString getGlobalStr(const gkString& name);

	//call from file
	bool call(const gkString& filename);
	//call from buffer
	bool call(const gkString& buf, const gkString& name);

	//dump stack
	void dumpStack();

	lua_State* getLuaState() { return m_L; }
	int getTop() const;

	int getTableSize(const gkString& table);
	gkString getTableStr(const gkString& table, const gkString& field);
	gkString getTableStr(const gkString& table, int index);
};

#endif //_liLuaUtils_h_