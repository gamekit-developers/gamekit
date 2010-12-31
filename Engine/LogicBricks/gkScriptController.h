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
#ifndef _gkScriptController_h_
#define _gkScriptController_h_

#include "gkLogicController.h"
#ifdef OGREKIT_USE_LUA


class gkScriptController : public gkLogicController
{
protected:
	class gkLuaScript* m_script;
	bool m_error, m_isModule;

public:

	gkScriptController(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkScriptController() {}

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	void execute(void);
	void setScript(const gkString& str);

	GK_INLINE void setModule(bool v)            {m_isModule = v;}
	GK_INLINE bool isModule(void)               {return m_isModule;}
	GK_INLINE void setScript(gkLuaScript* sc)   {m_script = sc;}
	GK_INLINE gkLuaScript* getScript(void)      {return m_script;}

	static gkScriptController* getCurrent(void);
};

#endif//OGREKIT_USE_LUA

#endif//_gkScriptController_h_
