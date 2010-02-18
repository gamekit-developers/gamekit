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
#ifndef __BLENDER_PARTEFF__H__
#define __BLENDER_PARTEFF__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class PartEff
    {
    public:
        PartEff *next;
        PartEff *prev;
        short type;
        short flag;
        short buttype;
        short stype;
        short vertgroup;
        short userjit;
        float sta;
        float end;
        float lifetime;
        int totpart;
        int totkey;
        int seed;
        float normfac;
        float obfac;
        float randfac;
        float texfac;
        float randlife;
        vec3f force;
        float damp;
        float nabla;
        float vectsize;
        float maxlen;
        float pad;
        vec3f defvec;
        vec4f mult;
        vec4f life;
        short child[4];
        short mat[4];
        short texmap;
        short curmult;
        short staticstep;
        short omat;
        short timetex;
        short speedtex;
        short flag2;
        short flag2neg;
        short disp;
        short vertgroup_v;
        char vgroupname[32];
        char vgroupname_v[32];
        float imat[4][4];
        bInvalidHandle *keys;
        Group *group;
    };
}


#endif//__BLENDER_PARTEFF__H__
