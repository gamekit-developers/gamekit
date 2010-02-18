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
#ifndef __BLENDER_BNODE__H__
#define __BLENDER_BNODE__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ListBase.h"
#include "blender_rctf.h"

namespace Blender {


    // ---------------------------------------------- //
    class bNode
    {
    public:
        bNode *next;
        bNode *prev;
        bNode *new_node;
        char name[32];
        char username[32];
        short type;
        short flag;
        short done;
        short level;
        short lasty;
        short menunr;
        short stack_index;
        short nr;
        ListBase inputs;
        ListBase outputs;
        ID *id;
        void *storage;
        float locx;
        float locy;
        float width;
        float miniwidth;
        short custom1;
        short custom2;
        float custom3;
        float custom4;
        short need_exec;
        short exec;
        rctf totr;
        rctf butr;
        rctf prvr;
        bInvalidHandle *preview;
        bInvalidHandle *typeinfo;
    };
}


#endif//__BLENDER_BNODE__H__
