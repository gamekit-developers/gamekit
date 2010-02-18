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
#ifndef __BLENDER_SCRAREA__H__
#define __BLENDER_SCRAREA__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ListBase.h"
#include "blender_ScriptLink.h"
#include "blender_rcti.h"

namespace Blender {


    // ---------------------------------------------- //
    class ScrArea
    {
    public:
        ScrArea *next;
        ScrArea *prev;
        ScrVert *v1;
        ScrVert *v2;
        ScrVert *v3;
        ScrVert *v4;
        bScreen *full;
        float winmat[4][4];
        rcti totrct;
        rcti headrct;
        rcti winrct;
        short headwin;
        short win;
        short headertype;
        char spacetype;
        char butspacetype;
        short winx;
        short winy;
        char head_swap;
        char head_equal;
        char win_swap;
        char win_equal;
        short headbutlen;
        short headbutofs;
        short cursor;
        short flag;
        ScriptLink scriptlink;
        ListBase spacedata;
        ListBase uiblocks;
        ListBase panels;
    };
}


#endif//__BLENDER_SCRAREA__H__
