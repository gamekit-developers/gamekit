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
#include "gkMathNode.h"
#include "gkMathUtils.h"


using namespace Ogre;



gkMathNode::gkMathNode(gkLogicTree *parent, size_t id) :
        gkLogicNode(parent, id),
        m_function(MTH_NO_FUNC)
{
    ADD_ISOCK(m_sockets[0], this, gkLogicSocket::ST_REAL);
    ADD_ISOCK(m_sockets[1], this, gkLogicSocket::ST_REAL);
    ADD_OSOCK(m_sockets[2], this, gkLogicSocket::ST_REAL);

    m_sockets[0].setValue(0.f);
    m_sockets[1].setValue(0.f);
    m_sockets[2].setValue(0.f);
}


void gkMathNode::update(gkScalar tick)
{
    gkScalar a = m_sockets[0].getValueReal();
    gkScalar b = m_sockets[1].getValueReal();
    gkScalar d = 0.0;

    switch (m_function)
    {
    case MTH_NO_FUNC:
        d = gkScalar(0.0);
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
        if (b == 0.0)
            b = 1.0;
        d = a / b;
        break;

    case MTH_SINE:
        {
            if (m_sockets[0].isLinked())
                d = gkMath::Sin(gkRadian(a));
            else
                d = gkMath::Sin(gkRadian(b));
        }
        break;
    case MTH_COSINE:
        {
            if (m_sockets[0].isLinked())
                d = gkMath::Cos(gkRadian(a));
            else
                d = gkMath::Cos(gkRadian(b));
        }
        break;
    case MTH_TANGENT:
        {
            if (m_sockets[0].isLinked())
                d = gkMath::Tan(gkRadian(a));
            else
                d = gkMath::Tan(gkRadian(b));
        }
        break;
    case MTH_ARCSINE:
        {
            if (m_sockets[0].isLinked())
            {
                if (a <= 1 && a >= -1)
                    d = gkMath::ASin(a).valueDegrees();
                else
                    d = 0.0;
            }
            else
            {
                if (b <= 1 && b >= -1)
                    d = gkMath::ASin(b).valueDegrees();
                else
                    d = 0.0;
            }
        }
        break;
    case MTH_ARCCOSINE:
        {
            a = gkRadian(a).valueRadians();
            b = gkRadian(b).valueRadians();

            if (m_sockets[0].isLinked())
            {
                if (a <= 1 && a >= -1)
                    d = gkMath::ACos(a).valueDegrees();
                else
                    d = 0.0;
            }
            else
            {
                if (b <= 1 && b >= -1)
                    d = gkMath::ACos(b).valueDegrees();
                else
                    d = 0.0;
            }
        }
        break;
    case MTH_ARCTANGENT:
        {
            a = gkRadian(a).valueRadians();
            b = gkRadian(b).valueRadians();

            if (m_sockets[0].isLinked())
                d = gkMath::ATan(a).valueDegrees();
            else
                d = gkMath::ATan(b).valueDegrees();
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
            if (m_sockets[0].isLinked())
            {
                gkScalar base = gkMath::Floor(a);
                gkScalar dec = base - a;
                d = dec > 0.5 ? base + 1 : base;
            }
            else
            {
                gkScalar base = gkMath::Floor(b);
                gkScalar dec = base - b;
                d = dec > 0.5 ? base + 1 : base;
            }
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
    default:
        break;
    }

    if (gkFuzzy(d))     d = 0.0;
    if (gkNan(d))       d = 0.0;
    if (!gkFinite(d))   d = 0.0;

    m_sockets[2].setValue(d);
}
