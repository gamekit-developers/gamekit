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
#ifndef __BLENDER_MULTIRESLEVEL__H__
#define __BLENDER_MULTIRESLEVEL__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class MultiresLevel
    {
    public:
        MultiresLevel *next;
        MultiresLevel *prev;
        MultiresFace *faces;
        MultiresColFace *colfaces;
        MultiresEdge *edges;
        char *edge_boundary_states;
        ListBase *vert_edge_map;
        ListBase *vert_face_map;
        bInvalidHandle *map_mem;
        int totvert;
        int totface;
        int totedge;
        int pad;
        MVert *verts;
    };
}


#endif//__BLENDER_MULTIRESLEVEL__H__
