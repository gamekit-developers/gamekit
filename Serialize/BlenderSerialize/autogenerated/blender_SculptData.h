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
#ifndef __BLENDER_SCULPTDATA__H__
#define __BLENDER_SCULPTDATA__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_BrushData.h"

namespace Blender {


    // ---------------------------------------------- //
    class SculptData
    {
    public:
        bInvalidHandle *session;
        MTex *mtex[18];
        CurveMapping *cumap;
        BrushData drawbrush;
        BrushData smoothbrush;
        BrushData pinchbrush;
        BrushData inflatebrush;
        BrushData grabbrush;
        BrushData layerbrush;
        BrushData flattenbrush;
        vec3f pivot;
        short brush_type;
        short texact;
        short texnr;
        short spacing;
        char texrept;
        char texfade;
        char texsep;
        char averaging;
        char flags;
        char tablet_size;
        char tablet_strength;
        char symm;
        char rake;
        char axislock;
        char pad[2];
    };
}


#endif//__BLENDER_SCULPTDATA__H__
