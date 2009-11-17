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
#ifndef __BLENDER_VIEW3D__H__
#define __BLENDER_VIEW3D__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ListBase.h"

#undef near
#undef far

namespace Blender {


    // ---------------------------------------------- //
    class View3D
    {
    public:
        SpaceLink *next;
        SpaceLink *prev;
        int spacetype;
        float blockscale;
        ScrArea *area;
        short blockhandler[8];
        float viewmat[4][4];
        float viewinv[4][4];
        float persmat[4][4];
        float persinv[4][4];
        float winmat1[4][4];
        float viewmat1[4][4];
        vec4f viewquat;
        float dist;
        float zfac;
        int lay_used;
        short persp;
        short view;
        Object *camera;
        Object *ob_centre;
        BGpic *bgpic;
        View3D *localvd;
        bInvalidHandle *ri;
        bInvalidHandle *retopo_view_data;
        bInvalidHandle *depths;
        char ob_centre_bone[32];
        short drawtype;
        short localview;
        int lay;
        int layact;
        short scenelock;
        short around;
        short camzoom;
        char pivot_last;
        char pad1;
        float lens;
        float grid;
        float gridview;
        float pixsize;
        float near;
        float far;
        float camdx;
        float camdy;
        vec3f ofs;
        vec3f cursor;
        short gridlines;
        short viewbut;
        short gridflag;
        short modeselect;
        short menunr;
        short texnr;
        short twtype;
        short twmode;
        short twflag;
        short twdrawflag;
        float twmat[4][4];
        float clip[4][4];
        BoundBox *clipbb;
        ListBase afterdraw;
        short zbuf;
        short transp;
        short xray;
        short flag;
        short flag2;
        short gridsubdiv;
        short keyflags;
        char ndofmode;
        char ndoffilter;
        void *properties_storage;
        bGPdata *gpd;
        vec4f lviewquat;
        short lpersp;
        short lview;
        short pad5;
        short pad6;
    };
}


#endif//__BLENDER_VIEW3D__H__
