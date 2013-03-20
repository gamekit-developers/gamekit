/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#include "gsBricks.h"
#include "gsCore.h"


#define GS_TYPEOF(a, b)    ((a) && typeid(*a) == typeid(b))
#define GS_TYPE_RET(P, T)  if (GS_TYPEOF(P, gk##T)) { return new gs##T(P); }



gsLogicManager::gsLogicManager(gsScene* scene) : m_scene(0)
{
	if (scene)
		m_scene = scene->getOwner();
	else
		m_scene = gkEngine::getSingleton().getActiveScene();
}


gsLogicManager::~gsLogicManager()
{
}



gkLogicLink* gsLogicManager::newObject(gsGameObject* obj)
{
	if (!m_scene)
		return 0;

	gkLogicManager* lptr = m_scene->getLogicBrickManager();

	if (lptr && obj)
	{
		gkLogicLink* lnk = lptr->createLink();

		lnk->setObject(obj->get());
		lnk->setState(1);
		// setting this true let the lua-garbagecollector destroy the link as well
		// eventhough it was still be used.
        lnk->setExternalOwner(false);
//        lnk->setExternalOwner(true);
		return lnk;
	}

	return 0;
}


gkLogicLink* gsLogicManager::getObject(const gkString& name)
{
	gkLogicManager* lptr = m_scene->getLogicBrickManager();

	gkSceneManager::Resources scenes = gkSceneManager::getSingleton().getResources();
	if (lptr)
	{
		gkLogicManager::Links& lnks = lptr->getLinks();
		if (!lnks.empty())
		{
			gkLogicManager::LinkIterator iter(lnks);

			while (iter.hasMoreElements())
			{
				gkLogicLink* lnk = iter.getNext();
				gkGameObject* obj = lnk->getObject();

				if (obj && obj->getName() == name)
					return lnk;
			}
		}
	}
	return 0;
}



gsArray<gsLogicObject, gkLogicLink> gsLogicManager::getObjectList()
{
	gkLogicManager* lptr = m_scene->getLogicBrickManager();

	if (lptr)
	{
		gkLogicManager::Links& lnks = lptr->getLinks();

		gsArray<gsLogicObject, gkLogicLink> arr;

		if (!lnks.empty())
		{
			gkLogicManager::LinkIterator iter(lnks);

			while (iter.hasMoreElements())
			{
				gkLogicLink* lnk = iter.getNext();
				arr.push(lnk);
			}
		}
		return arr;
	}
	return gsArray<gsLogicObject, gkLogicLink>();
}



//
//  Link impl
//





gsLogicObject::gsLogicObject() : m_link(0), m_incr(0)
{
}


gsLogicObject::gsLogicObject(gkLogicLink* lnk)  : m_link(lnk), m_incr(0)
{
}



gsLogicObject::~gsLogicObject()
{
	// actually I wonder if gsLogicObject should destroy also the gk-object?
	// this is done when gkGameObject is destroyed. Here we shouldn't trust the
	// lua-garbagecollector as we know that a lua-ref is not need for the gamelogic still to work
	// I only comment this out. Maybe there was something I did not think of, then we can bring this
	// back to work
	//
	//if (m_link && m_link->getExternalOwner())
	//{
	//	gkLogicManager* lptr = m_link->getLogicManager();
	//	if (lptr)
	//		lptr->destroy(m_link);
	//}
}


static void gsLogicObject_setDebugBricks(gkLogicLink::BrickList& lnks)
{
	if (!lnks.empty())
	{
		utListIterator<gkLogicLink::BrickList> iter(lnks);
		while (iter.hasMoreElements())
		{
			gkLogicBrick* brk = iter.getNext();
			brk->setDebugMask(brk->getLink()->getDebug());
		}
	}
}



void gsLogicObject::setDebug(bool v)
{
	if (m_link)
	{
		m_link->setDebug(v ? 1 : 0);

		gsLogicObject_setDebugBricks(m_link->getSensors());
		gsLogicObject_setDebugBricks(m_link->getControllers());
		gsLogicObject_setDebugBricks(m_link->getActuators());
	}

}



