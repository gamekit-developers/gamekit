/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkLogicCommon_h_
#define _gkLogicCommon_h_

#include "gkCommon.h"
#include "OgreStdHeaders.h"

#define MAX_ROT         360.0
#define MAX_TRANSLATION 100000.0

class gkLogicNode;
class gkLogicTree;
class gkGameObject;

typedef enum gkMotionTypes
{
	MT_NONE = -1,
	MT_ROTATION,
	MT_TRANSLATION,
	MT_SCALE,
	MT_FORCE,
	MT_TORQUE,
	MT_LINV,
	MT_ANGV,
} gkMotionTypes;

typedef enum gkMathFunction
{
	MTH_NO_FUNC = -1,
	MTH_ADD,
	MTH_SUBTRACT,
	MTH_MULTIPLY,
	MTH_DIVIDE,
	MTH_SINE,
	MTH_COSINE,
	MTH_TANGENT,
	MTH_ARCSINE,
	MTH_ARCCOSINE,
	MTH_ARCTANGENT,
	MTH_POWER,
	MTH_LOGARITHM,
	MTH_MINIMUM,
	MTH_MAXIMUM,
	MTH_ROUND,
	MTH_LESS_THAN,
	MTH_GREATER_THAN,
	MTH_SQRT,
	MTH_SQR,
} gkMathFunction;

typedef enum gkBoolStatement
{
	CMP_NULL = 0,
	CMP_TRUE,
	CMP_FALSE,
	CMP_AND,
	CMP_OR,
	CMP_NOT,
	CMP_EQUALS,
	CMP_NOT_EQUAL,
	CMP_GREATER,
	CMP_LESS,
	CMP_GTHAN,
	CMP_LTHAN,
	CMP_FIND,
} gkBoolStatement;

typedef enum gkObjectFunction
{
	OB_FUNC_NONE = 0,
	OB_FUNC_DESTROY,
	OB_FUNC_ADD,
} gkObjectFunction;


#endif//_gkLogicCommon_h_
