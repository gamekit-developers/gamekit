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
#ifndef __BLENDER_OBJECT__H__
#define __BLENDER_OBJECT__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ID.h"
#include "blender_ListBase.h"
#include "blender_ScriptLink.h"

namespace Blender {


    // ---------------------------------------------- //
    class Object
    {
    public:
        ID id;
        short type;
        short partype;
        int par1;
        int par2;
        int par3;
        char parsubstr[32];
        Object *parent;
        Object *track;
        Object *proxy;
        Object *proxy_group;
        Object *proxy_from;
        Ipo *ipo;
        bInvalidHandle *path;
        BoundBox *bb;
        bAction *action;
        bAction *poselib;
        bPose *pose;
        void *data;
        ListBase constraintChannels;
        ListBase effect;
        ListBase disp;
        ListBase defbase;
        ListBase modifiers;
        Material **mat;
        vec3f loc;
        vec3f dloc;
        vec3f orig;
        vec3f size;
        vec3f dsize;
        vec3f rot;
        vec3f drot;
        float obmat[4][4];
        float parentinv[4][4];
        float constinv[4][4];
        float imat[4][4];
        int lay;
        short flag;
        short colbits;
        short transflag;
        short ipoflag;
        short trackflag;
        short upflag;
        short nlaflag;
        short protectflag;
        short ipowin;
        short scaflag;
        short scavisflag;
        short boundtype;
        int dupon;
        int dupoff;
        int dupsta;
        int dupend;
        float sf;
        float ctime;
        float mass;
        float damping;
        float inertia;
        float formfactor;
        float rdamping;
        float sizefac;
        float margin;
        float max_vel;
        float min_vel;
        float m_contactProcessingThreshold;
        char dt;
        char dtx;
        char totcol;
        char actcol;
        char empty_drawtype;
        char pad1[3];
        float empty_drawsize;
        float dupfacesca;
        ScriptLink scriptlink;
        ListBase prop;
        ListBase sensors;
        ListBase controllers;
        ListBase actuators;
        vec3f bbsize;
        short index;
        short actdef;
        vec4f col;
        int gameflag;
        int gameflag2;
        BulletSoftBody *bsoft;
        short softflag;
        short recalc;
        vec3f anisotropicFriction;
        ListBase constraints;
        ListBase nlastrips;
        ListBase hooks;
        ListBase particlesystem;
        PartDeflect *pd;
        SoftBody *soft;
        Group *dup_group;
        short fluidsimFlag;
        short restrictflag;
        short shapenr;
        short shapeflag;
        float smoothresh;
        short recalco;
        short body_type;
        FluidsimSettings *fluidsimSettings;
        bInvalidHandle *derivedDeform;
        bInvalidHandle *derivedFinal;
        int lastDataMask;
        int state;
        int init_state;
        int pad2;
        void *vnode;
        ListBase gpulamp;
    };
}


#endif//__BLENDER_OBJECT__H__
