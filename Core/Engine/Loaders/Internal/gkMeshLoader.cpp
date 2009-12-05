/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "OgreMath.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"
#include "OgreHardwareBufferManager.h"
#include "OgreSkeletonManager.h"
#include "OgreSkeleton.h"
#include "OgreBone.h"
#include "OgreStringConverter.h"

#include "gkLogger.h"
#include "gkSubMeshBuffer.h"
#include "gkMeshLoader.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkMaterialLoader.h"
#include "gkLoaderUtils.h"
#include "gkBlendLoader.h"
#include "gkBlenderDefines.h"
#include "gkGameObject.h"
#include "gkBlendFile.h"
#include "bBlenderFile.h"
#include "bMain.h"

#include "blender.h"


using namespace Ogre;

namespace Ogre {

typedef std::vector<int>								AssignmentIndexList;
typedef std::vector<Ogre::VertexBoneAssignment>		 AssignmentList;
typedef std::map<int, AssignmentList>				   AssignmentListMap;


//-----------------------------------------------------------------------------
class gkMeshLoaderPrivate
{
public:
	gkMeshLoaderPrivate(gkBlendFile *file, gkMeshLoader *parent, Blender::Object *ob,  Ogre::Mesh *mesh);

	void convertMesh(void);
	void convertSkeleton(Ogre::Skeleton *skel, AssignmentListMap &ret,  AssignmentIndexList& index);
	void findWeight(int index, class gkSubMeshBufferItem *sub, Ogre::SubMesh *me, AssignmentListMap &assign);


	Blender::Object *object;
	Ogre::String skeletonName;
	Ogre::Mesh *mesh;

	gkBlendFile *file;
	gkMeshLoader* loader;
};

}

//-----------------------------------------------------------------------------
static unsigned int PackColour(Blender::MCol col, bool opengl)
{
	union
	{
		Blender::MCol col;
		unsigned int integer;
		unsigned char cp[4];
	} out_color, in_color;

	in_color.col= col;
	out_color.cp[0]= in_color.cp[3]; // red
	out_color.cp[1]= in_color.cp[2]; // green
	out_color.cp[2]= in_color.cp[1]; // blue
	out_color.cp[3]= in_color.cp[0]; // alpha

	return out_color.integer;
}

bool gkSubMeshSlot::blender_mat = false;


//-----------------------------------------------------------------------------
gkMeshLoaderPrivate::gkMeshLoaderPrivate(gkBlendFile *f, gkMeshLoader *parent, Blender::Object *ob, Ogre::Mesh *m) :
	loader(parent), object(ob), file(f), mesh(m)
{
	GK_ASSERT(loader);
	GK_ASSERT(object);
	GK_ASSERT(file);
	GK_ASSERT(mesh);
}

