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
#ifndef __BLENDER_MATERIAL__H__
#define __BLENDER_MATERIAL__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"
#include "blender_ListBase.h"
#include "blender_ScriptLink.h"

namespace Blender {


    // ---------------------------------------------- //
    class Material
    {
    public:
        ID id;
        short colormodel;
        short flag;
        float r;
        float g;
        float b;
        float specr;
        float specg;
        float specb;
        float mirr;
        float mirg;
        float mirb;
        float ambr;
        float ambb;
        float ambg;
        float amb;
        float emit;
        float ang;
        float spectra;
        float ray_mirror;
        float alpha;
        float ref;
        float spec;
        float zoffs;
        float add;
        float translucency;
        float fresnel_mir;
        float fresnel_mir_i;
        float fresnel_tra;
        float fresnel_tra_i;
        float filter;
        float tx_limit;
        float tx_falloff;
        short ray_depth;
        short ray_depth_tra;
        short har;
        char seed1;
        char seed2;
        float gloss_mir;
        float gloss_tra;
        short samp_gloss_mir;
        short samp_gloss_tra;
        float adapt_thresh_mir;
        float adapt_thresh_tra;
        float aniso_gloss_mir;
        float dist_mir;
        short fadeto_mir;
        short shade_flag;
        int mode;
        int mode_l;
        short flarec;
        short starc;
        short linec;
        short ringc;
        float hasize;
        float flaresize;
        float subsize;
        float flareboost;
        float strand_sta;
        float strand_end;
        float strand_ease;
        float strand_surfnor;
        float strand_min;
        float strand_widthfade;
        char strand_uvname[32];
        float sbias;
        float lbias;
        float shad_alpha;
        int septex;
        char rgbsel;
        char texact;
        char pr_type;
        char use_nodes;
        short pr_back;
        short pr_lamp;
        short pad4;
        short ml_flag;
        short diff_shader;
        short spec_shader;
        float roughness;
        float refrac;
        vec4f param;
        float rms;
        float darkness;
        short texco;
        short mapto;
        ColorBand *ramp_col;
        ColorBand *ramp_spec;
        char rampin_col;
        char rampin_spec;
        char rampblend_col;
        char rampblend_spec;
        short ramp_show;
        short pad3;
        float rampfac_col;
        float rampfac_spec;
        MTex *mtex[18];
        bNodeTree *nodetree;
        Ipo *ipo;
        Group *group;
        PreviewImage *preview;
        float friction;
        float fh;
        float reflect;
        float fhdist;
        float xyfrict;
        short dynamode;
        short pad2;
        vec3f sss_radius;
        vec3f sss_col;
        float sss_error;
        float sss_scale;
        float sss_ior;
        float sss_colfac;
        float sss_texfac;
        float sss_front;
        float sss_back;
        short sss_flag;
        short sss_preset;
        float YF_ar;
        float YF_ag;
        float YF_ab;
        float YF_dscale;
        float YF_dpwr;
        int YF_dsmp;
        int YF_preset;
        int YF_djit;
        ScriptLink scriptlink;
        ListBase gpumaterial;
    };
}


#endif//__BLENDER_MATERIAL__H__
