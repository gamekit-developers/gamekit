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
#include "gkPhysicsDebug.h"
#include "gkDynamicsWorld.h"
#include "gkScene.h"



gkPhysicsDebug::gkPhysicsDebug(gkDynamicsWorld *wo)
:       m_node(0), m_physics(wo), m_radius(0.f),
        m_bbmin(GK_INFINITY, GK_INFINITY, GK_INFINITY),
        m_bbmax(-GK_INFINITY, -GK_INFINITY, -GK_INFINITY),
        m_bufSize(0), m_flags(0)
{
    growBuffer(256);
}

gkPhysicsDebug::~gkPhysicsDebug()
{
    if (m_node && m_physics->isLoaded())
    {
        gkScene *sc = m_physics->getScene();
        if (sc->isLoaded())
        {
            Ogre::SceneManager *mgr = sc->getManager();
            mgr->destroySceneNode(m_node);
        } // else no manager no node

        m_node = 0;
    }
    delete mRenderOp.vertexData;
    mRenderOp.vertexData = 0;
    m_buffer.setNull();
}

void gkPhysicsDebug::verifyNode(void)
{
    if (m_node) 
        return;

    if (m_physics->isLoaded())
    {
        gkScene *sc = m_physics->getScene();
        if (sc->isLoaded())
        {
            Ogre::SceneManager *mgr = sc->getManager();
            m_node = mgr->getRootSceneNode()->createChildSceneNode();
            m_node->attachObject(this);

            const gkString matname = "gkPhysicsDebug::Material";


            m_pMaterial = Ogre::MaterialManager::getSingleton().getByName(matname);
            if (m_pMaterial.isNull())
            {
                m_pMaterial = Ogre::MaterialManager::getSingleton().create(matname, "General");
                m_pMaterial->setLightingEnabled(false);
                m_pMaterial->setReceiveShadows(false);

            }
        }
    }
}


void gkPhysicsDebug::flush(void)
{
    if (m_lineBuf.empty()) 
        return;

    if (!m_buffer.isNull())
    {
        gkDebugVertex *vp = (gkDebugVertex*)m_buffer->lock(0, sizeof(gkDebugVertex)*m_lineBuf.size(), Ogre::HardwareBuffer::HBL_DISCARD);

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

    m_lineBuf.resize(0);
}


void gkPhysicsDebug::growBuffer(UTsize newSize)
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

void gkPhysicsDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    verifyNode();

    if (!m_node) return;

    gkDebugVertex v0, v1;

    v0.v[0] = from[0]; v0.v[1] = from[1];
    v0.v[2] = from[2]; v1.v[0] = to[0];
    v1.v[1] = to[1];   v1.v[2] = to[2];
    Ogre::PixelUtil::packColour(color[0], color[1], color[2], 1.f, Ogre::PF_BYTE_RGBA, &v0.color);
    v1.color = v0.color;

    m_lineBuf.push_back(v0);
    m_lineBuf.push_back(v1);

    m_bbmin.makeFloor(Ogre::Vector3(&v0.v[0]));
    m_bbmin.makeFloor(Ogre::Vector3(&v1.v[0]));
    m_bbmax.makeCeil(Ogre::Vector3(&v0.v[0]));
    m_bbmax.makeCeil(Ogre::Vector3(&v1.v[0]));

    if (m_lineBuf.size() > m_bufSize)
        growBuffer(m_lineBuf.size() * 4);
}

void gkPhysicsDebug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    // todo 
}

void gkPhysicsDebug::draw3dText(const btVector3& location, const char* textString)
{
    // todo ?
}

void gkPhysicsDebug::reportErrorWarning(const char* warningString)
{
    // todo 
}

void gkPhysicsDebug::setDebugMode(int debugMode)
{
    m_flags = debugMode;
}

int gkPhysicsDebug::getDebugMode() const
{
    return m_flags;
}

Ogre::Real gkPhysicsDebug::getSquaredViewDepth(const Ogre::Camera* cam) const
{
    Ogre::Vector3 min, max, mid, dist;
    min = mBox.getMinimum();
    max = mBox.getMaximum();
    mid = ((max - min) * 0.5) + min;
    dist = cam->getDerivedPosition() - mid;
    return dist.squaredLength();
}

Ogre::Real gkPhysicsDebug::getBoundingRadius(void) const
{
    return m_radius;
}
