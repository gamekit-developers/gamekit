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
#include "gkVariableNode.h"
#include "gkVariable.h"
#include "gkGameObject.h"


using namespace Ogre;



gkVariableNode::gkVariableNode(gkLogicTree *parent, size_t id):
        gkLogicNode(parent, id), m_debug(false), m_prop(0), m_varName("")
{
    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_BOOL);
    ADD_ISOCK(m_sockets[1], this, gkLogicSocket::ST_VARIABLE);
    ADD_OSOCK(m_sockets[2], this, gkLogicSocket::ST_VARIABLE);
    m_sockets[0].setValue(true);
    m_sockets[1].setValue(0.f);
    m_sockets[2].setValue(0.f);
}


void gkVariableNode::initialize()
{
    if (!m_varName.empty())
    {
        gkGameObject *ob = m_sockets[0].getGameObject();
        if (ob != 0)
        {
            if (!ob->hasVariable(m_varName))
                m_prop = ob->createVariable(m_varName, m_debug);
            else
                m_prop = ob->getVariable(m_varName);

            if (m_prop && m_sockets[0].getValueBool())
                m_prop->setValue(m_sockets[1].getValueVariable());
        }
    }
}


bool gkVariableNode::evaluate(gkScalar tick)
{
    if (!m_prop) return false;


    m_sockets[2].setValue(m_prop);
    if (!m_sockets[0].isLinked() && !m_sockets[1].isLinked())
    {
        // variable will not change
        return false;
    }

    if (m_sockets[0].getValueBool())
        m_prop->setValue(m_sockets[1].getValueVariable());
    return false;
}


VariableOpNode::VariableOpNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, id),
        m_function(MTH_NO_FUNC), m_deg(false), m_prop(0), m_varName("")
{
    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_BOOL);
    ADD_ISOCK(m_sockets[1], this, gkLogicSocket::ST_VARIABLE);
    ADD_OSOCK(m_sockets[2], this, gkLogicSocket::ST_VARIABLE);
    m_sockets[0].setValue(true);
    m_sockets[1].setValue(0.f);
    m_sockets[2].setValue(0.f);
}


void VariableOpNode::initialize()
{
    if (!m_varName.empty())
    {
        gkGameObject *ob = m_sockets[0].getGameObject();
        if (ob != 0)
            m_prop = ob->getVariable(m_varName);
    }
}


bool VariableOpNode::evaluate(gkScalar tick)
{
    if (!m_prop || !m_sockets[0].getValueBool())
        return false;

    gkScalar a = m_prop->getValueReal();
    gkScalar b = m_sockets[1].getValueReal();
    gkScalar d = 0.0;

    switch (m_function)
    {
    case MTH_NO_FUNC:
        d = 0.0;
        break;
    case MTH_ADD:
        d = a + b;
        break;
    case MTH_SUBTRACT:
        d = a - b;
        break;
    case MTH_MULTIPLY:
        d = a * b;
        break;
    case MTH_DIVIDE:
        d = (b == 0.0) ? 0.0 : a / b;
        break;
    case MTH_SINE:
        {
            d = gkMath::Sin(gkRadian(a));
            if (m_deg) d = d * gkMath::fRad2Deg;
        }
        break;
    case MTH_COSINE:
        {
            d = gkMath::Cos(gkRadian(a));
            if (m_deg) d = d * gkMath::fRad2Deg;
        }
        break;
    case MTH_TANGENT:
        {
            d = gkMath::Tan(gkRadian(a));
            if (m_deg) d = d * gkMath::fRad2Deg;
        }
        break;
    case MTH_ARCSINE:
        {
            if (a <= 1 && a >= -1)
                d = gkMath::ASin(a).valueRadians();
            else
                d = 0.0;

            if (m_deg) d = d * gkMath::fRad2Deg;

        }
        break;
    case MTH_ARCCOSINE:
        {
            a = gkRadian(a).valueRadians();
            b = gkRadian(b).valueRadians();
            if (a <= 1 && a >= -1)
                d = gkMath::ACos(a).valueRadians();
            else
                d = 0.0;

            if (m_deg) d = d * gkMath::fRad2Deg;
        }
        break;
    case MTH_ARCTANGENT:
        {
            a = gkRadian(a).valueRadians();
            d = gkMath::ATan(a).valueRadians();

            if (m_deg) d = d * gkMath::fRad2Deg;
        }
        break;
    case MTH_POWER:
        d = a > 0 ? gkMath::Pow(a, b) : 0.0;
        break;
    case MTH_LOGARITHM:
        {
            if (a > 0 && b > 0)
                d = gkMath::Log(a) / gkMath::Log(b);
            else d = 0.0;
        }
        break;
    case MTH_MINIMUM:
        {
            d = (a < b) ? a : b;
        }
        break;
    case MTH_MAXIMUM:
        {
            d = (a > b) ? a : b;
        }
        break;
    case MTH_ROUND:
        {
            gkScalar base = gkMath::Floor(a);
            gkScalar dec = base - a;
            d = dec > 0.5 ? base + 1 : base;
        }
        break;
    case MTH_LESS_THAN:
        {
            d = (a < b) ? 1.0 : 0.0;
        }
        break;
    case MTH_GREATER_THAN:
        {
            d = (a > b) ? 1.0 : 0.0;
        }
        break;
    case MTH_SQRT:
        {
            d = gkMath::Sqrt(a);
        }
        break;
    case MTH_SQR:
        {
            d = a * a;
        }
        break;
    default:
        break;
    }

    if (gkFuzzy(d))     d = 0.0;
    if (gkNan(d))       d = 0.0;
    if (!gkFinite(d))   d = 0.0;
    m_prop->setValue(d);
    m_sockets[2].setValue(m_prop);
    return false;
}
