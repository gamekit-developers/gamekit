/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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
#ifndef _gkDebugger_h_
#define _gkDebugger_h_


#include "gkCommon.h"
#include "gkMathUtils.h"
#include "OgreSimpleRenderable.h"
#include "OgreHardwareVertexBuffer.h"

class gkSoundProperties;

// for debugging / building line lists
class gkDebugger : public Ogre::SimpleRenderable
{
public:

	struct DebugVertex
	{
		gkVector3 v;
		unsigned int color;
	};


	typedef utArray<DebugVertex> Buffer;


public:
	gkDebugger(gkScene* parent);
	virtual ~gkDebugger();

	void drawLine(const gkVector3& from, const gkVector3& to, const gkVector3& color);

	void clear(void);

#ifdef OGREKIT_OPENAL_SOUND
	void draw3dSound(const gkSoundProperties& props);
#endif

	void flush(void);

protected:


	Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
	Ogre::Real getBoundingRadius(void) const;


	void verifyNode(void);
	void growBuffer(UTsize newSize);

	Ogre::SceneNode*    m_node;
	gkScene*            m_parent;
	Ogre::Real          m_radius;
	Buffer              m_lineBuf;
	Ogre::Vector3       m_bbmin, m_bbmax;
	UTsize              m_bufSize;
	int                 m_flags;
	bool                m_d3dColor;

	Ogre::HardwareVertexBufferSharedPtr m_buffer;

};




#endif//_gkDebugger_h_
