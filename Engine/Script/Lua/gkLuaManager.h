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
#ifndef _gkLuaManager_h_
#define _gkLuaManager_h_

#include "gkCommon.h"
#include "gkResourceManager.h"
#include "gkMathUtils.h"
#include "gkEngine.h"
#include "gkString.h"
#include "utSingleton.h"
#include "Script/Lua/gkLuaScript.h"

struct lua_State;


class gkLuaManager : public gkResourceManager, public utSingleton<gkLuaManager>
{
public:


	//typedef utHashTable<gkHashedString, gkLuaScript*> ScriptMap;
	//typedef utList<gkLuaScript*> ScriptList;

private:
	lua_State*   L;
	//ScriptMap   m_scripts;


public:
	gkLuaManager();
	virtual ~gkLuaManager();

	gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle);


	// access to the lua virtual machine
	GK_INLINE lua_State* getLua(void) {return L;}

	void decompileAll(void);
	void decompileGroup(const gkString& group);


	//gkLuaScript* getScript(const gkString& name);

	// Create new script from text buffer
	gkLuaScript* createFromText(const gkResourceName& name, const gkString& text);

	// create from internal text file manager
	gkLuaScript* createFromTextBlock(const gkResourceName& name);

	// Destroys named file
	//void destroy(const gkString& name);

	// Destroys file pointer
	//void destroy(gkLuaScript* ob);

	// Destroys all internal files
	//void destroyAll(void);

	// Test for file existance
	//bool hasScript(const gkString& name);

protected:
	virtual void notifyResourceCreatedImpl(gkResource* res);
	virtual void notifyResourceDestroyedImpl(gkResource* res);


	UT_DECLARE_SINGLETON(gkLuaManager);
};

#endif//_gkLuaManager_h_
