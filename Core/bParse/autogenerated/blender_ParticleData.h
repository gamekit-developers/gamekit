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
#ifndef __BLENDER_PARTICLEDATA__H__
#define __BLENDER_PARTICLEDATA__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ParticleKey.h"

namespace Blender {


    // ---------------------------------------------- //
    class ParticleData
    {
    public:
        Object *stick_ob;
        ParticleKey state;
        ParticleKey prev_state;
        HairKey *hair;
        ParticleKey *keys;
        vec4f i_rot;
        vec4f r_rot;
        vec3f r_ave;
        vec3f r_ve;
        vec4f fuv;
        float foffset;
        float time;
        float lifetime;
        float dietime;
        float bank;
        float size;
        float sizemul;
        int num;
        int num_dmcache;
        int pad;
        int totkey;
        int bpi;
        short flag;
        short alive;
        short loop;
        short rt2;
    };
}


#endif//__BLENDER_PARTICLEDATA__H__
