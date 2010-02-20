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
#include "blender.h"
#include "gkBlenderDefines.h"
#include "gkLogicLoader.h"
#include "gkLogicLink.h"
#include "gkLogicManager.h"
#include "gkLogicSensor.h"
#include "gkLogicController.h"
#include "gkLogicActuator.h"
#include "gkAlwaysSensor.h"
#include "gkCollisionSensor.h"
#include "gkMouseSensor.h"
#include "gkLogicOpController.h"
#include "gkScriptController.h"
#include "gkMotionActuator.h"
#include "gkStateActuator.h"

#include "gkLuaManager.h"
#include "gkGameObject.h"
#include "gkRigidBody.h"

gkLogicLoader::gkLogicLoader()
{
}

gkLogicLoader::~gkLogicLoader()
{
}

void gkLogicLoader::resolveLinks(void)
{
    utListIterator<CreatedList> oblist(m_createdLinks);
    while (oblist.hasMoreElements())
    {
        gkLogicLink *link = oblist.getNext();

        utListIterator<ResolveObjectList> missing(m_missing);
        while (missing.hasMoreElements())
        {
            ResolveObject &ob = missing.getNext();

            if (ob.cont && ob.lsens)
            {
                gkLogicController *lc = link->findController(ob.cont->name);
                if (lc)
                {
                    ob.lsens->link(lc);
                    lc->link(ob.lsens);
                }
            }

            if (ob.act && ob.lcont)
            {
                gkLogicActuator *la = link->findActuator(ob.act->name);
                if (la)
                    ob.lcont->link(la);
            }

        }
    }
}


