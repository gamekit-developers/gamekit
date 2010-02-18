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
#ifndef __BLENDER_SCRIPT__H__
#define __BLENDER_SCRIPT__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"

namespace Blender {


    // ---------------------------------------------- //
    class Script
    {
    public:
        ID id;
        void *py_draw;
        void *py_event;
        void *py_button;
        void *py_browsercallback;
        void *py_globaldict;
        int flags;
        int lastspace;
        char scriptname[256];
        char scriptarg[256];
    };
}


#endif//__BLENDER_SCRIPT__H__
