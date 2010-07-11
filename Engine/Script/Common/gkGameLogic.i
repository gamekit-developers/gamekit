/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
%module GameLogic
%{
#include "gksDebugger.h"
#include "gksGameObject.h"
#include "gksInput.h"
#include "gksMath.h"
#include "gksObject.h"
#include "gksPointer.h"
#include "gksRayTest.h"
#include "gksScene.h"
%}

// rename types for exposed scripts. 
%rename(Debugger)       gksDebugger;
%rename(Mouse)          gksMouse;
%rename(Keyboard)       gksKeyboard;
%rename(Property)       gksProperty;
%rename(Scene)          gksScene;
%rename(Loadable)       gksObject;
%rename(GameObject)     gksGameObject;
%rename(Entity)         gksEntity;
%rename(RayTest)        gksRayTest;
%rename(Vector3)        gksVector3;
%rename(Quaternion)     gksQuaternion;
%rename(GameObjectPtr)  gksPointer<gksGameObject>;
%rename(DebugPtr)       gksPointer<gksDebugger>;
%rename(ScenePtr)       gksPointer<gksScene>;
%rename(EntityPtr)      gksPointer<gksEntity>;
%rename(RayTestPtr)     gksPointer<gksRayTest>;
%#ifdef SWIG_V8
%rename(getAttribue)    __getitem__;
%rename(setAttribue)    __setitem__;
%#endif

// property type map
%include "gkProperty.i"

%include "gksConstants.h"
%include "gkKeyConstants.i"
%include "gksDebugger.h"
%include "gksInput.h"
%include "gksMath.h"
%include "gksObject.h"
%include "gksGameObject.h"
%include "gksPointer.h"
%include "gksRayTest.h"
%include "gksScene.h"
%include "gksDebugger.h"

%template(GameObjectPtr)    gksPointer<gksGameObject>;
%template(ScenePtr)         gksPointer<gksScene>;
%template(DebugPtr)         gksPointer<gksDebugger>;
%template(EntityPtr)        gksPointer<gksEntity>;
%template(RayTestPtr)       gksPointer<gksRayTest>;

