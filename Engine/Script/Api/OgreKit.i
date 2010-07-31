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
%module OgreKit
%{
#include "OgreKitCommon.h"
%}

// internal binding utils don't bother 
%ignore OGRE_KIT_WRAP_CLASS_COPY_CTOR;
%ignore OGRE_KIT_WRAP_CLASS;
%ignore OGRE_KIT_INTERNAL_CAST;
%ignore OGRE_KIT_WRAP_BASE_COPY_CTOR;
%ignore OGRE_KIT_WRAP_CLASS_DEF_CTOR;


%include "OgreKitTypes.i"
%include "OgreKitCommon.h"


// dependent types
%include "OgreKitUtils.i"
%include "OgreKitMath.i"
%include "OgreKitCore.i"


// sub modules
%include "OgreKitAI.i"
%include "OgreKitAnimation.i"
%include "OgreKitBricks.i"
%include "OgreKitConstraints.i"
%include "OgreKitNetwork.i"
%include "OgreKitNodes.i"
%include "OgreKitParticles.i"
%include "OgreKitPhysics.i"
%include "OgreKitSound.i"
%include "OgreKitThread.i"


%template(GameObjectPtr)    OgreKit::Pointer<OgreKit::GameObject>;
%template(ScenePtr)         OgreKit::Pointer<OgreKit::Scene>;
%template(EntityPtr)        OgreKit::Pointer<OgreKit::Entity>;
%template(CameraPtr)        OgreKit::Pointer<OgreKit::Camera>;
%template(LightPtr)         OgreKit::Pointer<OgreKit::Light>;
