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
#ifndef _gkLogicSensor_h_
#define _gkLogicSensor_h_

#include "gkLogicBrick.h"


class gkLogicSensor : public gkLogicBrick
{
public:

	/// Open/Close mode as part of the sensor's header.
	enum Pulse
	{
		PM_IDLE     = (1 << 0),
		PM_TRUE     = (1 << 1),
		PM_FALSE    = (1 << 2),
	};

protected:

	gkControllers m_controllers;


	int     m_freq, m_tick, m_pulse;
	bool    m_invert, m_positive, m_suspend, m_tap, m_firstExec;
	bool    m_sorted, m_isDetector;
	int     m_dispatchType;


	// state cache
	int     m_oldState;

	// tap detection
	int m_firstTap, m_lastTap;


	void cloneImpl(gkLogicLink* link, gkGameObject* dest);


public:

	gkLogicSensor(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkLogicSensor();

	void connect(void);
	void disconnect(void);

	void link(gkLogicController* cont);

	void execute(void);

	virtual bool query(void) = 0;

	void sort(void);

	///Reset the sensor's header to initial state.
	void reset(void);


	///Tells the logic manager events have happened and a link should be opened or closed.
	void dispatch(void);


	bool isPositive(void) const;

	GK_INLINE gkControllers& getControllers(void) {return m_controllers;}


	GK_INLINE bool isNegativePulseMode(void) const {return (m_pulse & PM_FALSE) != 0;}
	GK_INLINE bool isPositivePulseMode(void) const {return (m_pulse & PM_TRUE) != 0;}
	GK_INLINE void setFrequency(int v)             {m_freq = (int)((((float)v) + .5) / 2.0);}
	GK_INLINE int  getFrequency(void)        const {return m_freq;}
	GK_INLINE void setMode(int m)                  {m_pulse = m;}
	GK_INLINE int  getMode(void)             const {return m_pulse;}
	GK_INLINE void invert(bool v)                  {m_invert = v;}
	GK_INLINE bool isInverse(void)           const {return m_invert;}
	GK_INLINE void suspend(bool v)                 {m_suspend = v;}
	GK_INLINE bool isSuspended(void)         const {return m_suspend;}
	GK_INLINE void setTap(bool v)                  {m_tap = v;}
	GK_INLINE bool isTap(void)               const {return m_tap;}
	GK_INLINE void setDetector(bool v)             {m_isDetector = v;}
	GK_INLINE bool isDetector(void)          const {return m_isDetector;}
	GK_INLINE void setStartState(int v)            {m_oldState = v;}
	GK_INLINE int  getDispatcher(void)       const {return m_dispatchType;}
};


#endif//_gkLogicSensor_h_
