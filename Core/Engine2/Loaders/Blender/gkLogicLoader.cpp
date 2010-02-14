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
#include "gkMouseSensor.h"
#include "gkLogicOpController.h"
#include "gkMotionActuator.h"


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


    for (Blender::bActuator *bact = (Blender::bActuator*)bobj->actuators.first; bact; bact = bact->next)
    {
        gkLogicActuator *la = 0;
        switch (bact->type)
        {
        case ACT_OBJECT:
            {
                gkMotionActuator *ma = new gkMotionActuator(gobj, bact->name);
                la = ma;

                Blender::bObjectActuator *objact = (Blender::bObjectActuator*)bact->data;
                ma->setRot(gkVector3(objact->drot.x, objact->drot.y, objact->drot.z), (objact->flag & ACT_DROT_LOCAL) != 0);
                ma->setLoc(gkVector3(objact->dloc.x, objact->dloc.y, objact->dloc.z), (objact->flag & ACT_DLOC_LOCAL) != 0);

            }
            break;
        }

        if (la)
            lnk->push(la);

    }

    int uuid = 0;

    for (Blender::bController *bcont = (Blender::bController*)bobj->controllers.first; bcont; bcont = bcont->next)
    {
        // might be a bug in blender, but 
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
                gkLogicOpController *ac = new gkLogicOpController(gobj, bcont->name);
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
            lnk->push(lc);
        }
    }


    for (Blender::bSensor *bsen = (Blender::bSensor*)bobj->sensors.first; bsen; bsen = bsen->next)
    {
        gkLogicSensor *ls = 0;
        switch (bsen->type)
        {
        case SENS_ALWAYS:
            {
                gkAlwaysSensor *asn = new gkAlwaysSensor(gobj, bsen->name);
                ls = asn;
            }
            break;
        case SENS_MOUSE:
            {
                gkMouseSensor *ms = new gkMouseSensor(gobj, bsen->name);
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

            ls->setTap(bsen->tap != 0);
            ls->setFrequency(bsen->freq);
            ls->invert(bsen->invert != 0);
            int pulse = gkLogicSensor::PULSE_NONE;

            if (bsen->pulse & SENS_PULSE_REPEAT)
                pulse |= gkLogicSensor::PULSE_POSITIVE;
            if (bsen->pulse & SENS_NEG_PULSE_MODE)
                pulse |= gkLogicSensor::PULSE_NEGATIVE;

            ls->setMode(pulse);
            lnk->push(ls);
        }
    }
}