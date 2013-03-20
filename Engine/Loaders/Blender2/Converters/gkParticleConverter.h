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


#ifndef _gkParticleConverter_h_
#define _gkParticleConverter_h_

//#include "bCommon.h"
#include "Blender.h"
#include "gkCommon.h"

class gkParticleConverter
{
	const gkResourceNameString	m_groupName;
	gkScalar m_fps;

public:

	gkParticleConverter(const gkResourceNameString& groupName="", gkScalar fps=24.f) : m_groupName(groupName), m_fps(fps) {}
	~gkParticleConverter() {}

	void convertParticle(Blender::ParticleSettings* pt);

};

#endif//_gkParticleConverter_h_

