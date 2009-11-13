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
        short uvcalc_mapdir;
        short uvcalc_mapalign;
        short uvcalc_flag;
        short pad2;
        float select_thresh;
        ImagePaintSettings imapaint;
        float clean_thresh;
        char retopo_mode;
        char retopo_paint_tool;
        char line_div;
        char ellipse_div;
        char retopo_hotspot;
        char pad4[3];
    };
}


#endif//__BLENDER_TOOLSETTINGS__H__
