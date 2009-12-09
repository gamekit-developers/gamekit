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
#ifndef __BLENDER_SEQUENCE__H__
#define __BLENDER_SEQUENCE__H__


// -------------------------------------------------- //
#include "blender_Common.h"
#include "blender_ListBase.h"

namespace Blender {


    // ---------------------------------------------- //
    class Sequence
    {
    public:
        Sequence *next;
        Sequence *prev;
        void *tmp;
        void *lib;
        char name[24];
        int flag;
        int type;
        int len;
        int start;
        int startofs;
        int endofs;
        int startstill;
        int endstill;
        int machine;
        int depth;
        int startdisp;
        int enddisp;
        float mul;
        float handsize;
        int sfra;
        int anim_preseek;
        Strip *strip;
        Ipo *ipo;
        Scene *scene;
        bInvalidHandle *anim;
        float facf0;
        float facf1;
        PluginSeq *plugin;
        Sequence *seq1;
        Sequence *seq2;
        Sequence *seq3;
        ListBase seqbase;
        bSound *sound;
        bInvalidHandle *hdaudio;
        float level;
        float pan;
        int scenenr;
        float strobe;
        void *effectdata;
        int anim_startofs;
        int anim_endofs;
        int blend_mode;
        float blend_opacity;
    };
}


#endif//__BLENDER_SEQUENCE__H__
