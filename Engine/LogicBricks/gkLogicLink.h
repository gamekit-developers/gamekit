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
#ifndef _gkLogicLink_h_
#define _gkLogicLink_h_

#include "gkCommon.h"
#include "gkString.h"

class gkLogicBrick;
class gkLogicActuator;
class gkLogicController;
class gkLogicSensor;


class gkLogicLink : public utListClass<gkLogicLink>::Link
{
public:
	typedef utList<gkLogicBrick*> BrickList;
	typedef utHashTable<utPointerHashKey, gkLogicBrick*> BrickFinder;
	typedef utArray<gkLogicLink*> OtherLinks;


protected:

	BrickList   m_sensors;
	BrickList   m_controllers;
	BrickList   m_actuators;
	int         m_state, m_debug;

	OtherLinks  m_others;

	BrickFinder m_sfind, m_cfind, m_afind;
	gkGameObject* m_object;
    bool        m_externalOwner;

public:

	gkLogicLink();
	~gkLogicLink();

	gkLogicLink*     clone(gkGameObject* dest);
	void            destroyInstance(void);



	gkLogicSensor* findSensor(const gkString& name);
	gkLogicActuator* findActuator(const gkString& name);
	gkLogicController* findController(const gkString& name);

	gkLogicSensor* findSensor(void* user);
	gkLogicActuator* findActuator(void* user);
	gkLogicController* findController(void* user);

	void push(gkLogicSensor* v, void* user = 0);
	void push(gkLogicController* v, void* user = 0);
	void push(gkLogicActuator* v, void* user = 0);

	void notifyLink(gkLogicLink* link);
	void notifyState(void);
	bool hasLink(gkLogicLink* link);

	BrickList& getSensors(void) {return m_sensors;}
	BrickList& getControllers(void) {return m_controllers;}
	BrickList& getActuators(void) {return m_actuators;}

	GK_INLINE void          setDebug(int v)                   {m_debug = v;}
	GK_INLINE void          setState(int v)                   {m_state = v;}
	GK_INLINE void          setObject(gkGameObject* v)        {m_object = v;}
    GK_INLINE void          setExternalOwner(bool b)          {m_externalOwner = b;}
	GK_INLINE int           getState(void)              const {return m_state;}
	GK_INLINE int           getDebug(void)              const {return m_debug;}
	GK_INLINE gkGameObject* getObject(void)             const {return m_object;}
	GK_INLINE bool          getExternalOwner(void)      const {return m_externalOwner;}
};

#endif//_gkLogicLink_h_