bool gsLogicObject::isDebug(void)
{
	return m_link ? m_link->getDebug() != 0 : false;
}



gkString gsLogicObject::getName(void)
{
	if (m_link && m_link->getObject())
		return m_link->getObject()->getName();

	return "Undefined";
}


gkLogicSensor* gsLogicObject::getSensor(const gkString& name)
{
	if (m_link)
	{
		gkLogicSensor* brick = m_link->findSensor(name);
		if (brick)
			return brick;
	}
	return 0;

}


gkLogicController* gsLogicObject::getController(const gkString& name)
{
	if (m_link)
	{
		gkLogicController* brick = m_link->findController(name);
		if (brick)
			return brick;
	}
	return 0;
}


gkLogicActuator* gsLogicObject::getActuator(const gkString& name)
{
	if (m_link)
	{
		gkLogicActuator* brick = m_link->findActuator(name);
		if (brick)
			return brick;
	}
	return 0;
}


bool gsLogicObject::hasBrick(gkLogicLink* link, const gkString& name)
{
	if (link->findSensor(name)) return true;
	if (link->findController(name)) return true;
	if (link->findActuator(name)) return true;
	return false;
}


gkString gsLogicObject::getUniqueName(void)
{
	if (m_link) return gkString("Brick #") + Ogre::StringConverter::toString(++m_incr);
	return "Null";
}



template<typename W, typename B>
static void gsLogicObject_getBricks(gsArray<W, B> &dest, gkLogicLink::BrickList& lnks)
{
	if (!lnks.empty())
	{
		utListIterator<gkLogicLink::BrickList> iter(lnks);
		while (iter.hasMoreElements())
		{
			B* lnk = (B*)iter.getNext();
			dest.push(lnk);
		}
	}
}



gsArray<gsSensor, gkLogicSensor> gsLogicObject::getSensors()
{
	gsArray<gsSensor, gkLogicSensor> arr;
	if (m_link)
		gsLogicObject_getBricks(arr, m_link->getSensors());
	return arr;
}



gsArray<gsController, gkLogicController> gsLogicObject::getControllers()
{
	gsArray<gsController, gkLogicController> arr;
	if (m_link)
		gsLogicObject_getBricks(arr, m_link->getControllers());
	return arr;
}


gsArray<gsActuator, gkLogicActuator> gsLogicObject::getActuators()
{
	gsArray<gsActuator, gkLogicActuator> arr;
	if (m_link)
		gsLogicObject_getBricks(arr, m_link->getActuators());
	return arr;
}





//
//  Base Brick impl
//




gsBrick::gsBrick() : m_brick(0), m_isLocal(false), m_listener(0)
{
}


gsBrick::gsBrick(gkLogicBrick* brk)  : m_brick(brk), m_isLocal(false), m_listener(0)
{
}



gsBrick::~gsBrick()
{
	if (m_isLocal)
	{
		// todo: .... til then be careful
		//gkLogicLink *lnk = m_brick->getLink();
		//lnk->unlink(m_brick);
	}

	if (m_listener)
		delete m_listener;
}


gsBrick::EventListener::EventListener(gsListenerMode mode, gkLogicBrick* par, gsSelf self, gsFunction fnc)
	:    m_evt(new gkLuaEvent(self, fnc)), m_parent(par)
{
	if (mode == LM_EVT_OVERIDE)
		m_mode = gkLogicBrick::Listener::OVERIDE;
	else
		m_mode = gkLogicBrick::Listener::AND;

}


gsBrick::EventListener::~EventListener()
{
	if (m_parent)
		m_parent->setListener(0);

	delete m_evt;
}


bool gsBrick::EventListener::executeEvent(gkLogicBrick* brick)
{
	m_evt->beginCall();

	bool result = false;

	if (!m_evt->call(result))
	{
		delete m_evt;
		m_evt = 0;
		if (m_parent)
			m_parent->setListener(0);
	}
	return result;
}




