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
#include "gsCommon.h"
%}

// internal binding utils don't bother 
%ignore OGRE_KIT_WRAP_CLASS_COPY_CTOR;
%ignore OGRE_KIT_WRAP_CLASS;
%ignore OGRE_KIT_INTERNAL_CAST;
%ignore OGRE_KIT_WRAP_BASE_COPY_CTOR;
%ignore OGRE_KIT_WRAP_CLASS_DEF_CTOR;
%ignore OGRE_KIT_TEMPLATE_CAST;
%ignore OGRE_KIT_TEMPLATE_NEW;
%ignore OGRE_KIT_TEMPLATE_NEW_INLINE;

#define GK_INLINE
#define gkScalar float

// dependent types
%include "gsTypes.i"

%include "gsCommon.h"
%include "../../gkSerialize.h"
%include "gsUtils.i"
%include "gsMath.i"
%include "gsCore.i"
// sub modules
%include "gsAI.i"
%include "gsAnimation.i"
%include "gsBricks.i"
%include "gsConstraints.i"
%include "gsNetwork.i"
%include "gsNodes.i"
%include "gsParticles.i"
%include "gsPhysics.i"
%include "gsSound.i"
%include "gsThread.i"
%include "gsTypeConverters.i"
%include "gsScript.i"

#ifdef OGREKIT_USE_PROCESSMANAGER
%include "gsProcess.i"
#endif
