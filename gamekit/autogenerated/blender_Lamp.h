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
#ifndef __BLENDER_LAMP__H__
#define __BLENDER_LAMP__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"
#include "blender_ScriptLink.h"

namespace Blender {


    // ---------------------------------------------- //
    class Lamp
    {
    public:
        ID id;
        short type;
        short mode;
        short colormodel;
        short totex;
        float r;
        float g;
        float b;
        float k;
        float energy;
        float dist;
        float spotsize;
        float spotblend;
        float haint;
        float att1;
        float att2;
        float clipsta;
        float clipend;
        float shadspotsize;
        float bias;
        float soft;
        short bufsize;
        short samp;
        short buffers;
        short filtertype;
        char bufflag;
        char buftype;
        short ray_samp;
        short ray_sampy;
        short ray_sampz;
        short ray_samp_type;
        short area_shape;
        float area_size;
        float area_sizey;
        float area_sizez;
        short texact;
        short shadhalostep;
        int YF_numphotons;
        int YF_numsearch;
        short YF_phdepth;
        short YF_useqmc;
        short YF_bufsize;
        short YF_pad;
        float YF_causticblur;
        float YF_ltradius;
        float YF_glowint;
        float YF_glowofs;
        short YF_glowtype;
        short YF_pad2;
        MTex *mtex[10];
        Ipo *ipo;
        ScriptLink scriptlink;
    };
}


#endif//__BLENDER_LAMP__H__
