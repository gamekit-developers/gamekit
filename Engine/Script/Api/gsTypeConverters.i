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


%{
static int gsGetProperty(lua_State *L, const gsProperty& prop)
{
	int SWIG_arg= 0;
	switch (prop.getType())
	{
	case PROP_BOOL:
		lua_pushboolean(L, prop.toBool()); 
		SWIG_arg++;
		break;
	case PROP_NUMBER:
		lua_pushnumber(L, prop.toNumber()); 
		SWIG_arg++;
		break;
	case PROP_STRING:
		{
			gkString str = prop.toString();
			lua_pushlstring(L, str.c_str(), str.size());
			SWIG_arg++;
			break;
		}
	}
	return SWIG_arg;
}

#define GS_TYPEOF(a, b)   ((a) && typeid(*a) == typeid(b))
#define GS_TYPE_RET(GK, GS, T) if (GS_TYPEOF(GK, gk##T)) { SWIG_NewPointerObj(L, GS, SWIGTYPE_p_##gs##T, 1); return 1; }


static int gsCreateSensor(lua_State *L, gsSensor *obj)
{
	gkLogicSensor *ob = obj->get();

	GS_TYPE_RET(ob, obj,  ActuatorSensor);
	GS_TYPE_RET(ob, obj,  AlwaysSensor);
	GS_TYPE_RET(ob, obj,  KeyboardSensor);
	GS_TYPE_RET(ob, obj,  CollisionSensor);
	GS_TYPE_RET(ob, obj,  DelaySensor);
	GS_TYPE_RET(ob, obj,  MessageSensor);
	GS_TYPE_RET(ob, obj,  MouseSensor);
	GS_TYPE_RET(ob, obj,  NearSensor);
	GS_TYPE_RET(ob, obj,  PropertySensor);
	GS_TYPE_RET(ob, obj,  RaySensor);
	GS_TYPE_RET(ob, obj,  RadarSensor);
	GS_TYPE_RET(ob, obj,  RandomSensor);

	SWIG_NewPointerObj(L, obj, SWIGTYPE_p_gsSensor, 1); 
	return 1;
}

static int gsCreateController(lua_State *L, gsController *obj)
{
	gkLogicController *ob = obj->get();

	GS_TYPE_RET(ob, obj,  LogicOpController);
	GS_TYPE_RET(ob, obj,  ScriptController);

	SWIG_NewPointerObj(L, obj, SWIGTYPE_p_gsController, 1); 
	return 1;
}

static int gsCreateActuator(lua_State *L, gsActuator *obj)
{
	gkLogicActuator *ob = obj->get();

	GS_TYPE_RET(ob, obj,  ActionActuator);
	GS_TYPE_RET(ob, obj,  EditObjectActuator);
	GS_TYPE_RET(ob, obj,  GameActuator);
	GS_TYPE_RET(ob, obj,  MessageActuator);
	GS_TYPE_RET(ob, obj,  MotionActuator);
	GS_TYPE_RET(ob, obj,  ParentActuator);
	GS_TYPE_RET(ob, obj,  PropertyActuator);
	GS_TYPE_RET(ob, obj,  RandomActuator);
	GS_TYPE_RET(ob, obj,  SceneActuator);
	GS_TYPE_RET(ob, obj,  SoundActuator);
	GS_TYPE_RET(ob, obj,  StateActuator);
	GS_TYPE_RET(ob, obj,  VisibilityActuator);

	SWIG_NewPointerObj(L, obj, SWIGTYPE_p_gsActuator, 1); 
	return 1;
}

static int gsCreateObject(lua_State *L, gsGameObject *obj)
{
	gkGameObject *ob = obj->get();

	GS_TYPE_RET(ob, obj,  Camera);
	GS_TYPE_RET(ob, obj,  Entity);
	GS_TYPE_RET(ob, obj,  Light);
	GS_TYPE_RET(ob, obj,  Skeleton);

	SWIG_NewPointerObj(L, obj, SWIGTYPE_p_gsGameObject, 1); 
	return 1;
}


%}
