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
#ifndef _gkActionActuator_h_
#define _gkActionActuator_h_

#include "gkLogicActuator.h"
#include "Animation/gkActionManager.h"



class gkActionActuator : public gkLogicActuator
{
public:

	enum Mode
	{
		AA_PLAY,
		AA_PONG,
		AA_FLIPPER,
		AA_LOOP_STOP,
		AA_LOOP_END,
		AA_PROPERTY,
	};


private:

	gkScalar m_start, m_end, m_blend, m_curTick;
	int m_mode, m_prio;

	gkString m_startAct, m_startProp;

	bool m_reset, m_isInit;
	gkAction*        m_action;

	void doInit(void);

	void notifyActiveStatus(void);

public:

	gkActionActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkActionActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);


	GK_INLINE void  setStart(int v)                 {m_start = v;}
	GK_INLINE void  setEnd(int v)                   {m_end = v;}
	GK_INLINE void  setBlend(int v)                 {m_blend = v;}
	GK_INLINE void  setMode(int v)                  {m_mode = v;}
	GK_INLINE void  setPriority(int v)              {m_prio = v;}
	GK_INLINE void  setAction(const gkString& v)    {m_startAct = v;}
	GK_INLINE void  setProperty(const gkString& v)  {m_startProp = v;}
	GK_INLINE void  setReset(bool v)                {m_reset = v;}

	GK_INLINE int   getStart(void)                 {return m_start;}
	GK_INLINE int   getEnd(void)                   {return m_end;}
	GK_INLINE int   getBlend(void)                 {return m_blend;}
	GK_INLINE int   getMode(void)                  {return m_mode;}
	GK_INLINE int   getPriority(void)              {return m_prio;}
	GK_INLINE const gkString&  getAction(void)     {return m_startAct;}
	GK_INLINE const gkString&  getProperty(void)   {return m_startProp;}
	GK_INLINE bool  getReset(void)                 {return m_reset;}

	void execute(void);
};

#endif//_gkActionActuator_h_
