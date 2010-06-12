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



gkDebugger::gkDebugger(gkScene *parent)
    :   m_node(0), 
        m_parent(parent), 
        m_radius(0.f),
        m_bbmin(GK_INFINITY, GK_INFINITY, GK_INFINITY),
        m_bbmax(-GK_INFINITY, -GK_INFINITY, -GK_INFINITY),
        m_bufSize(0), 
        m_flags(0)
{
    growBuffer(128);
    m_lineBuf.reserve(128);
}


gkDebugger::~gkDebugger()
{
    if (m_node && m_parent->isLoaded())
    {
        Ogre::SceneManager *mgr = m_parent->getManager();
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


    if (m_parent->isLoaded())
    {
        Ogre::SceneManager *mgr = m_parent->getManager();
        m_node = mgr->getRootSceneNode()->createChildSceneNode();
        m_node->attachObject(this);

        const gkString matname = "gkDebugger::Material";


        m_pMaterial = Ogre::MaterialManager::getSingleton().getByName(matname);
        if (m_pMaterial.isNull())
        {
            m_pMaterial = Ogre::MaterialManager::getSingleton().create(matname, "<gkBuiltin>");
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
        DebugVertex *vp = (DebugVertex*)m_buffer->lock(0, sizeof(DebugVertex)*m_lineBuf.size(), Ogre::HardwareBuffer::HBL_DISCARD);

        // write contents to buffer
        Buffer::Pointer ptr = m_lineBuf.ptr();
        UTsize i=0, size = m_lineBuf.size();
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
    Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

    size_t offs = 0;
    decl->addElement(0, offs, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offs += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    decl->addElement(0, offs, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
    offs += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);


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
    Ogre::PixelUtil::packColour(color.x, color.y, color.z, 1.f, Ogre::PF_BYTE_RGBA, &v0.color);

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
