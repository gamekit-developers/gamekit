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
#include "gkOgreMeshLoader.h"
#include "gkOgreMaterialLoader.h"
#include "gkMesh.h"
#include "gkSkeletonResource.h"

#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreMeshManager.h"
#include "OgreHardwareBufferManager.h"


static const UTuint16 gk16BitClamp = (0xFFFF) - 1;
static const UTuint32 gk32BitClamp = (0xFFFFFFFF) - 1;


gkMeshLoader::gkMeshLoader(gkMesh* me)
	:    m_mesh(me)
{
	Ogre::MeshManager& mgr = Ogre::MeshManager::getSingleton();
	const gkString& name = m_mesh->getResourceName().getName();



	Ogre::MeshPtr omesh = mgr.getByName(name);
	if (omesh.isNull())
		omesh = mgr.create(name, me->getGroupName(), true, this); //GK_BUILTIN_GROUP
}



gkMeshLoader::~gkMeshLoader()
{
	Ogre::MeshManager& mgr = Ogre::MeshManager::getSingleton();
	mgr.remove(m_mesh->getResourceName().getName());
}



void gkMeshLoader::loadSubMesh(Ogre::SubMesh* submesh, gkSubMesh* gks)
{
	UTsize iBufSize = gks->getIndexBuffer().size() * 3, vBufSize = gks->getVertexBuffer().size();

	submesh->vertexData = new Ogre::VertexData();
	submesh->vertexData->vertexCount = vBufSize;

	// disable sharing
	submesh->useSharedVertices = false;

	// converting to tri list
	submesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

	UTsize offs = 0;

	// fill in the declaration
	Ogre::VertexDeclaration* decl = submesh->vertexData->vertexDeclaration;


	// position
	decl->addElement(0, offs, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offs += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	// no, blending weights

	// normals
	decl->addElement(0, offs, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	offs += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	// texture coordinates
	int maxTco = gks->getUvLayerCount();
	for (int lay = 0; lay < maxTco; ++lay)
	{
		decl->addElement(0, offs, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, lay);
		offs += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
	}

	bool diffuseVert = gks->hasVertexColors();
	if (diffuseVert)
	{
		// diffuse colours
		decl->addElement(0, offs, Ogre::VET_COLOUR_ABGR, Ogre::VES_DIFFUSE);
		offs += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR_ABGR);
	}

	// no, specular colours

	Ogre::HardwareVertexBufferSharedPtr vertBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(offs,
	        submesh->vertexData->vertexCount,
	        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);


	// bind the source
	Ogre::VertexBufferBinding* bind = submesh->vertexData->vertexBufferBinding;
	bind->setBinding(0, vertBuf);


	// index buffer
	Ogre::HardwareIndexBuffer::IndexType buff_type = (iBufSize > gk16BitClamp) ?
	        Ogre::HardwareIndexBuffer::IT_32BIT : Ogre::HardwareIndexBuffer::IT_16BIT;

	Ogre::HardwareIndexBufferSharedPtr indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(buff_type,
	        iBufSize,
	        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	//
	submesh->indexData->indexCount = iBufSize;
	submesh->indexData->indexBuffer = indexBuffer;


	// build index items
	{
		bool using32 = buff_type == Ogre::HardwareIndexBuffer::IT_32BIT;

		unsigned int* indices32 = 0;
		unsigned short* indices16 = 0;

		if (!using32)
			indices16 = static_cast<unsigned short*>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
		else
			indices32 = static_cast<unsigned int*>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));


		UTsize iBufTot = iBufSize / 3;

		gkTriangle* ibuf = gks->getIndexBuffer().ptr();

		for (UTsize cur = 0; cur < iBufTot; cur++)
		{
			const gkTriangle& i = ibuf[cur];
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
		unsigned char* vptr = static_cast<unsigned char*>(vertBuf->lock(Ogre::HardwareBuffer::HBL_NORMAL));

		Ogre::VertexData* vdata = submesh->vertexData;

		float*           fptr = 0;
		unsigned int*    iptr = 0;


		gkVertex* vbuf  = gks->getVertexBuffer().ptr();

		UTsize i = 0;
		while (i < vBufSize)
		{
			const gkVertex& vtx = vbuf[i++];

			// packed as
			// VES_POSITION | VES_NORMAL | VES_TEXTURE_COORDINATES[<8] | |= VES_DIFFUSE

			// VES_POSITION
			fptr = (float*)vptr;

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
					*fptr++ = vtx.uv[tc].x;
					*fptr++ = 1.f - vtx.uv[tc].y;
				}
			}

			if (diffuseVert)
			{
				// VES_DIFFUSE
				iptr    = (unsigned int*)fptr;
				*iptr++ = vtx.vcol;

				// goto next
				vptr = (unsigned char*)iptr;
			}
			else
				// goto next
				vptr = (unsigned char*)fptr;

		}

		vertBuf->unlock();
		gkSkeletonResource* skel = m_mesh->getSkeleton();


		if (skel)
		{
			gkSubMesh::DeformVerts& dvbuf = gks->getDeformVertexBuffer();


			i = 0;
			UTsize totvert = dvbuf.size();
			gkSubMesh::DeformVerts::Pointer dvp = dvbuf.ptr();
			while (i < totvert)
			{
				gkDeformVertex& dvtx = dvp[i++];


				Ogre::VertexBoneAssignment vba;
				gkVertexGroup* vg = m_mesh->findVertexGroup(dvtx.group);
				if (vg != 0)
				{
					gkBone* bone = skel->getBone(vg->getName());
					if (bone)
					{
						vba.boneIndex   = bone->_getBoneIndex();
						vba.vertexIndex = dvtx.vertexId;
						vba.weight      = dvtx.weight;
						submesh->addBoneAssignment(vba);
					}
				}
			}

			if (totvert > 0)
			{
#if OGRE_VERSION >= 0x10800
				Ogre::VertexDeclaration* newDecl = decl->getAutoOrganisedDeclaration(true, false, false);
#else
				Ogre::VertexDeclaration* newDecl = decl->getAutoOrganisedDeclaration(true, false);
#endif
				vdata->reorganiseBuffers(newDecl);
			}
		}
	}
}


void gkMeshLoader::loadResource(Ogre::Resource* res)
{

	Ogre::Mesh* omesh = static_cast<Ogre::Mesh*>(res);

	if (m_mesh->getSkeleton())
	{
		gkSkeletonResource* skelres = m_mesh->getSkeleton();
		omesh->setSkeletonName(skelres->getResourceName().getName());
	}



	gkMesh::SubMeshIterator iter = m_mesh->getSubMeshIterator();
	while (iter.hasMoreElements())
	{
		gkSubMesh* gks = iter.getNext();

		Ogre::SubMesh* submesh = omesh->createSubMesh();
		submesh->setMaterialName(gks->getMaterialName());

		gkMaterialLoader::loadSubMeshMaterial(gks, m_mesh->getGroupName());
		loadSubMesh(submesh, gks);

	}

	omesh->_setBounds(m_mesh->getBoundingBox(), false);
	omesh->_setBoundingSphereRadius(m_mesh->getBoundingBox().getSize().squaredLength());
}
