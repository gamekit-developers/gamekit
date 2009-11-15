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
#ifndef __BLENDER_TEX__H__
#define __BLENDER_TEX__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"
#include "blender_ImageUser.h"

namespace Blender {


    // ---------------------------------------------- //
    class Tex
    {
    public:
        ID id;
        float noisesize;
        float turbul;
        float bright;
        float contrast;
        float rfac;
        float gfac;
        float bfac;
        float filtersize;
        float mg_H;
        float mg_lacunarity;
        float mg_octaves;
        float mg_offset;
        float mg_gain;
        float dist_amount;
        float ns_outscale;
        float vn_w1;
        float vn_w2;
        float vn_w3;
        float vn_w4;
        float vn_mexp;
        short vn_distm;
        short vn_coltype;
        short noisedepth;
        short noisetype;
        short noisebasis;
        short noisebasis2;
        short imaflag;
        short flag;
        short type;
        short stype;
        float cropxmin;
        float cropymin;
        float cropxmax;
        float cropymax;
        short xrepeat;
        short yrepeat;
        short extend;
        short fie_ima;
        int len;
        int frames;
        int offset;
        int sfra;
        float checkerdist;
        float nabla;
        float norfac;
        ImageUser iuser;
        bNodeTree *nodetree;
        Ipo *ipo;
        Image *ima;
        PluginTex *plugin;
        ColorBand *coba;
        EnvMap *env;
        PreviewImage *preview;
        char use_nodes;
        char pad[7];
    };
}


#endif//__BLENDER_TEX__H__
