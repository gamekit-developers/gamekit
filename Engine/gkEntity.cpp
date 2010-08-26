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
#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreMeshManager.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"
#include "gkEntity.h"
#include "gkScene.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkActionManager.h"
#include "gkAction.h"
#include "gkSkeleton.h"
#include "gkMesh.h"




// ----------------------------------------------------------------------------
gkEntity::gkEntity(gkScene *scene, const gkString &name)
	:	gkGameObject(scene, name, GK_ENTITY),
	    m_entityProps(new gkEntityProperties()),
	    m_entity(0),
	    m_active(0),
	    m_skeleton(0),
	    m_meshLoader(0)
{
}



// ----------------------------------------------------------------------------
gkEntity::~gkEntity()
{
	if (m_meshLoader)
		delete m_meshLoader;

	delete m_entityProps;
}


// ----------------------------------------------------------------------------
void gkEntity::initializeImpl(void)
{
	gkGameObject::initializeImpl();

	GK_ASSERT(!m_entity);


	if (!m_entityProps->m_mesh)
		return;

	Ogre::SceneManager *manager = m_scene->getManager();


	// update mesh state
	createMesh();



	gkGameObject *parent = 0;
	if (!m_baseProps.m_parent.empty())
		parent = m_scene->getObject(m_baseProps.m_parent);


	if (parent && parent->getType() == GK_SKELETON)
	{
		// just in case
		parent->initialize();

		// attach entity to skeleton
		m_skeleton = static_cast<gkSkeleton *>(parent);
		m_skeleton->setController(this);
	}

	m_entity = manager->createEntity(m_name, m_entityProps->m_mesh->getName());

	if (parent && parent->getType() == GK_SKELETON)
		m_skeleton->setEntity(m_entity);

	if (!m_entityProps->m_startPose.empty())
		_resetPose();



	m_entity->setCastShadows(m_entityProps->m_casts);
	m_node->attachObject(m_entity);

	if (m_baseProps.isInvisible())
		m_node->setVisible(false, false);
}



// ----------------------------------------------------------------------------
void gkEntity::finalizeImpl(void)
{
	if (m_entity)
	{

		Ogre::SceneManager *manager = m_scene->getManager();

		if (m_skeleton)
			m_skeleton->setController(0);

		if (!m_entityProps->m_startPose.empty())
			_resetPose();

		if (!m_scene->isFinalizing())
		{
			if (m_node)
				m_node->detachObject(m_entity);
		
			manager->destroyEntity(m_entity);
		}
	}

	m_entity = 0;
	m_skeleton = 0;


	gkGameObject::finalizeImpl();
}


// ----------------------------------------------------------------------------
void gkEntity::_finalizeAsInstance(void)
{
	if (m_entity != 0)
	{
		Ogre::SceneManager *manager = m_scene->getManager();
		if (m_skeleton)
			m_skeleton->setController(0);

		if (!m_entityProps->m_startPose.empty())
			_resetPose();

		if (m_node)
			m_node->detachObject(m_entity);

		manager->destroyEntity(m_entity);

		m_entity = 0;
		m_skeleton = 0;
	}
}



// ----------------------------------------------------------------------------
void gkEntity::evalAction(gkAction *act, gkScalar animTime)
{
	if (m_skeleton)
	{
		if (act && act != m_active)
		{
			if (m_skeleton->hasAction(act->getName()))
			{
				m_active = m_skeleton->getAction(act->getName());
				m_actionMgr.setAction(m_active);
			}
			else return;

		}

		if (m_active)
			m_actionMgr.update(animTime, 0.416f);
	}
}



// ----------------------------------------------------------------------------
void gkEntity::playAction(const gkString &act, gkScalar blend)
{
	if (m_skeleton)
	{
		if (m_active == 0 || act != m_active->getName())
		{
			if (m_skeleton->hasAction(act))
			{
				m_active = m_skeleton->getAction(act);
				m_actionMgr.setAction(m_active);
			}
			else  return;
		}

		if (m_active)
		{
			m_active->setBlendFrames(blend);
			m_actionMgr.update(0.416f);
		}
	}

}



