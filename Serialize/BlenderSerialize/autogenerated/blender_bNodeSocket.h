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
#ifndef __BLENDER_BNODESOCKET__H__
#define __BLENDER_BNODESOCKET__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_bNodeStack.h"

namespace Blender {


    // ---------------------------------------------- //
    class bNodeSocket
    {
    public:
        bNodeSocket *next;
        bNodeSocket *prev;
        bNodeSocket *new_sock;
        char name[32];
        bNodeStack ns;
        short type;
        short flag;
        short limit;
        short stack_index;
        short intern;
        short stack_index_ext;
        int pad1;
        float locx;
        float locy;
        int own_index;
        int to_index;
        bNodeSocket *tosock;
        bNodeLink *link;
    };
}


#endif//__BLENDER_BNODESOCKET__H__
