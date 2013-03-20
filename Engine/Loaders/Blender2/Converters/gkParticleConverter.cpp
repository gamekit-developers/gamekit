/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "gkSerialize.h"
#include "gkBlenderDefines.h"
#include "gkParticleConverter.h"
#include "gkParticleManager.h"
#include "gkLoaderCommon.h"

void gkParticleConverter::convertParticle(Blender::ParticleSettings* pt)
{
	if (!pt || !pt->effector_weights)
		return;	

	if (pt->ren_as == PART_DRAW_NOT)
		return;
	
	Blender::EffectorWeights* wt = pt->effector_weights;

	gkParticleSettingsProperties pp;

	pp.m_phyType = pt->phystype;

	pp.m_name = GKB_IDNAME(pt);

	pp.m_emitfrom = pt->from;
	pp.m_amount = pt->totpart;
	pp.m_lifetime = pt->lifetime/m_fps; 
	pp.m_start = pt->sta/m_fps;
	pp.m_end = pt->end/m_fps;
	pp.m_random = pt->randlife;
	pp.m_jitter = pt->jitfac;

	pp.m_velocity = gkVector3(pt->ob_vel[0], pt->ob_vel[1], pt->ob_vel[2])*10;
	pp.m_velNormal = pt->normfac;
	pp.m_velTanget = pt->tanfac;

	pp.m_size = pt->size*10;
	pp.m_mass = pt->mass;
	pp.m_sizeRandom = pt->randsize;		

	pp.m_gravity = wt->global_gravity; 
		
	pp.m_trailCount = pt->trail_count;
	pp.m_drawEmitter = (pt->draw & 0x8) != 0;

	pp.m_material = pt->omat - 1;

	if (pt->ren_as == PART_DRAW_HALO)
		pp.m_render = gkParticleSettingsProperties::R_HALO;
	else if (pt->ren_as == PART_DRAW_BB)
		pp.m_render = gkParticleSettingsProperties::R_BILLBOARD;	
	else if (pt->ren_as == PART_DRAW_OB)
		pp.m_render = gkParticleSettingsProperties::R_OBJECT;
	else if (pt->ren_as == PART_DRAW_GR)
		pp.m_render = gkParticleSettingsProperties::R_GROUP;
	else if (pt->ren_as == PART_DRAW_LINE)
		pp.m_render = gkParticleSettingsProperties::R_LINE;
	else if (pt->ren_as == PART_DRAW_PATH)
		pp.m_render = gkParticleSettingsProperties::R_PATH;
	//else if (pt->ren_as == PART_DRAW_NOT)
	//	pp.m_render = gkParticleSettingsProperties::R_NONE;
	

	gkParticleManager::getSingleton().createParticle(gkResourceName(pp.m_name, m_groupName), pp);
}