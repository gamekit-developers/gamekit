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
#ifndef __BLENDER_SPACEIMASEL__H__
#define __BLENDER_SPACEIMASEL__H__


// -------------------------------------------------- //
#include "blender_Common.h"

namespace Blender {


    // ---------------------------------------------- //
    class SpaceImaSel
    {
    public:
        SpaceLink *next;
        SpaceLink *prev;
        int spacetype;
        float blockscale;
        ScrArea *area;
        char title[28];
        int fase;
        short mode;
        short subfase;
        short mouse_move_redraw;
        short imafase;
        short mx;
        short my;
        short dirsli;
        short dirsli_lines;
        short dirsli_sx;
        short dirsli_ey;
        short dirsli_ex;
        short dirsli_h;
        short imasli;
        short fileselmenuitem;
        short imasli_sx;
        short imasli_ey;
        short imasli_ex;
        short imasli_h;
        short dssx;
        short dssy;
        short dsex;
        short dsey;
        short desx;
        short desy;
        short deex;
        short deey;
        short fssx;
        short fssy;
        short fsex;
        short fsey;
        short dsdh;
        short fsdh;
        short fesx;
        short fesy;
        short feex;
        short feey;
        short infsx;
        short infsy;
        short infex;
        short infey;
        short dnsx;
        short dnsy;
        short dnw;
        short dnh;
        short fnsx;
        short fnsy;
        short fnw;
        short fnh;
        char fole[128];
        char dor[128];
        char file[128];
        char dir[128];
        bInvalidHandle *firstdir;
        bInvalidHandle *firstfile;
        int topdir;
        int totaldirs;
        int hilite;
        int topfile;
        int totalfiles;
        float image_slider;
        float slider_height;
        float slider_space;
        short topima;
        short totalima;
        short curimax;
        short curimay;
        bInvalidHandle *first_sel_ima;
        bInvalidHandle *hilite_ima;
        short total_selected;
        short ima_redraw;
        int pad2;
        bInvalidHandle *cmap;
        void (*returnfunc)();
        void *arg1;
    };
}


#endif//__BLENDER_SPACEIMASEL__H__
