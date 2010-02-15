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
#include "gkLogicManager.h"
#include "gkLogicLink.h"
#include "gkLogicDispatcher.h"
#include "gkLogicActuator.h"
#include "gkLogicController.h"
#include "gkMouseSensor.h"



gkLogicManager::gkLogicManager()
{
    m_dispatchers = new gkAbstractDispatcherPtr[DIS_MAX];
    m_dispatchers[DIS_CONSTANT] = new gkConstantDispatch;
    m_dispatchers[DIS_MOUSE]    = new gkMouseDispatch;
}


gkLogicManager::~gkLogicManager()
{
    for (int i = 0; i < DIS_MAX; ++ i)
        delete m_dispatchers[i];

    delete []m_dispatchers;
    m_dispatchers = 0;

    clear();
}


void gkLogicManager::clear(void)
{
    if (m_dispatchers) {
        getDispatcher(DIS_CONSTANT).clear();
        getDispatcher(DIS_MOUSE).clear();
    }

    if (!m_links.empty()) {
        gkLogicLink *node = m_links.begin();
        while (node) {
            gkLogicLink *tmp = node;
            node = node->getNext();
            delete tmp;
        }
        m_links.clear();
    }
}

gkLogicLink *gkLogicManager::createLink(void)
{
    gkLogicLink *link = new gkLogicLink();
    m_links.push_back(link);
    return link;
}



void gkLogicManager::update(gkScalar delta)
{
    if (m_dispatchers) {
        for (int i = 0; i < DIS_MAX; ++ i)
            m_dispatchers[i]->dispatch();
    }

    while (!m_sensorStack.empty()) {
        m_sensorStack.top()->dispatch();
        m_sensorStack.pop();
    }

    utStackIterator<ControllerStack> constant(m_controllerStack);
    while (constant.hasMoreElements()) {
        gkLogicController *cont = constant.getNext();
        GK_ASSERT(cont->isGate());
        cont->relay();
    }

    while (!m_actuatorStack.empty()) {
        gkLogicActuator *act = m_actuatorStack.top();
        act->execute();
        act->setActive(false);
        m_actuatorStack.pop();
    }
}


GK_IMPLEMENT_SINGLETON(gkLogicManager)
