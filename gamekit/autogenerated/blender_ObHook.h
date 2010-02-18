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
#ifndef __BLENDER_OBHOOK__H__
#define __BLENDER_OBHOOK__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class ObHook
    {
    public:
        ObHook *next;
        ObHook *prev;
        Object *parent;
        float parentinv[4][4];
        float mat[4][4];
        vec3f cent;
        float falloff;
        char name[32];
        int *indexar;
        int totindex;
        int curindex;
        short type;
        short active;
        float force;
    };
}


#endif//__BLENDER_OBHOOK__H__
