/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
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
#include "OgreAnimation.h"
#include "OgreEntity.h"
#include "gkAnimationNode.h"
#include "gkLogicSocket.h"
#include "gkEntity.h"
#include "gkEngine.h"
#include "gkLogger.h"

using namespace Ogre;



gkAnimationNode::gkAnimationNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, id), m_anim(""), m_func(AF_LOOP)
{

    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_BOOL);
    ADD_ISOCK(m_sockets[1], this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(m_sockets[2], this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(m_sockets[3], this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(m_sockets[4], this, gkLogicSocket::ST_REAL);

    m_sockets[0].setValue(true);
    m_sockets[1].setValue(0.f);
    m_sockets[2].setValue(0.f);
    m_sockets[3].setValue(0.f);
    m_sockets[4].setValue(0.f);
}


bool gkAnimationNode::evaluate(gkScalar tick)
{
    return m_sockets[0].getValueBool();
}


void gkAnimationNode::update(gkScalar tick)
{
    gkGameObject *ob = m_sockets[0].getGameObject();
    if (ob == 0)
        ob = m_object;

    if (ob != 0)
    {
        // else no anim yet
        if (ob->getType() != GK_ENTITY)
            return;


        gkEntity *ent = ob->getEntity();
        if (ent->isLoaded())
        {
            gkAnimProperties& props = ent->getAnimationProperties();
            props.state = m_func == AF_LOOP ? gkAnimProperties::APS_LOOP : gkAnimProperties::APS_PLAY_STOP;
            props.anim  = m_anim;
            props.blend = m_sockets[1].getValueReal();
            props.start = m_sockets[2].getValueReal();
            props.end   = m_sockets[3].getValueReal();
            props.speed = m_sockets[4].getValueReal();

            ent->updateAnimations();
        }
    }
}
