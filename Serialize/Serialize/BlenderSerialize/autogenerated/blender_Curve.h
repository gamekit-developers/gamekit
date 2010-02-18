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
#ifndef __BLENDER_CURVE__H__
#define __BLENDER_CURVE__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_CharInfo.h"
#include "blender_ID.h"
#include "blender_ListBase.h"

namespace Blender {


    // ---------------------------------------------- //
    class Curve
    {
    public:
        ID id;
        BoundBox *bb;
        ListBase nurb;
        ListBase disp;
        Object *bevobj;
        Object *taperobj;
        Object *textoncurve;
        Ipo *ipo;
        bInvalidHandle *path;
        Key *key;
        Material **mat;
        ListBase bev;
        vec3f loc;
        vec3f size;
        vec3f rot;
        int texflag;
        short pathlen;
        short totcol;
        short flag;
        short bevresol;
        float width;
        float ext1;
        float ext2;
        short resolu;
        short resolv;
        short resolu_ren;
        short resolv_ren;
        int pad2;
        short len;
        short lines;
        short pos;
        short spacemode;
        float spacing;
        float linedist;
        float shear;
        float fsize;
        float wordspace;
        float ulpos;
        float ulheight;
        float xof;
        float yof;
        float linewidth;
        char *str;
        char family[24];
        VFont *vfont;
        VFont *vfontb;
        VFont *vfonti;
        VFont *vfontbi;
        int sepchar;
        int totbox;
        int actbox;
        int pad;
        TextBox *tb;
        int selstart;
        int selend;
        CharInfo *strinfo;
        CharInfo curinfo;
    };
}


#endif//__BLENDER_CURVE__H__
