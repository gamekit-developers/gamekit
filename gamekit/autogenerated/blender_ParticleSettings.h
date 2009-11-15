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
#ifndef __BLENDER_PARTICLESETTINGS__H__
#define __BLENDER_PARTICLESETTINGS__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"

namespace Blender {


    // ---------------------------------------------- //
    class ParticleSettings
    {
    public:
        ID id;
        int flag;
        short type;
        short from;
        short distr;
        short phystype;
        short rotmode;
        short avemode;
        short reactevent;
        short draw;
        short draw_as;
        short draw_size;
        short childtype;
        short draw_step;
        short ren_step;
        short hair_step;
        short keys_step;
        short adapt_angle;
        short adapt_pix;
        short disp;
        short omat;
        short interpolation;
        short rotfrom;
        short integrator;
        short kink;
        short kink_axis;
        short nbetween;
        short boidneighbours;
        short bb_align;
        short bb_uv_split;
        short bb_anim;
        short bb_split_offset;
        float bb_tilt;
        float bb_rand_tilt;
        float bb_offset[2];
        short simplify_flag;
        short simplify_refsize;
        float simplify_rate;
        float simplify_transition;
        float simplify_viewport;
        float sta;
        float end;
        float lifetime;
        float randlife;
        float timetweak;
        float jitfac;
        float keyed_time;
        float eff_hair;
        float rt;
        int totpart;
        int userjit;
        int grid_res;
        float normfac;
        float obfac;
        float randfac;
        float partfac;
        float tanfac;
        float tanphase;
        float reactfac;
        float avefac;
        float phasefac;
        float randrotfac;
        float randphasefac;
        float mass;
        float size;
        float randsize;
        float reactshape;
        vec3f acc;
        float dragfac;
        float brownfac;
        float dampfac;
        float length;
        float abslength;
        float randlength;
        int child_nbr;
        int ren_child_nbr;
        float parents;
        float childsize;
        float childrandsize;
        float childrad;
        float childflat;
        float childspread;
        float clumpfac;
        float clumppow;
        float kink_amp;
        float kink_freq;
        float kink_shape;
        float rough1;
        float rough1_size;
        float rough2;
        float rough2_size;
        float rough2_thres;
        float rough_end;
        float rough_end_shape;
        float branch_thres;
        float draw_line[2];
        float max_vel;
        float max_lat_acc;
        float max_tan_acc;
        float average_vel;
        float banking;
        float max_bank;
        float groundz;
        float boidfac[8];
        char boidrule[8];
        Group *dup_group;
        Group *eff_group;
        Object *dup_ob;
        Object *bb_ob;
        Ipo *ipo;
        PartDeflect *pd;
        PartDeflect *pd2;
    };
}


#endif//__BLENDER_PARTICLESETTINGS__H__
