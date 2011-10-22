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
#ifndef _gkLogicManager_h_
#define _gkLogicManager_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "utSingleton.h"

class gkLogicBrick;
class gkLogicSensor;
class gkLogicController;
class gkLogicActuator;
class gkLogicLink;
class gkAbstractDispatcher;


enum gkDispatchedTypes
{
	DIS_CONSTANT = 0,
	DIS_KEY,
	DIS_MOUSE,
	DIS_COLLISION,
	DIS_JOY,
	DIS_MAX,
};




class gkLogicManager : public utSingleton<gkLogicManager>
{
public:

	typedef utListClass<gkLogicLink> Links;
	typedef utListIterator<Links>    LinkIterator;
	typedef gkAbstractDispatcher*    gkAbstractDispatcherPtr;
	typedef utArray<gkLogicBrick*>   Bricks;
	typedef utHashSet<gkLogicBrick*> BrickSet;
	typedef utList<gkLogicActuator*> TickActuators;

protected:

	Links m_links;

	gkAbstractDispatcherPtr*    m_dispatchers;
	Bricks                      m_cin,  m_ain, m_aout; // Temporary open or closed links
	bool                        m_sort;

	BrickSet					m_updateBricks;
	TickActuators 				m_tickActuators; // actuators that get processed by the controller.
											//  This list makes it possible to set the actuator-state to false and only change to true if needed

	void push(gkLogicBrick* a, gkLogicBrick* b, Bricks& in, bool stateValue);

	void clearActuators(void);
	void clearActive(gkLogicLink* link);

	void sort(void);

public:

	gkLogicManager();
	~gkLogicManager();

	gkLogicLink* createLink(void);

	void destroy(gkLogicLink* link);

	GK_INLINE Links& getLinks(void) {return m_links;}

	void notifySceneInstanceDestroyed(void);
	void notifyLinkInstanceDestroyed(gkLogicLink* link);


	///Notifies the manager that a state change has taken place.
	void notifyState(unsigned int state, gkLogicLink* link);


	///Notifies the manager to sort logic bricks based on their priority.
	void notifySort(void);


	///Frees all created links and resets dispatchers
	void clear(void);

	void update(gkScalar delta);


	GK_INLINE gkAbstractDispatcher& getDispatcher(int dt) { GK_ASSERT(m_dispatchers && dt >= 0 && dt <= DIS_MAX); return *m_dispatchers[dt]; }


	///Tells the manager a link from a sensor to controller has been opened or closed.
	void push(gkLogicSensor* s, gkLogicController* v, bool stateValue);

	///Tells the manager a link from a controller to an actuator has been opened or closed.
	void push(gkLogicController* c, gkLogicActuator* v, bool stateValue);

	void GK_INLINE requestUpdate(gkLogicBrick* b) { if (b) m_updateBricks.insert(b); }
	void GK_INLINE removeUpdate(gkLogicBrick* b)  { if (b) m_updateBricks.erase(b);  }

	UT_DECLARE_SINGLETON(gkLogicManager);
};


#endif//_gkLogicManager_h_
