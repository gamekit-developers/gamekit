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
#include "gkObjectNode.h"
#include "gkEngine.h"
#include "gkGameObjectGroup.h"
#include "gkScene.h"


using namespace Ogre;



gkObjectNode::gkObjectNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, NT_OBJECT, id), m_otherName(""), m_func(OB_FUNC_NONE)
{
    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_BOOL);
    ADD_ISOCK(m_sockets[1], this, gkLogicSocket::ST_VECTOR);
    ADD_ISOCK(m_sockets[2], this, gkLogicSocket::ST_EULER);

    ADD_OSOCK(m_sockets[3], this, gkLogicSocket::ST_BOOL);
    ADD_OSOCK(m_sockets[4], this, gkLogicSocket::ST_VECTOR);
    ADD_OSOCK(m_sockets[5], this, gkLogicSocket::ST_EULER);

    m_sockets[0].setValue(true);
    m_sockets[1].setValue(gkVector3::ZERO);
    m_sockets[2].setValue(gkVector3::ZERO);
    m_sockets[3].setValue(false);
    m_sockets[4].setValue(gkVector3::ZERO);
    m_sockets[5].setValue(gkVector3::ZERO);
}


bool gkObjectNode::evaluate(gkScalar tick)
{
    m_sockets[3].setValue(m_sockets[0].getValueBool());
    return m_sockets[0].getValueBool();
}


void gkObjectNode::initialize()
{
    if (!m_otherName.empty())
    {
        // look in this instance
        if (m_object && m_object->isInstance())
        {
            //gkGameObjectGroupInstance *inst = m_object->getGroupInstance();

            //if (inst->hasObject(m_otherName))
            //    m_other = inst->getObject(m_otherName);
        }

        // look at global scope
        if (!m_other)
        {
            if (m_object)
            {
                gkScene *scene = m_object->getOwner();
                GK_ASSERT(scene);
                m_other = scene->getObject(m_otherName);
            }
        }
    }
}


void gkObjectNode::update(gkScalar tick)
{
    gkGameObject *ob = m_other ? m_other : m_object;
    if (!ob)
        return;

    if (m_sockets[1].isLinked())
        ob->setPosition(m_sockets[1].getValueVector3());
    else
        m_sockets[4].setValue(ob->getPosition());

    if (m_sockets[2].isLinked())
    {
        gkQuaternion q = gkMathUtils::getQuatFromEuler(m_sockets[2].getValueVector3(), true);
        ob->setOrientation(q);
        m_sockets[5].setValue(m_sockets[2].getValueVector3());
    }
    else
        m_sockets[5].setValue(gkMathUtils::getEulerFromQuat(ob->getOrientation(), true));
}
