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
#ifndef _gkLuaScript_h_
#define _gkLuaScript_h_

#include "gkCommon.h"
#include "gkResource.h"

struct lua_State;

class gkLuaScript : public gkResource
{
protected:
	gkString		m_text;
	int             m_script;
	bool            m_compiled;
	bool			m_isInvalid;

	bool			m_lastRetBoolValue;
	gkString		m_lastRetStrValue;

	void compile(void);

public:
	gkLuaScript(gkResourceManager *creator, const gkResourceName &name, const gkResourceHandle &handle);
	~gkLuaScript();

	GK_INLINE bool  isCompiled(void)          const {return m_compiled;}

	void setScript(const gkString& text);

	void decompile(void);
	// compile & run the script
	bool execute(void);

	GK_INLINE bool     getReturnBoolValue() { return m_lastRetBoolValue; }
	GK_INLINE gkString getReturnStrValue()  { return m_lastRetStrValue;  }
};



#endif//_gkLuaScript_h_
