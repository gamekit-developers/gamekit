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
