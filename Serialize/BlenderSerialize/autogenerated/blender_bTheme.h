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
#ifndef __BLENDER_BTHEME__H__
#define __BLENDER_BTHEME__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ThemeSpace.h"
#include "blender_ThemeUI.h"
#include "blender_ThemeWireColor.h"

namespace Blender {


    // ---------------------------------------------- //
    class bTheme
    {
    public:
        bTheme *next;
        bTheme *prev;
        char name[32];
        ThemeUI tui;
        ThemeSpace tbuts;
        ThemeSpace tv3d;
        ThemeSpace tfile;
        ThemeSpace tipo;
        ThemeSpace tinfo;
        ThemeSpace tsnd;
        ThemeSpace tact;
        ThemeSpace tnla;
        ThemeSpace tseq;
        ThemeSpace tima;
        ThemeSpace timasel;
        ThemeSpace text;
        ThemeSpace toops;
        ThemeSpace ttime;
        ThemeSpace tnode;
        ThemeWireColor tarm[20];
        char bpad[4];
        char bpad1[4];
    };
}


#endif//__BLENDER_BTHEME__H__
