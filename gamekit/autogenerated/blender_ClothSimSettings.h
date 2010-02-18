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
#ifndef __BLENDER_CLOTHSIMSETTINGS__H__
#define __BLENDER_CLOTHSIMSETTINGS__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class ClothSimSettings
    {
    public:
        bInvalidHandle *cache;
        float mingoal;
        float Cdis;
        float Cvi;
        float gravity;
        vec3f unknown;
        float dt;
        float mass;
        float structural;
        float shear;
        float bending;
        float max_bend;
        float max_struct;
        float max_shear;
        float avg_spring_len;
        float timescale;
        float maxgoal;
        float eff_force_scale;
        float eff_wind_scale;
        float sim_time_old;
        float defgoal;
        float goalspring;
        float goalfrict;
        int stepsPerFrame;
        int flags;
        int preroll;
        int maxspringlen;
        short solver_type;
        short vgroup_bend;
        short vgroup_mass;
        short vgroup_struct;
        short presets;
        short pad;
        int pad2;
    };
}


#endif//__BLENDER_CLOTHSIMSETTINGS__H__
