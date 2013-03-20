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

#ifndef _gkOgreParticleRenderer_h_
#define _gkOgreParticleRenderer_h_

#include "OgreBillboardParticleRenderer.h"
#include "OgreParticle.h"

class gkParticleResource;

class gkParticleVisualData : public Ogre::ParticleVisualData
{
public:
	Ogre::Vector3 m_initDir;
};

class gkOgreParticleRenderer : public Ogre::BillboardParticleRenderer
{
protected:
	gkParticleResource* m_creator;

public:
	static const gkString NAME;

	gkOgreParticleRenderer();
	virtual ~gkOgreParticleRenderer();

	GK_INLINE void setCreator(gkParticleResource* creator) { m_creator = creator; }

	virtual Ogre::ParticleVisualData* _createVisualData(void) { return OGRE_NEW gkParticleVisualData; }
	virtual void  _destroyVisualData(Ogre::ParticleVisualData *vis) { OGRE_DELETE vis; }
};


class gkOgreParticleRendererFactory : public Ogre::ParticleSystemRendererFactory
{
public:
	GK_INLINE const Ogre::String& getType() const { return gkOgreParticleRenderer::NAME; }

	Ogre::ParticleSystemRenderer* createInstance(const Ogre::String& name);

	void destroyInstance(Ogre::ParticleSystemRenderer* inst);
};


#endif//_gkOgreParticleRenderer_h_