//-----------------------------------------------------------------------------
void gkMeshLoaderPrivate::convertMesh(void)
{
	Blender::Mesh* bmesh= static_cast< Blender::Mesh*>(object->data);
	GK_ASSERT(bmesh);



	Blender::MFace* mface= bmesh->mface;
	Blender::MVert* mvert= bmesh->mvert;
	Blender::MCol*  mcol=  0;

	if (!mface || !mvert)
	{
		gkPrintf("Skipping mesh %s, it has no usable data", bmesh->id.name);
		return;
	}

	if ((bmesh->totvert) > LIMIT)
	{
		gkPrintf("Maximum vertex limit exceeded (%i)", LIMIT);
		return;
	}

	// notify that sorting based in materials 
	gkSubMeshSlot::blender_mat = gkEngine::getSingleton().getUserDefs().blendermat;


	gkLoaderUtils ut(file->getInternalFile());

	// Find Uv layers
	Blender::MTFace *layers[8]= {0, 0, 0, 0, 0, 0, 0, 0};
	int validLayers= 0;
	ut.getLayers(bmesh, (Blender::MTFace**)layers, &mcol, validLayers);

	/// set up skeleton
	AssignmentIndexList index;
	AssignmentListMap assignments;
	if (object->parent && object->parent->type == OB_ARMATURE)
	{
		index.resize(bmesh->totvert);
		SkeletonPtr skel= SkeletonManager::getSingleton().load(GKB_IDNAME(object->parent), mesh->getGroup());
		if (!skel.isNull())
			convertSkeleton(skel.getPointer(), assignments, index);
	}


	SubMeshBufferLookupMap bufferObjects;
	bufferObjects.reserve(15);


	/// parse all faces and convert
	for (int fi= 0; fi < bmesh->totface; fi++)
	{
		const Blender::MFace& curface= mface[fi];

		/// just an edge
		if (!curface.v3)
			continue;

		/// split condition
		gkSubMeshSlot tester;
		tester.material_nr= curface.mat_nr;
		tester.mode= 0;
		tester.alpha = 0;

		const bool isQuad= curface.v4 != 0;
		gkFaceBufferObject tri[2]= {gkFaceBufferObject(), gkFaceBufferObject()};

		if (isQuad)
		{
			unsigned int vcol[4]= {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
			if (mcol != 0)
			{
				vcol[0]= PackColour(mcol[0], true);
				vcol[1]= PackColour(mcol[1], true);
				vcol[2]= PackColour(mcol[2], true);
				vcol[3]= PackColour(mcol[3], true);
			}

			Blender::MVert verts[4];
			verts[0]= mvert[curface.v1];
			verts[1]= mvert[curface.v2];
			verts[2]= mvert[curface.v3];
			verts[3]= mvert[curface.v4];


			float va[3], vb[3];
			VEC3SUB2(verts[0].co, verts[1].co, va);
			VEC3SUB2(verts[2].co, verts[3].co, vb);

			float vael= VEC3DOT(va);
			float vbel= VEC3DOT(vb);

			/// split to tri (0,1,2) (2,3,0)
			if (vael < vbel)
			{
				tri[0].addVertex(0, VEC2F(verts[0].co)); tri[0].addVertex(1, VEC2F(verts[1].co)); tri[0].addVertex(2, VEC2F(verts[2].co));
				tri[1].addVertex(0, VEC2F(verts[2].co)); tri[1].addVertex(1, VEC2F(verts[3].co)); tri[1].addVertex(2, VEC2F(verts[0].co));

				tri[0].addNormal(0, verts[0].no); tri[0].addNormal(1, verts[1].no); tri[0].addNormal(2, verts[2].no);
				tri[1].addNormal(0, verts[2].no); tri[1].addNormal(1, verts[3].no); tri[1].addNormal(2, verts[0].no);

				tri[0].addColour(0, vcol[0]); tri[0].addColour(1, vcol[1]); tri[0].addColour(2, vcol[2]);
				tri[1].addColour(0, vcol[2]); tri[1].addColour(1, vcol[3]); tri[1].addColour(2, vcol[0]);

				tri[0].addIndex(0, curface.v1); tri[0].addIndex(1, curface.v2); tri[0].addIndex(2, curface.v3);
				tri[1].addIndex(0, curface.v3); tri[1].addIndex(1, curface.v4); tri[1].addIndex(2, curface.v1);

				for (int i= 0; i < validLayers; i++)
				{
					if (layers[i] != 0)
					{
						tri[0].addUvCord(0, i, (float*)layers[i][fi].uv[0]);
						tri[0].addUvCord(1, i, (float*)layers[i][fi].uv[1]);
						tri[0].addUvCord(2, i, (float*)layers[i][fi].uv[2]);

						tri[1].addUvCord(0, i, (float*)layers[i][fi].uv[2]);
						tri[1].addUvCord(1, i, (float*)layers[i][fi].uv[3]);
						tri[1].addUvCord(2, i, (float*)layers[i][fi].uv[0]);
					}
				}
				if (!index.empty())
				{
					tri[0].addVertexBoneAssignment(0, index[curface.v1]);
					tri[0].addVertexBoneAssignment(1, index[curface.v2]);
					tri[0].addVertexBoneAssignment(2, index[curface.v3]);

					tri[1].addVertexBoneAssignment(0, index[curface.v3]);
					tri[1].addVertexBoneAssignment(1, index[curface.v4]);
					tri[1].addVertexBoneAssignment(2, index[curface.v1]);
				}
			}

			/// split to tri (0,1,3) (3,1,2)
			else
			{
				tri[0].addVertex(0, VEC2F(verts[0].co)); tri[0].addVertex(1, VEC2F(verts[1].co)); tri[0].addVertex(2, VEC2F(verts[3].co));
				tri[1].addVertex(0, VEC2F(verts[3].co)); tri[1].addVertex(1, VEC2F(verts[1].co)); tri[1].addVertex(2, VEC2F(verts[2].co));

				tri[0].addNormal(0, verts[0].no); tri[0].addNormal(1, verts[1].no); tri[0].addNormal(2, verts[3].no);
				tri[1].addNormal(0, verts[3].no); tri[1].addNormal(1, verts[1].no); tri[1].addNormal(2, verts[2].no);

				tri[0].addColour(0, vcol[0]); tri[0].addColour(1, vcol[1]); tri[0].addColour(2, vcol[3]);
				tri[1].addColour(0, vcol[3]); tri[1].addColour(1, vcol[1]); tri[1].addColour(2, vcol[2]);


				tri[0].addIndex(0, curface.v1); tri[0].addIndex(1, curface.v2); tri[0].addIndex(2, curface.v4);
				tri[1].addIndex(0, curface.v4); tri[1].addIndex(1, curface.v2); tri[1].addIndex(2, curface.v3);

				for (int i= 0; i < validLayers; i++)
				{
					if (layers[i] != 0)
					{
						tri[0].addUvCord(0, i, (float*)layers[i][fi].uv[0]);
						tri[0].addUvCord(1, i, (float*)layers[i][fi].uv[1]);
						tri[0].addUvCord(2, i, (float*)layers[i][fi].uv[3]);

						tri[1].addUvCord(0, i, (float*)layers[i][fi].uv[3]);
						tri[1].addUvCord(1, i, (float*)layers[i][fi].uv[1]);
						tri[1].addUvCord(2, i, (float*)layers[i][fi].uv[2]);
					}
				}
				if (!index.empty())
				{
					tri[0].addVertexBoneAssignment(0, index[curface.v1]);
					tri[0].addVertexBoneAssignment(1, index[curface.v2]);
					tri[0].addVertexBoneAssignment(2, index[curface.v4]);

					tri[1].addVertexBoneAssignment(0, index[curface.v4]);
					tri[1].addVertexBoneAssignment(1, index[curface.v2]);
					tri[1].addVertexBoneAssignment(2, index[curface.v3]);
				}
			}
		}

		else
		{
			/// normal (0,1,2)
			unsigned int vcol[3]= {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
			if (mcol != 0)
			{
				vcol[0]= PackColour(mcol[0], true);
				vcol[1]= PackColour(mcol[1], true);
				vcol[2]= PackColour(mcol[2], true);
			}

			Blender::MVert verts[3];
			verts[0]= mvert[curface.v1];
			verts[1]= mvert[curface.v2];
			verts[2]= mvert[curface.v3];

			tri[0].addVertex(0, VEC2F(verts[0].co)); tri[0].addVertex(1, VEC2F(verts[1].co)); tri[0].addVertex(2, VEC2F(verts[2].co));
			tri[0].addNormal(0, verts[0].no); tri[0].addNormal(1, verts[1].no); tri[0].addNormal(2, verts[2].no);
			tri[0].addColour(0, vcol[0]); tri[0].addColour(1, vcol[1]); tri[0].addColour(2, vcol[2]);
			tri[0].addIndex(0, curface.v1); tri[0].addIndex(1, curface.v2); tri[0].addIndex(2, curface.v3);

			for (int i= 0; i < validLayers; i++)
			{
				if (layers[i] != 0)
				{
					tri[0].addUvCord(0, i, (float*)layers[i][fi].uv[0]);
					tri[0].addUvCord(1, i, (float*)layers[i][fi].uv[1]);
					tri[0].addUvCord(2, i, (float*)layers[i][fi].uv[2]);
				}
			}
			if (!index.empty())
			{
				tri[0].addVertexBoneAssignment(0, index[curface.v1]);
				tri[0].addVertexBoneAssignment(1, index[curface.v2]);
				tri[0].addVertexBoneAssignment(2, index[curface.v3]);

			}
		}

		if (layers[0] != 0)
		{
			tester.mode= layers[0][fi].mode;
			tester.alpha = layers[0][fi].transp;
		}

		/// tpage objects
		if (layers[0] != 0)
			tester.tpage[0]= layers[0][fi].tpage;
		if (layers[1] != 0)
			tester.tpage[1]= layers[1][fi].tpage;
		if (layers[2] != 0)
			tester.tpage[2]= layers[2][fi].tpage;
		if (layers[3] != 0)
			tester.tpage[3]= layers[3][fi].tpage;
		if (layers[4] != 0)
			tester.tpage[4]= layers[4][fi].tpage;
		if (layers[5] != 0)
			tester.tpage[5]= layers[5][fi].tpage;
		if (layers[6] != 0)
			tester.tpage[6]= layers[6][fi].tpage;
		if (layers[7] != 0)
			tester.tpage[7]= layers[7][fi].tpage;

		gkSubMeshBufferItem* foundItem= 0;


		SubMeshBufferLookupIterator iter= SubMeshBufferLookupIterator(bufferObjects);
		while (iter.hasMoreElements())
		{
			const gkSlotItemPair& slot= iter.peekNext();

			if (*slot.slot == tester)
			{
				foundItem= slot.item;
				break;
			}
			iter.getNext();
		}

		if (foundItem == 0)
		{
			foundItem= new gkSubMeshBufferItem();
			foundItem->submesh= mesh->createSubMesh();

			foundItem->ibuf.reserve(bmesh->totface);
			foundItem->vbuf.reserve(bmesh->totvert);

			/// track for next iteration
			gkSlotItemPair p= {new gkSubMeshSlot(tester), foundItem};
			bufferObjects.push_back(p);
		}

		tri[0].setMaxLayer(validLayers);
		if (isQuad)
			tri[1].setMaxLayer(validLayers);

		if (!(curface.flag & ME_SMOOTH))
		{
			tri[0].calcNormal();
			if (isQuad)
				tri[1].calcNormal();
		}

		foundItem->addTri(tri[0]);
		if (isQuad)
			foundItem->addTri(tri[1]);

		if (mcol != 0)
			mcol += 4;
	}

	/// Setup OGRE Mesh data
	MaterialManager &mgr= MaterialManager::getSingleton();

	HardwareBufferManager* bufferManager= HardwareBufferManager::getSingletonPtr();

	Vector3 min_aabb= Vector3(-0.5, -0.5, -0.5), max_aabb= Vector3(0.5, 0.5, 0.5);
	Real rad_aabb= 1.0;

	int count= 0;

	// extra setup of parent is an armature
	bool parentIsSkel= !skeletonName.empty();
	if (parentIsSkel)
		mesh->setSkeletonName(skeletonName);


	SubMeshBufferLookupIterator iter= SubMeshBufferLookupIterator(bufferObjects);
	while (iter.hasMoreElements())
	{
		const gkSubMeshSlot &slot= *(iter.peekNext().slot);
		gkSubMeshBufferItem *currentMesh= iter.peekNext().item;

		SubMesh *submesh= currentMesh->submesh;

		/// initialize the aabb
		if (count == 0)
		{
			const gkVertexBufferObject& vbo= currentMesh->vbuf[0];

			min_aabb= max_aabb= Vector3(vbo.vertex);
			rad_aabb= min_aabb.squaredLength();
		}

		/// build all materials for the mesh
		if (gkSubMeshSlot::blender_mat)
		{
			count ++;
			Blender::Material *me= ut.getMaterial(object,slot.material_nr);

			if (me != 0)
			{
				MaterialPtr ptr= mgr.getByName(GKB_IDNAME(me));

				// Share materials
				// Note if you have a material script defined,
				// place the name of it in the blender
				// text box and it will be used in place of the
				// standard material. (Remember: Texture unit names
				// from inside blender will be the name of the IM data block
				// minus the IM) -

				if (ptr.isNull())
				{
					ptr= mgr.create(GKB_IDNAME(me), mesh->getGroup());

					gkMaterialUtils util(file);
					util.getOgreMaterialFromMaterial(ptr, me, (Blender::Mesh*)object->data, slot.mode);
				}

				submesh->setMaterialName(GKB_IDNAME(me));
			}
			else
				submesh->setMaterialName("BaseWhiteNoLighting");
		}
		else
		{
			Blender::Material *me= ut.getMaterial(object,slot.material_nr);

			{
				/// naming scheme
				/// OB + / + MA + / unique count
				/// TODO, share somehow

				String materialName= mesh->getName() + "/";
				materialName += String(me ? GKB_IDNAME(me) : "NULL") + "/";
				materialName += StringConverter::toString(count++);


				if (mgr.resourceExists(materialName))
				{
					mgr.unload(materialName);
					mgr.remove(materialName);
				}

				/// create it
				MaterialPtr ptr= mgr.create(materialName, mesh->getGroup());
				gkMaterialUtils util(file);


				/// apply
				util.setOgreMaterialDefault(ptr, (slot.mode & TF_LIGHT) != 0, slot.mode, slot.alpha);

				/// slot.tpage will be in the uv chanel,
				/// so it's in it's own layer ;)
				if (slot.tpage[0] != 0)
					util.addTextureUnit(ptr, slot.tpage[0], TL_UV0);
				if (slot.tpage[1] != 0)
					util.addTextureUnit(ptr, slot.tpage[1], TL_UV1);
				if (slot.tpage[2] != 0)
					util.addTextureUnit(ptr, slot.tpage[2], TL_UV2);
				if (slot.tpage[3] != 0)
					util.addTextureUnit(ptr, slot.tpage[3], TL_UV3);
				if (slot.tpage[4] != 0)
					util.addTextureUnit(ptr, slot.tpage[4], TL_UV4);
				if (slot.tpage[5] != 0)
					util.addTextureUnit(ptr, slot.tpage[5], TL_UV5);
				if (slot.tpage[6] != 0)
					util.addTextureUnit(ptr, slot.tpage[6], TL_UV6);
				if (slot.tpage[7] != 0)
					util.addTextureUnit(ptr, slot.tpage[7], TL_UV7);

				submesh->setMaterialName(materialName);
			}
		}

		submesh->vertexData= new VertexData();
		submesh->vertexData->vertexCount= currentMesh->vbuf.size();

		/// disable sharing
		submesh->useSharedVertices= false;

		/// converting to tri list
		submesh->operationType= RenderOperation::OT_TRIANGLE_LIST;

		size_t offs= 0;

		/// fill in the declaration
		VertexDeclaration* decl= submesh->vertexData->vertexDeclaration;


		/// position
		decl->addElement(0, offs, VET_FLOAT3, VES_POSITION);
		offs += VertexElement::getTypeSize(VET_FLOAT3);

		/// no, blending weights

		/// normals
		decl->addElement(0, offs, VET_FLOAT3, VES_NORMAL);
		offs += VertexElement::getTypeSize(VET_FLOAT3);

		/// diffuse colours
		decl->addElement(0, offs, VET_COLOUR_ABGR, VES_DIFFUSE);
		offs += VertexElement::getTypeSize(VET_COLOUR_ABGR);

		/// no, specular colours


		/// texture coordinates
		for (int lay= 0; lay < validLayers; ++lay)
		{
			decl->addElement(0, offs, VET_FLOAT2, VES_TEXTURE_COORDINATES, TL_UV0 + lay);
			offs += VertexElement::getTypeSize(VET_FLOAT2);
		}



		/// the initial vertex buffer
		HardwareVertexBufferSharedPtr vertBuf= bufferManager->createVertexBuffer(offs,
											   submesh->vertexData->vertexCount,
											   HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		/// bind the source
		VertexBufferBinding* bind= submesh->vertexData->vertexBufferBinding;
		bind->setBinding(0, vertBuf);


		/// index buffer
		size_t indx_size= currentMesh->ibuf.size() * 3;

		HardwareIndexBuffer::IndexType buff_type= (indx_size > 65536) ?
				HardwareIndexBuffer::IT_32BIT : HardwareIndexBuffer::IT_16BIT;

		HardwareIndexBufferSharedPtr indexBuffer= bufferManager->createIndexBuffer(buff_type, indx_size,
				HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		///
		submesh->indexData->indexCount= indx_size;
		submesh->indexData->indexBuffer= indexBuffer;


		/// build index items
		{
			bool using32= buff_type == HardwareIndexBuffer::IT_32BIT;

			unsigned int *indices32= 0;
			unsigned short *indices16= 0;

			if (!using32)
				indices16= static_cast<unsigned short*>(indexBuffer->lock(HardwareBuffer::HBL_NORMAL));
			else
				indices32= static_cast<unsigned int*>(indexBuffer->lock(HardwareBuffer::HBL_NORMAL));


			size_t ele_len= currentMesh->ibuf.size();
			gkIndexBufferItem *ibuf= currentMesh->ibuf.ptr();

			for (size_t cur= 0; cur < ele_len; cur++)
			{
				const gkIndexBufferItem& i= ibuf[cur];
				if (using32)
				{
					*indices32++= (unsigned int)i.v1;
					*indices32++= (unsigned int)i.v2;
					*indices32++= (unsigned int)i.v3;
				}
				else
				{
					*indices16++= (unsigned short)i.v1;
					*indices16++= (unsigned short)i.v2;
					*indices16++= (unsigned short)i.v3;
				}
				if (parentIsSkel)
				{
					findWeight((int)i.v1, currentMesh, submesh, assignments);
					findWeight((int)i.v2, currentMesh, submesh, assignments);
					findWeight((int)i.v3, currentMesh, submesh, assignments);
				}
			}

			indexBuffer->unlock();
		}

		/// build vertex items
		{
			unsigned char *vertdata= static_cast<unsigned char *>(vertBuf->lock(HardwareBuffer::HBL_NORMAL));

			VertexData *vdata= submesh->vertexData;

			VertexDeclaration* decl= vdata->vertexDeclaration;
			VertexDeclaration::VertexElementList elems= decl->findElementsBySource(0);


			float *floatdata= 0;
			size_t ele_len= currentMesh->vbuf.size();
			gkVertexBufferObject* vbuf= currentMesh->vbuf.ptr();

			for (size_t cur= 0; cur < ele_len; ++cur)
			{
				const gkVertexBufferObject &vbo= vbuf[cur];
				int layer_count= 0;
				int offset_texcoord= 0;

				VertexDeclaration::VertexElementList::const_iterator ele= elems.begin(), eleend= elems.end();
				for (;ele != eleend; ++ele)
				{
					const VertexElement &vert= (*ele);

					int sem= vert.getSemantic();

					if (sem == VES_POSITION)
					{
						vert.baseVertexPointerToElement(vertdata, &floatdata);

						Vector3 pos= Vector3(vbo.vertex);

						*floatdata++= pos.x;
						*floatdata++= pos.y;
						*floatdata++= pos.z;

						min_aabb.makeFloor(pos);
						max_aabb.makeCeil(pos);
						rad_aabb= std::max(pos.squaredLength(), rad_aabb);
					}
					else if (sem == VES_NORMAL)
					{
						vert.baseVertexPointerToElement(vertdata, &floatdata);

						*floatdata++= vbo.normal[0];
						*floatdata++= vbo.normal[1];
						*floatdata++= vbo.normal[2];
					}
					else if (sem == VES_TEXTURE_COORDINATES)
					{
						vert.baseVertexPointerToElement(vertdata, &floatdata);

						layer_count = gkClamp(layer_count, TL_UV0, TL_UV7);
						*floatdata++= vbo.uv[layer_count][0];
						*floatdata++= 1.f - vbo.uv[layer_count][1];

						layer_count ++;
					}
					else if (sem == VES_DIFFUSE)
					{
						unsigned int *intdata= 0;
						vert.baseVertexPointerToElement(vertdata, &intdata);
						*intdata++= vbo.colour;
					}
				}

				vertdata += vertBuf->getVertexSize();
			}

			vertBuf->unlock();
			if (parentIsSkel)
			{
				VertexDeclaration* newDecl= decl->getAutoOrganisedDeclaration(true, false);
				vdata->reorganiseBuffers(newDecl);
			}
		}


		iter.getNext();
	}

	/// clear temp
	SubMeshBufferLookupIterator nit= SubMeshBufferLookupIterator(bufferObjects);
	while (nit.hasMoreElements())
	{
		delete nit.peekNext().slot;
		delete nit.peekNext().item;
		nit.getNext();
	}

	mesh->_setBounds(AxisAlignedBox(min_aabb, max_aabb), false);
	mesh->_setBoundingSphereRadius(Math::Sqrt(rad_aabb));

}


//-----------------------------------------------------------------------------
void gkMeshLoaderPrivate::convertSkeleton(Skeleton *skel, AssignmentListMap &ret, AssignmentIndexList& indexl)
{
	Blender::Mesh *me= (Blender::Mesh*)object->data;
	if (!me->dvert)
		return;

	skeletonName= skel->getName();
	gkLoaderUtils ut(file->getInternalFile());

	unsigned short size= skel->getNumBones();
	for (unsigned short b= 0; b < size; b++)
	{
		Bone *bone= skel->getBone(b);

		int index= ut.getDeformGroupIndex(object, bone->getName().c_str());

		Blender::MDeformVert *dvert= me->dvert;
		for (int n= 0; n < me->totvert; n++, ++dvert)
		{
			for (int w= 0; w < dvert->totweight; w++)
			{
				if (dvert->dw[w].def_nr == index)
				{
					Real weight= dvert->dw[w].weight;

					if (weight > 0 && weight > std::numeric_limits<Real>::epsilon())
					{
						VertexBoneAssignment vba;
						vba.vertexIndex= 0;
						vba.boneIndex= bone->getHandle();
						vba.weight= dvert->dw[w].weight;
						ret[n].push_back(vba);
						indexl[n]= n;
					}
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
void gkMeshLoaderPrivate::findWeight(int index, gkSubMeshBufferItem *sub, SubMesh *me, AssignmentListMap &assign)
{
	AssignmentListMap::iterator it= assign.find(sub->vbuf[index].vba_index);
	if (it == assign.end())
		return;

	AssignmentList list= it->second;
	for (size_t i= 0; i < list.size(); i++)
	{
		VertexBoneAssignment vba= list[i];
		// apply the ogre side index
		vba.vertexIndex= index;
		me->useSharedVertices ? mesh->addBoneAssignment(vba) : me->addBoneAssignment(vba);
	}
}



//-----------------------------------------------------------------------------
gkMeshLoader::gkMeshLoader(gkBlendFile *fp, Blender::Object *ob) :
		mFile(fp), mObject(ob)
{
	GK_ASSERT(mFile);
	GK_ASSERT(mObject);
}

//-----------------------------------------------------------------------------
gkMeshLoader::~gkMeshLoader()
{
}

//-----------------------------------------------------------------------------
void gkMeshLoader::loadResource(Resource* resource)
{
	GK_ASSERT(mFile);
	GK_ASSERT(mObject);
	GK_ASSERT(resource);

	gkMeshLoaderPrivate converter(mFile,  this, mObject, static_cast<Mesh*>(resource));
	converter.convertMesh();
}