// ----------------------------------------------------------------------------
gkGameObject *gkEntity::clone(const gkString &name)
{
	gkEntity *cl= new gkEntity(m_scene, name);

	memcpy(cl->m_entityProps, m_entityProps, sizeof(gkEntityProperties));
	cl->m_entityProps->m_mesh = m_entityProps->m_mesh;

	gkGameObject::cloneImpl(cl);
	return cl;
}



// ----------------------------------------------------------------------------
void gkEntity::_resetPose(void)
{
	if (m_skeleton)
	{
		if (!m_entityProps->m_startPose.empty())
		{
			if (m_skeleton->hasAction(m_entityProps->m_startPose))
			{
				m_active = m_skeleton->getAction(m_entityProps->m_startPose);
				m_actionMgr.setAction(m_active);
				m_active->setTimePosition(gkEngine::getSingleton().getUserDefs().startframe);
				m_actionMgr.update(0.416f);
			}
		}
	}
}


// ----------------------------------------------------------------------------
gkBoundingBox gkEntity::getAabb() const
{
	return m_entityProps->m_mesh ? m_entityProps->m_mesh->getBoundingBox() : gkGameObject::getAabb();
}




// ----------------------------------------------------------------------------
class gkEntityMeshLoader : public Ogre::ManualResourceLoader
{
public:

	gkEntityMeshLoader(gkEntity *ent);
	virtual ~gkEntityMeshLoader() {}

	void loadResource(Ogre::Resource *resource);
	void loadMaterial(gkSubMesh *mesh);

private:
	gkEntity *m_entity;
};





// ----------------------------------------------------------------------------
void gkEntity::createMesh(void)
{
	if (m_meshLoader != 0)
		return;

	if (m_entityProps->m_mesh != 0)
	{
		Ogre::MeshPtr omesh = Ogre::MeshManager::getSingleton().getByName(m_entityProps->m_mesh->getName());
		if (omesh.isNull())
		{
			m_meshLoader = new gkEntityMeshLoader(this);
			omesh = Ogre::MeshManager::getSingleton().create(m_entityProps->m_mesh->getName(), "<gkBuiltin>", true, m_meshLoader);
		}
	}
}




// ----------------------------------------------------------------------------
gkEntityMeshLoader::gkEntityMeshLoader(gkEntity *ent)
	:   m_entity(ent)
{
}



// ----------------------------------------------------------------------------
void gkEntityMeshLoader::loadMaterial(gkSubMesh *mesh)
{
	gkMaterialProperties &gma = mesh->getMaterial();
	if (gma.m_name.empty())
		gma.m_name = "<gkBuiltin/DefaultMaterial>";

	Ogre::MaterialPtr oma = Ogre::MaterialManager::getSingleton().getByName(gma.m_name.c_str());
	if (!oma.isNull())
		return;

	oma = Ogre::MaterialManager::getSingleton().create(gma.m_name.c_str(), "<gkBuiltin>");

	if (gma.m_mode & gkMaterialProperties::MA_INVISIBLE)
	{
		// disable writing to this material
		oma->setReceiveShadows(false);
		oma->setColourWriteEnabled(false);
		oma->setDepthWriteEnabled(false);
		oma->setDepthCheckEnabled(false);
		oma->setLightingEnabled(false);
		return;
	}

	if (gma.m_mode & gkMaterialProperties::MA_TWOSIDE)
	{
		oma->setCullingMode(Ogre::CULL_NONE);
		oma->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
	}

	// apply lighting params

	bool enableLights = m_entity->getOwner()->hasLights() && (gma.m_mode & gkMaterialProperties::MA_LIGHTINGENABLED) != 0;
	oma->setReceiveShadows((gma.m_mode & gkMaterialProperties::MA_RECEIVESHADOWS) != 0);

	oma->setLightingEnabled(enableLights);
	if (enableLights)
	{
		gkColor emissive, ambient, specular, diffuse;

		emissive    = gma.m_diffuse * gma.m_emissive;
		ambient     = gma.m_diffuse * gma.m_ambient;
		specular    = gma.m_specular * gma.m_spec;
		diffuse     = gma.m_diffuse * (gma.m_emissive + gma.m_refraction);

		emissive.a = ambient.a = specular.a = diffuse.a = 1.f;

		oma->setSelfIllumination(emissive);
		oma->setAmbient(ambient);
		oma->setSpecular(specular);
		oma->setDiffuse(diffuse);
		oma->setShininess(gma.m_hardness);
	}

	Ogre::Pass *pass = oma->getTechnique(0)->getPass(0);

	for (int i=0; i<gma.m_totaltex; ++i)
	{
		gkTexureProperties &gte = gma.m_textures[i];
		Ogre::TextureUnitState *otus = pass->createTextureUnitState(gte.m_name.c_str(), gte.m_layer);
		otus->setColourOperation(Ogre::LBO_MODULATE);
	}

	if (gma.m_mode & gkMaterialProperties::MA_ALPHABLEND)
	{
		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 150);
		pass->setDepthWriteEnabled(false);
	}
}