gkString gsBrick::getName(void)
{
	return m_brick ? m_brick->getName() : "";
}

gkGameObject* gsBrick::getOwner(){
	return get()->getObject();
}


#define GS_IMPLEMENT_DEFAULT_BRICK(base, cls) \
    cls::~cls() { }\
    cls::cls() {}\
    cls::cls(gkLogicBrick *brk) : base(brk) {}

#define GS_IMPLEMENT_USER_BRICK(base, cls, wrap) \
    GS_IMPLEMENT_DEFAULT_BRICK(base, cls)\
    cls::cls(gsLogicObject *parent, const gkString& name) {\
        m_isLocal = true;\
        m_brick = create<wrap>(parent, name);\
    }

#define GS_IMPLEMENT_USER_BRICK_GENERIC(base, cls) \
    GS_IMPLEMENT_DEFAULT_BRICK(base, gs##cls)\
    gs##cls::gs##cls(gsLogicObject *parent, const gkString& name) {\
        m_isLocal = true;\
        m_brick = create<gk##cls>(parent, name);\
    }



//
//  Base Sensor impl
//

GS_IMPLEMENT_DEFAULT_BRICK(gsBrick, gsSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, ActuatorSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, AlwaysSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, KeyboardSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, CollisionSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, DelaySensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, MessageSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, MouseSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, NearSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, PropertySensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, RaySensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, RadarSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, RandomSensor);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsSensor, TouchSensor);



//
//  Base Controller impl
//

GS_IMPLEMENT_DEFAULT_BRICK(gsBrick, gsController);


void gsController::link(gsSensor* sens)
{
	if (sens && sens->get())
		sens->get()->link(get());
}



void gsController::link(gsActuator* act)
{
	if (act && act->get())
		get()->link(act->get());
}

gkLogicSensor* gsController::getSensor(gkString sensorName) {
	return get()->getLink()->findSensor(sensorName);
}

gkLogicActuator* gsController::getActuator(gkString actuatorName){
	return get()->getLink()->findActuator(actuatorName);
}



GS_IMPLEMENT_USER_BRICK_GENERIC(gsController, LogicOpController);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsController, ScriptController);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsController, ExpressionController);


gsController* getCurrentController(void)
{
	gkScriptController* ctx = static_cast<gkScriptController*>(gkScriptController::getCurrent());
	if (ctx) return new gsScriptController(ctx);

	gkExpressionController* ctxExp = static_cast<gkExpressionController*>(gkExpressionController::getCurrent());
	if (ctx) return new gsExpressionController(ctxExp);

	return 0;
}

void  gsActionActuator::play(void) {
	get()->playAction();
}

float gsSoundActuator::getPitch() {
#ifdef OGREKIT_OPENAL_SOUND
	return get()->getProperties().m_pitch;
#else
	return -1;
#endif
}

void gsSoundActuator::setPitch(float pitch){
#ifdef OGREKIT_OPENAL_SOUND
	get()->getProperties().m_pitch=pitch;
	get()->updateProperties();
#endif
}

float gsSoundActuator::getVolume() {
#ifdef OGREKIT_OPENAL_SOUND
	return get()->getProperties().m_volume;
#else
	return -1;
#endif
}

void gsSoundActuator::setVolume(float volume){
#ifdef OGREKIT_OPENAL_SOUND
	if (volume > 1) volume=1;
	else if (volume < 0) volume=0;
	get()->getProperties().m_volume=volume;
	get()->updateProperties();
#endif
}


//
//  Base Actuator impl
//

GS_IMPLEMENT_DEFAULT_BRICK(gsBrick, gsActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, ActionActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, EditObjectActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, GameActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, MessageActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, MotionActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, ParentActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, PropertyActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, RandomActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, SceneActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, SoundActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, StateActuator);
GS_IMPLEMENT_USER_BRICK_GENERIC(gsActuator, VisibilityActuator);
