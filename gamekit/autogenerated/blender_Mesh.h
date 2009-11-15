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
#ifndef __BLENDER_MESH__H__
#define __BLENDER_MESH__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_CustomData.h"
#include "blender_ID.h"
#include "blender_ListBase.h"

namespace Blender {


    // ---------------------------------------------- //
    class Mesh
    {
    public:
        ID id;
        BoundBox *bb;
        ListBase effect;
        Ipo *ipo;
        Key *key;
        Material **mat;
        MFace *mface;
        MTFace *mtface;
        TFace *tface;
        MVert *mvert;
        MEdge *medge;
        MDeformVert *dvert;
        MCol *mcol;
        MSticky *msticky;
        Mesh *texcomesh;
        MSelect *mselect;
        CustomData vdata;
        CustomData edata;
        CustomData fdata;
        int totvert;
        int totedge;
        int totface;
        int totselect;
        int act_face;
        int texflag;
        vec3f loc;
        vec3f size;
        vec3f rot;
        float cubemapsize;
        float pad;
        short smoothresh;
        short flag;
        short subdiv;
        short subdivr;
        short totcol;
        short subsurftype;
        Multires *mr;
        PartialVisibility *pv;
        void *vnode;
    };
}


#endif//__BLENDER_MESH__H__
