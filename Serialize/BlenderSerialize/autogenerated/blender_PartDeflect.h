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
#ifndef __BLENDER_PARTDEFLECT__H__
#define __BLENDER_PARTDEFLECT__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class PartDeflect
    {
    public:
        short deflect;
        short forcefield;
        short flag;
        short falloff;
        float pdef_damp;
        float pdef_rdamp;
        float pdef_perm;
        float pdef_frict;
        float pdef_rfrict;
        float f_strength;
        float f_power;
        float f_dist;
        float f_damp;
        float maxdist;
        float mindist;
        float maxrad;
        float minrad;
        float f_power_r;
        float pdef_sbdamp;
        float pdef_sbift;
        float pdef_sboft;
        float clump_fac;
        float clump_pow;
        float kink_freq;
        float kink_shape;
        float kink_amp;
        float free_end;
        float tex_nabla;
        short tex_mode;
        short kink;
        short kink_axis;
        short rt2;
        Tex *tex;
        bInvalidHandle *rng;
        float f_noise;
        int seed;
    };
}


#endif//__BLENDER_PARTDEFLECT__H__
