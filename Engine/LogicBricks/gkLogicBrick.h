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
#ifndef _gkLogicBrick_h_
#define _gkLogicBrick_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "gkString.h"

class gkLogicSensor;
class gkLogicController;
class gkLogicActuator;
class gkLogicLink;


typedef utList<gkLogicSensor*>          gkSensorList;
typedef utArray<gkLogicSensor*>         gkSensors;
typedef utArray<gkLogicController*>     gkControllers;
typedef utArray<gkLogicActuator*>       gkActuators;

typedef utListIterator<gkSensorList>    gkSensorListIterator;
typedef utArrayIterator<gkSensors>      gkSensorIterator;
typedef utArrayIterator<gkControllers>  gkControllerIterator;
typedef utArrayIterator<gkActuators>    gkActuatorIterator;

enum gkBrickMode
{
	BM_IDLE = 0,
	BM_OFF,
	BM_ON,
};



class gkLogicBrick
{
public:

	class Listener
	{
	public:

		enum Mode
		{
			OVERIDE,
			AND,
		};

		int m_mode;

	public:
		Listener() : m_mode(OVERIDE) {}
		virtual ~Listener() {}

		virtual bool executeEvent(gkLogicBrick* brick) = 0;
	};


protected:

	gkGameObject*       m_object;
	gkScene*            m_scene;
	const gkString      m_name;
	gkLogicLink*        m_link;
	int                 m_stateMask, m_pulseState, m_debugMask;
	bool                m_isActive;
	int                 m_priority;
	Listener*           m_listener;

	virtual void        cloneImpl(gkLogicLink* link, gkGameObject* dest);
	virtual void        notifyActiveStatus(void) {}


public:
	gkLogicBrick(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkLogicBrick();

	bool inActiveState(void) const;

	bool wantsDebug(void) const;

	virtual gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest) = 0;

	virtual void notifyLinkDestroyed(void) {}

	virtual void execute(void) = 0;

	virtual void update(void) {}

	GK_INLINE void setListener(gkLogicBrick::Listener* listener) {m_listener = listener;}


	GK_INLINE void              setPulse(int v)           { m_pulseState = v;}
	GK_INLINE int               getPulse(void)      const { return m_pulseState;}
	GK_INLINE bool              isPulseOn(void)     const { return m_pulseState == BM_ON; }
	GK_INLINE bool              isPulseOff(void)    const { return m_pulseState == BM_OFF; }
	GK_INLINE bool              isIdle(void)        const { return m_pulseState == BM_IDLE; }
	GK_INLINE void              setActive(bool v)         { m_isActive = v; notifyActiveStatus(); }
	GK_INLINE bool              isActive(void)      const { return m_isActive;}

	GK_INLINE const gkString&   getName(void)       const { return m_name; }
	GK_INLINE gkGameObject*     getObject(void)           { return m_object; }
	GK_INLINE void              setMask(int v)            { m_stateMask = v;}
	GK_INLINE int               getMask(void)       const { return m_stateMask;}
	GK_INLINE void              setDebugMask(int v)       { m_debugMask = v;}
	GK_INLINE int               getDebugMask(void)  const { return m_debugMask;}
	GK_INLINE gkLogicLink*      getLink(void)             { return m_link; }
	GK_INLINE int               getPriority(void)   const { return m_priority;}

	void setPriority(bool v);
	void setPriority(int v);

	const gkString&				getObjectGroupName(void) const;
};


#endif//_gkLogicBrick_h_
