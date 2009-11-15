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
#ifndef __BLENDER_USERDEF__H__
#define __BLENDER_USERDEF__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ColorBand.h"
#include "blender_ListBase.h"
#include "blender_SolidLight.h"

namespace Blender {


    // ---------------------------------------------- //
    class UserDef
    {
    public:
        int flag;
        int dupflag;
        int savetime;
        char tempdir[160];
        char fontdir[160];
        char renderdir[160];
        char textudir[160];
        char plugtexdir[160];
        char plugseqdir[160];
        char pythondir[160];
        char sounddir[160];
        char yfexportdir[160];
        short versions;
        short vrmlflag;
        int gameflags;
        int wheellinescroll;
        int uiflag;
        int language;
        short userpref;
        short viewzoom;
        short console_buffer;
        short console_out;
        int mixbufsize;
        int fontsize;
        short encoding;
        short transopts;
        short menuthreshold1;
        short menuthreshold2;
        char fontname[256];
        ListBase themes;
        short undosteps;
        short undomemory;
        short gp_manhattendist;
        short gp_euclideandist;
        short gp_eraser;
        short gp_settings;
        short tb_leftmouse;
        short tb_rightmouse;
        SolidLight light[3];
        short tw_hotspot;
        short tw_flag;
        short tw_handlesize;
        short tw_size;
        int textimeout;
        int texcollectrate;
        int memcachelimit;
        int prefetchframes;
        short frameserverport;
        short pad_rot_angle;
        short obcenter_dia;
        short rvisize;
        short rvibright;
        short recent_files;
        short smooth_viewtx;
        short glreslimit;
        short ndof_pan;
        short ndof_rotate;
        short curssize;
        short pad1;
        char pad[8];
        char versemaster[160];
        char verseuser[160];
        float glalphaclip;
        short autokey_mode;
        short autokey_flag;
        ColorBand coba_weight;
    };
}


#endif//__BLENDER_USERDEF__H__
