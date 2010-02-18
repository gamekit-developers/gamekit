/* Copyright (C) 2006 Charlie C
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
// Auto generated from makesdna dna.c
#ifndef __BLENDER_SCENE__H__
#define __BLENDER_SCENE__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_AudioData.h"
#include "blender_GameFraming.h"
#include "blender_ID.h"
#include "blender_ListBase.h"
#include "blender_RenderData.h"
#include "blender_ScriptLink.h"
#include "blender_SculptData.h"

namespace Blender {


    // ---------------------------------------------- //
    class Scene
    {
    public:
        ID id;
        Object *camera;
        World *world;
        Scene *set;
        Image *ima;
        ListBase base;
        Base *basact;
        vec3f cursor;
        vec3f twcent;
        vec3f twmin;
        vec3f twmax;
        int lay;
        float editbutsize;
        short selectmode;
        short proportional;
        short prop_mode;
        short automerge;
        short pad5;
        short pad6;
        short autokey_mode;
        short use_nodes;
        bNodeTree *nodetree;
        void *ed;
        Radio *radio;
        GameFraming framing;
        ToolSettings *toolsettings;
        RenderData r;
        AudioData audio;
        ScriptLink scriptlink;
        ListBase markers;
        ListBase transform_spaces;
        short jumpframe;
        short snap_mode;
        short snap_flag;
        short snap_target;
        bInvalidHandle *theDag;
        short dagisvalid;
        short dagflags;
        short pad4;
        short recalc;
        SculptData sculptdata;
        int frame_step;
        int pad;
    };
}


#endif//__BLENDER_SCENE__H__
