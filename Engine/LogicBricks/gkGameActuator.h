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
#ifndef _gkGameActuator_h_
#define _gkGameActuator_h_

#include "gkLogicActuator.h"


class gkGameActuator : public gkLogicActuator
{
public:

	enum Mode
	{
		GM_QUIT,
		GM_RESTART,
		GM_START_NEW,
		GM_SAVE,
		GM_LOAD,
	};


private:

	int m_mode;
	gkString m_otherGame;

public:

	gkGameActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkGameActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	GK_INLINE void  setMode(int v)                  {m_mode = v;}
	GK_INLINE void  setGameFile(const gkString& v)  {m_otherGame = v;}

	GK_INLINE int             getMode(void)         const {return m_mode;}
	GK_INLINE const gkString& getGameFile(void)     const {return m_otherGame;}


	void execute(void);
};

#endif//_gkGameActuator_h_