// ----------------------------------------------------------------------------
void gkEntityMeshLoader::loadResource(Ogre::Resource *resource)
{
	using namespace Ogre;

	Ogre::Mesh *omesh = static_cast<Ogre::Mesh *>(resource);
	gkMesh *gmesh = m_entity->getEntityProperties().m_mesh;

	HardwareBufferManager *bufferManager = HardwareBufferManager::getSingletonPtr();

	// setup skeleton instance

	gkSkeleton *skel = 0;


	if (m_entity->getSkeleton())
	{
		skel = m_entity->getSkeleton();
		if (skel)
			omesh->setSkeletonName(skel->getName());
	}


	gkMesh::SubMeshIterator iter = gmesh->getSubMeshIterator();
	while (iter.hasMoreElements())
	{
		gkSubMesh *gsubmesh     = iter.getNext();
		Ogre::SubMesh *osubmesh = omesh->createSubMesh();


		loadMaterial(gsubmesh);
		osubmesh->setMaterialName(gsubmesh->getMaterialName());

		osubmesh->vertexData = new VertexData();
		osubmesh->vertexData->vertexCount = gsubmesh->getVertexBuffer().size();

		// disable sharing
		osubmesh->useSharedVertices = false;

		// converting to tri list
		osubmesh->operationType = RenderOperation::OT_TRIANGLE_LIST;

		size_t offs = 0;

		// fill in the declaration
		VertexDeclaration *decl = osubmesh->vertexData->vertexDeclaration;


		// position
		decl->addElement(0, offs, VET_FLOAT3, VES_POSITION);
		offs += VertexElement::getTypeSize(VET_FLOAT3);

		// no, blending weights

		// normals
		decl->addElement(0, offs, VET_FLOAT3, VES_NORMAL);
		offs += VertexElement::getTypeSize(VET_FLOAT3);

		// texture coordinates
		int maxTco = gsubmesh->getUvLayerCount();
		for (int lay = 0; lay < maxTco; ++lay)
		{
			decl->addElement(0, offs, VET_FLOAT2, VES_TEXTURE_COORDINATES, lay);
			offs += VertexElement::getTypeSize(VET_FLOAT2);
		}

		bool diffuseVert = gsubmesh->hasVertexColors();
		if (diffuseVert)
		{
			// diffuse colours
			decl->addElement(0, offs, VET_COLOUR_ABGR, VES_DIFFUSE);
			offs += VertexElement::getTypeSize(VET_COLOUR_ABGR);
		}

		// no, specular colours

		HardwareVertexBufferSharedPtr vertBuf = bufferManager->createVertexBuffer(offs,
		                                        osubmesh->vertexData->vertexCount,
		                                        HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		// bind the source
		VertexBufferBinding *bind = osubmesh->vertexData->vertexBufferBinding;
		bind->setBinding(0, vertBuf);


		// index buffer
		size_t indx_size = gsubmesh->getIndexBuffer().size() * 3;

		HardwareIndexBuffer::IndexType buff_type = (indx_size > 65536) ?
		        HardwareIndexBuffer::IT_32BIT : HardwareIndexBuffer::IT_16BIT;

		HardwareIndexBufferSharedPtr indexBuffer = bufferManager->createIndexBuffer(buff_type, indx_size,
		        HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		//
		osubmesh->indexData->indexCount = indx_size;
		osubmesh->indexData->indexBuffer = indexBuffer;


		// build index items
		{
			bool using32 = buff_type == HardwareIndexBuffer::IT_32BIT;

			unsigned int *indices32 = 0;
			unsigned short *indices16 = 0;

			if (!using32)
				indices16 = static_cast<unsigned short *>(indexBuffer->lock(HardwareBuffer::HBL_NORMAL));
			else
				indices32 = static_cast<unsigned int *>(indexBuffer->lock(HardwareBuffer::HBL_NORMAL));


			size_t ele_len = gsubmesh->getIndexBuffer().size();
			gkTriangle *ibuf = gsubmesh->getIndexBuffer().ptr();

			for (size_t cur = 0; cur < ele_len; cur++)
			{
				const gkTriangle &i = ibuf[cur];
				if (using32)
				{
					*indices32++ = (unsigned int)i.i0;
					*indices32++ = (unsigned int)i.i1;
					*indices32++ = (unsigned int)i.i2;
				}
				else
				{
					*indices16++ = (unsigned short)i.i0;
					*indices16++ = (unsigned short)i.i1;
					*indices16++ = (unsigned short)i.i2;
				}
			}
			indexBuffer->unlock();
		}

		// build vertex items
		{
			unsigned char *vptr = static_cast<unsigned char *>(vertBuf->lock(HardwareBuffer::HBL_NORMAL));

			VertexData *vdata = osubmesh->vertexData;

			float           *fptr = 0;
			unsigned int    *iptr = 0;


			UTsize  totvert = gsubmesh->getVertexBuffer().size();
			gkVertex *vbuf  = gsubmesh->getVertexBuffer().ptr();
			UTsize vbufSize = vertBuf->getVertexSize();


			UTsize i=0;
			while (i < totvert)
			{
				const gkVertex &vtx = vbuf[i++];

				// packed as
				// VES_POSITION | VES_NORMAL | VES_TEXTURE_COORDINATES[<8] | |= VES_DIFFUSE

				// VES_POSITION
				fptr = (float *)vptr;

				*fptr++ = vtx.co.x;
				*fptr++ = vtx.co.y;
				*fptr++ = vtx.co.z;

				// VES_NORMAL
				*fptr++ = vtx.no.x;
				*fptr++ = vtx.no.y;
				*fptr++ = vtx.no.z;


				// VES_TEXTURE_COORDINATES
				if (maxTco > 0)
				{
					for (int tc = 0; tc < maxTco; ++tc)
					{
						*fptr++ =          vtx.uv[tc].x;
						*fptr++ = 1.f -    vtx.uv[tc].y;
					}
				}

				if (diffuseVert)
				{
					// VES_DIFFUSE
					iptr    = (unsigned int *)fptr;
					*iptr++ = vtx.vcol;

					// goto next
					vptr = (unsigned char *)iptr;
				}
				else
					// goto next
					vptr = (unsigned char *)fptr;

			}

			vertBuf->unlock();

			if (skel)
			{
				gkSubMesh::DeformVerts &dvbuf = gsubmesh->getDeformVertexBuffer();

				i=0;
				totvert = dvbuf.size();
				gkSubMesh::DeformVerts::Pointer dvp = dvbuf.ptr();
				while (i < totvert)
				{
					gkDeformVertex &dvtx = dvp[i++];


					Ogre::VertexBoneAssignment vba;
					gkVertexGroup *vg = gmesh->findVertexGroup(dvtx.group);
					if (vg != 0)
					{
						gkBone *bone = skel->getBone(vg->getName());
						if (bone)
						{
							vba.boneIndex   = bone->getOgreBone()->getHandle();
							vba.vertexIndex = dvtx.vertexId;
							vba.weight      = dvtx.weight;
							osubmesh->addBoneAssignment(vba);
						}
					}
				}

				if (totvert > 0)
				{
					VertexDeclaration *newDecl = decl->getAutoOrganisedDeclaration(true, false);
					vdata->reorganiseBuffers(newDecl);
				}
			}


		}
	}

	omesh->_setBounds(gmesh->getBoundingBox(), false);
	omesh->_setBoundingSphereRadius(gmesh->getBoundingBox().getSize().squaredLength());
}
