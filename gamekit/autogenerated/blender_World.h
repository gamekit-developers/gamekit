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
#ifndef __BLENDER_WORLD__H__
#define __BLENDER_WORLD__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"
#include "blender_ScriptLink.h"

namespace Blender {


    // ---------------------------------------------- //
    class World
    {
    public:
        ID id;
        short colormodel;
        short totex;
        short texact;
        short mistype;
        float horr;
        float horg;
        float horb;
        float hork;
        float zenr;
        float zeng;
        float zenb;
        float zenk;
        float ambr;
        float ambg;
        float ambb;
        float ambk;
        int fastcol;
        float exposure;
        float exp;
        float range;
        float linfac;
        float logfac;
        float gravity;
        float activityBoxRadius;
        short skytype;
        short mode;
        short occlusionRes;
        short physicsEngine;
        short ticrate;
        short maxlogicstep;
        short physubstep;
        short maxphystep;
        float misi;
        float miststa;
        float mistdist;
        float misthi;
        float starr;
        float starg;
        float starb;
        float stark;
        float starsize;
        float starmindist;
        float stardist;
        float starcolnoise;
        short dofsta;
        short dofend;
        short dofmin;
        short dofmax;
        float aodist;
        float aodistfac;
        float aoenergy;
        float aobias;
        short aomode;
        short aosamp;
        short aomix;
        short aocolor;
        float ao_adapt_thresh;
        float ao_adapt_speed_fac;
        float ao_approx_error;
        float ao_approx_correction;
        short ao_samp_method;
        short ao_gather_method;
        short ao_approx_passes;
        short pad1;
        float *aosphere;
        float *aotables;
        Ipo *ipo;
        MTex *mtex[18];
        PreviewImage *preview;
        ScriptLink scriptlink;
    };
}


#endif//__BLENDER_WORLD__H__