void gkLogicLoader::convertObject(Blender::Object *bobj, gkGameObject *gobj)
{
    GK_ASSERT(gobj && bobj);

    if (!bobj->sensors.first && !bobj->controllers.first && !bobj->actuators.first)
        return;

    gkLogicManager *rlm = gkLogicManager::getSingletonPtr();
    gkLogicLink *lnk = rlm->createLink();
    m_createdLinks.push_back(lnk);
    lnk->setState(bobj->init_state ? bobj->init_state : bobj->state);


    for (Blender::bActuator *bact = (Blender::bActuator*)bobj->actuators.first; bact; bact = bact->next)
    {
        gkLogicActuator *la = 0;
        switch (bact->type)
        {
        case ACT_STATE:
            {
                gkStateActuator *sa = new gkStateActuator(gobj, lnk, bact->name);
                la = sa;

                Blender::bStateActuator *bst = (Blender::bStateActuator*)bact->data;
                
                int op = gkStateActuator::OP_NILL;


                switch (bst->type)
                {
                case 0: {op = gkStateActuator::OP_CPY; break;}
                case 1: {op = gkStateActuator::OP_ADD; break;}
                case 2: {op = gkStateActuator::OP_SUB; break;}
                case 3: {op = gkStateActuator::OP_INV; break;}
                }

                sa->setOp(op);
                sa->setMask(bst->mask);

            }break;

        case ACT_OBJECT:
            {
                gkMotionActuator *ma = new gkMotionActuator(gobj, lnk, bact->name);
                la = ma;

                Blender::bObjectActuator *objact = (Blender::bObjectActuator*)bact->data;
                ma->setRotation(gkVector3(objact->drot.x, objact->drot.y, objact->drot.z), (objact->flag & ACT_DROT_LOCAL) != 0);
                ma->setTranslation(gkVector3(objact->dloc.x, objact->dloc.y, objact->dloc.z), (objact->flag & ACT_DLOC_LOCAL) != 0);
                ma->setForce(gkVector3(objact->forceloc.x, objact->forceloc.y, objact->forceloc.z), (objact->flag & ACT_FORCE_LOCAL) != 0);
                ma->setTorque(gkVector3(objact->forcerot.x, objact->forcerot.y, objact->forcerot.z), (objact->flag & ACT_TORQUE_LOCAL) != 0);

                ma->setLinearVelocity(gkVector3(objact->linearvelocity.x, objact->linearvelocity.y, objact->linearvelocity.z), 
                        (objact->flag & ACT_LIN_VEL_LOCAL) != 0);
                ma->setAngularVelocity(gkVector3(objact->angularvelocity.x, objact->angularvelocity.y, objact->angularvelocity.z), 
                    (objact->flag & ACT_ANG_VEL_LOCAL) != 0);

                ma->setIncrementalVelocity((objact->flag & ACT_ADD_LIN_VEL) != 0);
                ma->setDamping(gkScalar(objact->damping) / 1000.f);
            }
            break;
        }

        if (la)
            lnk->push(la);

    }

    int uuid = 0;

    for (Blender::bController *bcont = (Blender::bController*)bobj->controllers.first; bcont; bcont = bcont->next)
    {
        // sometimes controller names are not unique
        // if user lookup on the controller name is not undefined,
        // this will need to change, I'm trying to avoid 
        // lookup maps by storing a ptr to the controller  
        sprintf(bcont->name, "%s_%i", bcont->name, (uuid++));


        gkLogicController *lc = 0;
        switch (bcont->type)
        {
        case CONT_LOGIC_OR:
        case CONT_LOGIC_XOR:
        case CONT_LOGIC_AND:
        case CONT_LOGIC_NAND:
        case CONT_LOGIC_NOR:
        case CONT_LOGIC_XNOR:
            {
                gkLogicOpController *ac = new gkLogicOpController(gobj, lnk, bcont->name);
                if (bcont->type == CONT_LOGIC_OR)
                    ac->setOp(gkLogicOpController::OP_OR);
                else if (bcont->type == CONT_LOGIC_XOR)
                    ac->setOp(gkLogicOpController::OP_XOR);
                else if (bcont->type == CONT_LOGIC_AND)
                    ac->setOp(gkLogicOpController::OP_AND);
                else if (bcont->type == CONT_LOGIC_NAND)
                    ac->setOp(gkLogicOpController::OP_NAND);
                else if (bcont->type == CONT_LOGIC_NOR)
                    ac->setOp(gkLogicOpController::OP_NOR);
                else if (bcont->type == CONT_LOGIC_XNOR)
                    ac->setOp(gkLogicOpController::OP_XNOR);
                lc = ac;
            }
            break;
        case CONT_PYTHON:
            {
                gkScriptController *sc = new gkScriptController(gobj, lnk, bcont->name);
                lc = sc;

                Blender::bPythonCont *pcon = (Blender::bPythonCont*)bcont->data;
                sc->setModule(false);

                if (pcon->text)
                {
                    gkLuaManager &lua = gkLuaManager::getSingleton();
                    if (lua.hasScript(GKB_IDNAME(pcon->text)))
                        sc->setScript(lua.getScript(GKB_IDNAME(pcon->text)));
                    else
                        sc->setScript(lua.create(GKB_IDNAME(pcon->text)));
                }

            }break;
        

        }

        if (lc)
        {



            for (int i = 0; i < bcont->totlinks; ++i)
            {
                Blender::bActuator *a = bcont->links[i];
                if (a)
                {
                    gkLogicActuator *la = lnk->findActuator(a->name);
                    if (la)
                        lc->link(la);
                    else
                    {
                        ResolveObject robj = {0, 0, 0, 0};
                        robj.lcont = lc;
                        robj.act = a;
                        m_missing.push_back(robj);
                    }

                }
            }

            lc->setMask(bcont->state_mask);
            lc->setPriority((bcont->flag & CONT_PRIO) != 0);
            lnk->push(lc);
        }
    }


    for (Blender::bSensor *bsen = (Blender::bSensor*)bobj->sensors.first; bsen; bsen = bsen->next)
    {
        gkLogicSensor *ls = 0;
        switch (bsen->type)
        {
        case SENS_COLLISION:
            {
                // tell the gk world we want to collect collsion information
                gkRigidBody *body = gobj->getAttachedBody();
                if (body) body->setFlags(body->getFlags() | gkRigidBody::RBF_CONTACT_INFO);

                gkCollisionSensor *col = new gkCollisionSensor(gobj, lnk, bsen->name);
                ls = col;
                Blender::bCollisionSensor *cse = (Blender::bCollisionSensor*)bsen->data;

                if (cse->mode & SENS_COLLISION_MATERIAL)
                    col->setMaterial(cse->materialName);
                else
                    col->setProperty(cse->name);

            }break;

        case SENS_ALWAYS:
            {
                gkAlwaysSensor *asn = new gkAlwaysSensor(gobj, lnk, bsen->name);
                ls = asn;
            }
            break;
        case SENS_MOUSE:
            {
                gkMouseSensor *ms = new gkMouseSensor(gobj, lnk, bsen->name);
                ls = ms;

                Blender::bMouseSensor *mse = (Blender::bMouseSensor*)bsen->data;

                int type = 0;
                if (mse->type == BL_SENS_MOUSE_LEFT_BUTTON)
                    type = gkMouseSensor::MOUSE_LEFT;
                else if (mse->type == BL_SENS_MOUSE_MIDDLE_BUTTON)
                    type = gkMouseSensor::MOUSE_MIDDLE;
                else if (mse->type == BL_SENS_MOUSE_RIGHT_BUTTON)
                    type = gkMouseSensor::MOUSE_RIGHT;
                else if (mse->type == BL_SENS_MOUSE_WHEEL_UP)
                    type = gkMouseSensor::MOUSE_WHEEL_UP;
                else if (mse->type == BL_SENS_MOUSE_WHEEL_DOWN)
                    type = gkMouseSensor::MOUSE_WHEEL_DOWN;
                else if (mse->type == BL_SENS_MOUSE_MOVEMENT)
                    type = gkMouseSensor::MOUSE_MOTION;
                else if (mse->type == BL_SENS_MOUSE_MOUSEOVER)
                    type = gkMouseSensor::MOUSE_MOUSE_OVER;
                else if (mse->type == BL_SENS_MOUSE_MOUSEOVER_ANY)
                    type = gkMouseSensor::MOUSE_MOUSE_OVER_ANY;

                ms->setType(type);
            }
            break;
        }

        if (ls)
        {
            for (int i = 0; i < bsen->totlinks; ++i)
            {
                Blender::bController *c = bsen->links[i];
                if (c)
                {
                    gkLogicController *lc = lnk->findController(c->name);
                    if (lc)
                    {
                        ls->link(lc);
                        lc->link(ls);
                    }
                    else
                    {
                        ResolveObject robj = {0, 0, 0, 0};
                        robj.lsens = ls;
                        robj.cont = c;
                        m_missing.push_back(robj);
                    }
                }
            }
            ls->setDetector(bsen->level != 0);
            ls->setTap(bsen->tap != 0);
            ls->setFrequency(bsen->freq);
            ls->invert(bsen->invert != 0);
            int pulse = gkLogicSensor::PULSE_NONE;

            if (bsen->pulse & SENS_PULSE_REPEAT)
                pulse |= gkLogicSensor::PULSE_POSITIVE;
            if (bsen->pulse & SENS_NEG_PULSE_MODE)
                pulse |= gkLogicSensor::PULSE_NEGATIVE;


            // special case, use tap (most likely more will need this behavior)
            if (bsen->type == SENS_COLLISION && pulse == gkLogicSensor::PULSE_NONE)
                ls->setTap(true);

            ls->setMode(pulse);
            lnk->push(ls);
        }
    }
}
