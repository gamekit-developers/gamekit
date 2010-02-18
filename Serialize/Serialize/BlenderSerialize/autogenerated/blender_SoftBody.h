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
#ifndef __BLENDER_SOFTBODY__H__
#define __BLENDER_SOFTBODY__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class SoftBody
    {
    public:
        ParticleSystem *particles;
        int totpoint;
        int totspring;
        bInvalidHandle *bpoint;
        bInvalidHandle *bspring;
        char pad;
        char msg_lock;
        short msg_value;
        float nodemass;
        char namedVG_Mass[32];
        float grav;
        float mediafrict;
        float rklimit;
        float physics_speed;
        float goalspring;
        float goalfrict;
        float mingoal;
        float maxgoal;
        float defgoal;
        short vertgroup;
        char namedVG_Softgoal[32];
        short fuzzyness;
        float inspring;
        float infrict;
        char namedVG_Spring_K[32];
        int sfra;
        int efra;
        int interval;
        short local;
        short solverflags;
        SBVertex **keys;
        int totpointkey;
        int totkey;
        float secondspring;
        float colball;
        float balldamp;
        float ballstiff;
        short sbc_mode;
        short aeroedge;
        short minloops;
        short maxloops;
        short choke;
        short solver_ID;
        short plastic;
        short springpreload;
        bInvalidHandle *scratch;
        float shearstiff;
        float inpush;
        PointCache *pointcache;
    };
}


#endif//__BLENDER_SOFTBODY__H__
