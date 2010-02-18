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
#ifndef __BLENDER_CAMERA__H__
#define __BLENDER_CAMERA__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"
#include "blender_ScriptLink.h"

namespace Blender {


    // ---------------------------------------------- //
    class Camera
    {
    public:
        ID id;
        short type;
        short flag;
        float passepartalpha;
        float angle;
        float clipsta;
        float clipend;
        float lens;
        float ortho_scale;
        float drawsize;
        float shiftx;
        float shifty;
        float YF_dofdist;
        float YF_aperture;
        short YF_bkhtype;
        short YF_bkhbias;
        float YF_bkhrot;
        Ipo *ipo;
        ScriptLink scriptlink;
        Object *dof_ob;
    };
}


#endif//__BLENDER_CAMERA__H__
