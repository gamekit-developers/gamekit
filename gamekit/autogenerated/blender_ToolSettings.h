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
#ifndef __BLENDER_TOOLSETTINGS__H__
#define __BLENDER_TOOLSETTINGS__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ImagePaintSettings.h"
#include "blender_ParticleEditSettings.h"

namespace Blender {


    // ---------------------------------------------- //
    class ToolSettings
    {
    public:
        short cornertype;
        short editbutflag;
        float jointrilimit;
        float degr;
        short step;
        short turn;
        float extr_offs;
        float doublimit;
        short segments;
        short rings;
        short vertices;
        short unwrapper;
        float uvcalc_radius;
        float uvcalc_cubesize;
        float uvcalc_margin;
        float pad;
        short uvcalc_mapdir;
        short uvcalc_mapalign;
        short uvcalc_flag;
        short autoik_chainlen;
        ImagePaintSettings imapaint;
        ParticleEditSettings particle;
        float select_thresh;
        float clean_thresh;
        char retopo_mode;
        char retopo_paint_tool;
        char line_div;
        char ellipse_div;
        char retopo_hotspot;
        char multires_subdiv_type;
        short skgen_resolution;
        float skgen_threshold_internal;
        float skgen_threshold_external;
        float skgen_length_ratio;
        float skgen_length_limit;
        float skgen_angle_limit;
        float skgen_correlation_limit;
        float skgen_symmetry_limit;
        float skgen_retarget_angle_weight;
        float skgen_retarget_length_weight;
        float skgen_retarget_distance_weight;
        short skgen_options;
        char skgen_postpro;
        char skgen_postpro_passes;
        char skgen_subdivisions[3];
        char skgen_multi_level;
        Object *skgen_template;
        char bone_sketching;
        char bone_sketching_convert;
        char skgen_subdivision_number;
        char skgen_retarget_options;
        char skgen_retarget_roll;
        char skgen_side_string[8];
        char skgen_num_string[8];
        char edge_mode;
        char pad3[2];
    };
}


#endif//__BLENDER_TOOLSETTINGS__H__
