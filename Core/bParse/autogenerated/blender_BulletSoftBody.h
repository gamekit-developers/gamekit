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
#ifndef __BLENDER_BULLETSOFTBODY__H__
#define __BLENDER_BULLETSOFTBODY__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class BulletSoftBody
    {
    public:
        int flag;
        float linStiff;
        float angStiff;
        float volume;
        int viterations;
        int piterations;
        int diterations;
        int citerations;
        float kSRHR_CL;
        float kSKHR_CL;
        float kSSHR_CL;
        float kSR_SPLT_CL;
        float kSK_SPLT_CL;
        float kSS_SPLT_CL;
        float kVCF;
        float kDP;
        float kDG;
        float kLF;
        float kPR;
        float kVC;
        float kDF;
        float kMT;
        float kCHR;
        float kKHR;
        float kSHR;
        float kAHR;
        int collisionflags;
        int numclusteriterations;
        float welding;
        float margin;
    };
}


#endif//__BLENDER_BULLETSOFTBODY__H__
