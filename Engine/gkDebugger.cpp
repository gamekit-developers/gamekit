/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreSceneManager.h"
#include "OgrePixelFormat.h"
#include "OgreHardwareBufferManager.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreMaterialManager.h"
#include "OgreMaterial.h"
#include "gkDebugger.h"
#include "gkScene.h"
#include "gkEngine.h"
#include "gkUserDefs.h"


#ifdef OGREKIT_OPENAL_SOUND
#include "Sound/gkSource.h"
#endif


gkDebugger::gkDebugger(gkScene* parent)
	:   m_node(0),
	    m_parent(parent),
	    m_radius(0.f),
	    m_bbmin(GK_INFINITY, GK_INFINITY, GK_INFINITY),
	    m_bbmax(-GK_INFINITY, -GK_INFINITY, -GK_INFINITY),
	    m_bufSize(0),
	    m_flags(0),
		m_d3dColor(false)
{
	growBuffer(128);
	m_lineBuf.reserve(128);

	m_d3dColor = gkEngine::getSingleton().getUserDefs().isD3DRenderSystem();
}


gkDebugger::~gkDebugger()
{
	if (m_node && m_parent->isInstanced())
	{
		Ogre::SceneManager* mgr = m_parent->getManager();
		mgr->destroySceneNode(m_node);
		m_node = 0;
	}

	delete mRenderOp.vertexData;
	mRenderOp.vertexData = 0;
	m_buffer.setNull();
}


void gkDebugger::verifyNode(void)
{
	if (!isVisible())
		setVisible(true);

	if (m_node)
		return;


	if (m_parent->isInstanced())
	{
		Ogre::SceneManager* mgr = m_parent->getManager();
		m_node = mgr->getRootSceneNode()->createChildSceneNode();
		m_node->attachObject(this);

		const gkString matname = "gkDebugger::Material";


		m_pMaterial = Ogre::MaterialManager::getSingleton().getByName(matname, GK_BUILTIN_GROUP);
		if (m_pMaterial.isNull())
		{
			m_pMaterial = Ogre::MaterialManager::getSingleton().create(matname, GK_BUILTIN_GROUP);
			m_pMaterial->setLightingEnabled(false);
			m_pMaterial->setReceiveShadows(false);
		}
	}
}

void gkDebugger::clear(void)
{
	if (m_node && this->isVisible())
		setVisible(false);
}


void gkDebugger::flush(void)
{
	if (m_lineBuf.empty())
		return;

	if (!m_buffer.isNull())
	{
		DebugVertex* vp = (DebugVertex*)m_buffer->lock(0, sizeof(DebugVertex) * m_lineBuf.size(), Ogre::HardwareBuffer::HBL_DISCARD);

		// write contents to buffer
		Buffer::Pointer ptr = m_lineBuf.ptr();
		UTsize i = 0, size = m_lineBuf.size();
		while (i < size) (*vp++) = ptr[i++];

		m_buffer->unlock();

		// update vertex count
		mRenderOp.vertexData->vertexCount = m_lineBuf.size();
	}


	// apply new bounding box
	mBox.merge(Ogre::AxisAlignedBox(m_bbmin, m_bbmax));

	m_radius = gkMaxf(m_bbmax.squaredLength(), m_radius);
	m_radius = gkMaxf(m_bbmin.squaredLength(), m_radius);
	m_radius = Ogre::Math::Sqrt(m_radius);

	m_bbmin = Ogre::Vector3( GK_INFINITY,  GK_INFINITY,  GK_INFINITY);
	m_bbmax = Ogre::Vector3(-GK_INFINITY, -GK_INFINITY, -GK_INFINITY);

	// must mark the node
	m_node->_update(false, false);

	m_lineBuf.clear(true);
}


void gkDebugger::growBuffer(UTsize newSize)
{
	// expand vertex buffer size
	if (newSize <= m_bufSize)
		return;

	if (mRenderOp.vertexData)
	{
		delete mRenderOp.vertexData;
		mRenderOp.vertexData = 0;
		m_buffer.setNull();
	}

	m_bufSize = newSize;

	mRenderOp.vertexData = new Ogre::VertexData();
	Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;

	size_t offs = 0;
	decl->addElement(0, offs, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offs += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	decl->addElement(0, offs, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
	offs += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR_ABGR);


	mRenderOp.vertexData->vertexStart = 0;
	mRenderOp.vertexData->vertexCount = m_bufSize;
	mRenderOp.operationType = Ogre::RenderOperation::OT_LINE_LIST;

	m_buffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
	               offs, mRenderOp.vertexData->vertexCount,
	               Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false);


	mRenderOp.vertexData->vertexBufferBinding->setBinding(0, m_buffer);
	mRenderOp.useIndexes = false;
	mRenderOp.indexData = 0;
}


void gkDebugger::drawLine(const gkVector3& from, const gkVector3& to, const gkVector3& color)
{
	verifyNode();

	if (!m_node) return;

	DebugVertex v0, v1;
	Ogre::PixelUtil::packColour(color.x, color.y, color.z, 1.f, m_d3dColor ? Ogre::PF_A8R8G8B8 : Ogre::PF_A8B8G8R8, &v0.color);

	v0.v = from;
	v1.v = to;
	v1.color = v0.color;


	m_lineBuf.push_back(v0);
	m_lineBuf.push_back(v1);

	m_bbmin.makeFloor(v0.v);  m_bbmin.makeFloor(v1.v);
	m_bbmax.makeCeil(v0.v);   m_bbmax.makeCeil(v1.v);

	if (m_lineBuf.size() > m_bufSize)
		growBuffer(m_lineBuf.size() * 4);
}


#ifdef OGREKIT_OPENAL_SOUND

void gkDebugger::draw3dSound(const gkSoundProperties& props)
{
	// Draw arrow  (sound --> ...)
	// head is the direction
	// tail is the velocity
	// arrow opening is the cone outer angle
	// 2nd arrow opening is the cone inner angle
	// cone height is the objects AABB size


	const gkVector3 RED(1, 0, 0);
	const gkVector3 ORANGE(1, 0.5, 0.0);
	const gkVector3 WHITE(1, 1, 1);

	gkScalar ia = gkMaxf(props.m_coneAngle.x, 1.f);
	gkScalar oa = gkMaxf(props.m_coneAngle.y, 1.f);
	gkScalar hi  = gkMaxf(props.m_height, 0.5f);
	gkScalar ve  = props.m_velocity.length();

	gkVector3 he = props.m_position - props.m_direction;
	gkVector3 ta = props.m_position + (props.m_orientation * gkVector3(0, ve, 0));

	// head to tail
	drawLine(he, ta, WHITE);

	// outer cone angle
	gkScalar a  = hi / (oa * gkRPD);
	gkScalar r = .5f * (a * a - hi * hi);


	gkVector3 v0 = props.m_position + (props.m_orientation * gkVector3(r, 0, 0));
	gkVector3 v1 = props.m_position + (props.m_orientation * gkVector3(0, 0, r));
	gkVector3 v2 = props.m_position + (props.m_orientation * gkVector3(-r, 0, 0));
	gkVector3 v3 = props.m_position + (props.m_orientation * gkVector3(0, 0, -r));

	drawLine(v0, v1, RED);
	drawLine(v1, v2, RED);
	drawLine(v2, v3, RED);
	drawLine(v3, v0, RED);
	drawLine(he, v0, ORANGE);
	drawLine(he, v1, ORANGE);
	drawLine(he, v2, ORANGE);
	drawLine(he, v3, ORANGE);

	if (ia < oa)
	{
		// inner cone angle
		a  = hi / (ia * gkRPD);
		r = .5f * (a * a - hi * hi);

		v0 = props.m_position + (props.m_orientation * gkVector3(r, 0, 0));
		v1 = props.m_position + (props.m_orientation * gkVector3(0, 0, r));
		v2 = props.m_position + (props.m_orientation * gkVector3(-r, 0, 0));
		v3 = props.m_position + (props.m_orientation * gkVector3(0, 0, -r));

		drawLine(v0, v1, RED);
		drawLine(v1, v2, RED);
		drawLine(v2, v3, RED);
		drawLine(v3, v0, RED);
	}
}

#endif

Ogre::Real gkDebugger::getSquaredViewDepth(const Ogre::Camera* cam) const
{
	Ogre::Vector3 min, max, mid, dist;
	min = mBox.getMinimum();
	max = mBox.getMaximum();
	mid = ((max - min) * 0.5) + min;
	dist = cam->getDerivedPosition() - mid;
	return dist.squaredLength();
}

Ogre::Real gkDebugger::getBoundingRadius(void) const
{
	return m_radius;
}
