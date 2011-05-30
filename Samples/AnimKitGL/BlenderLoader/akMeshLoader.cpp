/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Xavier T.
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

#include "akMeshLoader.h"

#include "Blender.h"
#include "akBlenderDefines.h"

#include "akVertexGroup.h"
#include "akMorphTarget.h"

#include <stdio.h>

#define VEC3CPY(a, b) {a.setX(b[0]); a.setY(b[1]); a.setZ(b[2]);}
#define VEC2CPY(a, b) {a[0]= b[0]; a[1]= b[1];}

// float normal from short
#define VEC3CPN(a, b) {a.setX(b[0]/32767.f); a.setY(b[1]/32767.f); a.setZ(b[2]/32767.f);}



static void akMeshLoaderUtils_getLayers(
    Blender::Mesh* mesh,
    Blender::MTFace** eightLayerArray,
    Blender::MCol** oneMCol,
    int& validLayers)
{
	UT_ASSERT(mesh);

	validLayers = 0;

	Blender::CustomDataLayer* layers = (Blender::CustomDataLayer*)mesh->fdata.layers;
	if (layers)
	{
		// push valid layers
		for (int i = 0; i < mesh->fdata.totlayer && validLayers < 8; i++)
		{
			if (layers[i].type == CD_MTFACE && eightLayerArray)
			{
				Blender::MTFace* mtf = (Blender::MTFace*)layers[i].data;
				if (mtf)
					eightLayerArray[validLayers++] = mtf;
			}
			else if (layers[i].type == CD_MCOL)
			{
				if (oneMCol && !(*oneMCol))
					*oneMCol = static_cast<Blender::MCol*>(layers[i].data);
			}
		}
	}
	else
	{
		if (eightLayerArray && mesh->mtface)
			eightLayerArray[validLayers++] = mesh->mtface;
		if (oneMCol && mesh->mcol)
			*oneMCol = mesh->mcol;
	}
}


class akSubMeshHashKey
{
protected:

	friend class    akMeshLoader;

	short           m_matnr, m_mode;
	bool            m_blenderMat;

	Blender::Image* m_images[8];

public:
	akSubMeshHashKey()
		: m_matnr(0), m_mode(0), m_blenderMat(false)
	{
		for (int i = 0; i < 8; ++i) m_images[i] = 0;
	}

	// Blender materials
	akSubMeshHashKey(int mat_nr, int mode)
		: m_matnr(mat_nr), m_mode(0), m_blenderMat(true)
	{
		for (int i = 0; i < 8; ++i) m_images[i] = 0;
//		if (mode & TF_INVISIBLE)    m_mode |= gkMaterialProperties::MA_INVISIBLE;
//		if (mode & TF_TWOSIDE)      m_mode |= gkMaterialProperties::MA_TWOSIDE;
	}


	// Blender texture materials
	akSubMeshHashKey(int mode, int alpha, Blender::Image* images[8])
		: m_matnr(-1), m_mode(0), m_blenderMat(false)
	{
//		m_mode |= gkMaterialProperties::MA_RECEIVESHADOWS;
//		m_mode |= gkMaterialProperties::MA_DEPTHWRITE;

//		if (mode & TF_INVISIBLE)    m_mode |= gkMaterialProperties::MA_INVISIBLE;
//		if (mode & TF_LIGHT)        m_mode |= gkMaterialProperties::MA_LIGHTINGENABLED;
//		if (mode & TF_TWOSIDE)      m_mode |= gkMaterialProperties::MA_TWOSIDE;
//		if (alpha & TF_CLIP)        m_mode &= ~gkMaterialProperties::MA_DEPTHWRITE;
//		if (alpha & TF_ADD)         m_mode |= gkMaterialProperties::MA_ADDITIVEBLEND;
//		if (mode & TF_TEX)          m_mode |= gkMaterialProperties::MA_HASFACETEX;

//		if (alpha & TF_ALPHA || alpha & TF_CLIP)
//			m_mode |= gkMaterialProperties::MA_ALPHABLEND;
		for (int i = 0; i < 8; ++i) m_images[i] = images[i];
	}

	// Copy constructor
	akSubMeshHashKey(const akSubMeshHashKey& k)
		:   m_matnr(k.m_matnr), m_mode(k.m_mode),  m_blenderMat(k.m_blenderMat)

	{
		for (int i = 0; i < 8; ++i) m_images[i] = m_blenderMat ? 0 : k.m_images[i];
	}


	bool operator == (const akSubMeshHashKey& rhs) const
	{
		if (m_blenderMat)
			return m_matnr == rhs.m_matnr;

		return  m_mode == rhs.m_mode &&
		        m_images[0] == rhs.m_images[0] &&
		        m_images[1] == rhs.m_images[1] &&
		        m_images[2] == rhs.m_images[2] &&
		        m_images[3] == rhs.m_images[3] &&
		        m_images[4] == rhs.m_images[4] &&
		        m_images[5] == rhs.m_images[5] &&
		        m_images[6] == rhs.m_images[6] &&
		        m_images[7] == rhs.m_images[7];
	}
};


class akSubMeshPair
{
public:
	akSubMeshHashKey test;
	akSubMesh*     item;
	utArray<UTuint32> idxmap; // store the old index (in the blender mesh) of the vertex
	utArray<UTuint32> facemap; // store the old face index (in the blender mesh) of the vertex
	
	akSubMeshPair() : test(), item(0)
	{
	}
	akSubMeshPair(akSubMesh* cur) : test(), item(cur)
	{
	}

	~akSubMeshPair()
	{
		idxmap.clear();
		facemap.clear();
	}

	akSubMeshPair& operator = (const akSubMeshPair& p)
	{
		if (this != &p)
		{
			test = akSubMeshHashKey(p.test);
			item = p.item;
			idxmap = p.idxmap;
		}
		return *this;
	}

	bool operator == (const akSubMeshPair& rhs) const
	{
		return test == rhs.test;
	}

	unsigned int getVertexIndex(unsigned int fi, unsigned int index, const akMeshLoader::TempVert& ref)
	{
		UTsize size = item->getVertexCount();
		for(unsigned int i=0; i<size; i++)
		{
			if(idxmap[i] == index && vertEq(i,ref))
				return i;
		}
		addVertex(fi, index, ref);
		return size;
	}


	bool vertEq(const unsigned int index, const akMeshLoader::TempVert& b)
	{
		const akBufferInfo* vbi = item->getVertexBuffer();
		akVector3* co, *no;
		UTuint32* vcol;
		float* uvlayer;
		UTuint32 cos, nos, vcols, uvlayers;
		
		if(vbi->getElement(akBufferInfo::BI_DU_VERTEX, akBufferInfo::VB_DT_3FLOAT32, 1, (void**)&co, &cos))
		{
			akAdvancePointer(co, cos * index);
			if (!akFuzzyT(lengthSqr(co[0] - b.co), 1e-10))
				return false;
		}
		
		if(vbi->getElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, 1, (void**)&no, &nos))
		{
			akAdvancePointer(no, nos * index);
			if (!akFuzzyT(lengthSqr(no[0] - b.no), 1e-10))
				return false;
		}
		
		if(vbi->getElement(akBufferInfo::BI_DU_COLOR, akBufferInfo::VB_DT_UINT32, 1, (void**)&vcol, &vcols))
		{
			akAdvancePointer(vcol, vcols * index);
			if (vcol[0] != b.vcol)
				return false;
		}
		
		for (int i = 0; i < item->getUVLayerCount(); i++)
		{
			if(vbi->getElement(akBufferInfo::BI_DU_UV, akBufferInfo::VB_DT_2FLOAT32, i+1, (void**)&uvlayer, &uvlayers))
			{
				float* uv = uvlayer;
				akAdvancePointer(uv, uvlayers * index);;
				akScalar d1 = uv[0] - b.uv[i][0];
				akScalar d2 = uv[1] - b.uv[i][1];
				if (!akFuzzyT(d1*d1+d2*d2, 1e-10))
					return false;
			}
		}
		return true;
	}
	
	void addVertex(unsigned int fi, unsigned int index, const akMeshLoader::TempVert& ref)
	{
		utArray<float> uvs;
		for(int j=0; j<AK_UV_MAX; j++)
		{
			uvs.push_back(ref.uv[j][0]);
			uvs.push_back(ref.uv[j][1]);
		}
		item->addVertex(ref.co, ref.no, ref.vcol, uvs);
		idxmap.push_back(index);
		facemap.push_back(fi);
	}
};



akMeshLoader::akMeshLoader(akMesh* gmesh, Blender::Object* bobject, Blender::Mesh* bmesh)
	:   m_gmesh(gmesh), m_bmesh(bmesh), m_bobj(bobject)
{
}


akMeshLoader::~akMeshLoader()
{

}

void akMeshLoader::addTriangle(akSubMeshPair* subp,
							unsigned int fi,
							const TempVert& v0,
                            unsigned int i0,
                            const TempVert& v1,
                            unsigned int i1,
                            const TempVert& v2,
                            unsigned int i2)
{
	UTuint32 idx1, idx2, idx3;
	idx1 = subp->getVertexIndex(fi, i0, v0);
	idx2 = subp->getVertexIndex(fi, i1, v1);
	idx3 = subp->getVertexIndex(fi, i2, v2);
	subp->item->addIndex(idx1);
	subp->item->addIndex(idx2);
	subp->item->addIndex(idx3);
}

void akMeshLoader::convertIndexedTriangle(
    TempFace* dest,
    unsigned int i0,
    unsigned int i1,
    unsigned int i2,
    PackedFace& face)
{
	TempVert& v0 = dest->v0;
	TempVert& v1 = dest->v1;
	TempVert& v2 = dest->v2;

	dest->i0 = face.index[i0];
	dest->i1 = face.index[i1];
	dest->i2 = face.index[i2];

	v0.vcol = face.colors[i0];
	v1.vcol = face.colors[i1];
	v2.vcol = face.colors[i2];

	VEC3CPY(v0.co, face.verts[i0].co);
	VEC3CPY(v1.co, face.verts[i1].co);
	VEC3CPY(v2.co, face.verts[i2].co);
	VEC3CPN(v0.no, face.verts[i0].no);
	VEC3CPN(v1.no, face.verts[i1].no);
	VEC3CPN(v2.no, face.verts[i2].no);

	for (int i = 0; i < face.totlay; ++i)
	{
		VEC2CPY(v0.uv[i], face.uvLayers[i][i0]);
		VEC2CPY(v1.uv[i], face.uvLayers[i][i1]);
		VEC2CPY(v2.uv[i], face.uvLayers[i][i2]);
	}
}


akVector3 akMeshLoader::calcNormal(akVector3& v0, akVector3& v1, akVector3& v2)
{
	return normalize(cross((v1-v2),(v2-v0)));
}

void akMeshLoader::calcNormal(TempFace* tri)
{
	akVector3 n = calcNormal(tri->v0.co, tri->v1.co, tri->v2.co);
	tri->v0.no = tri->v1.no = tri->v2.no = n;
}

akVector3 akMeshLoader::calcMorphNormal(const Blender::MFace& bface, float* pos)
{
	akVector3 normal;
	if(bface.v4 != 0)
	{
		akVector3 e0, e1;
		akVector3 n1, n2;
		akVector3 v0(pos[bface.v1*3], pos[bface.v1*3+1], pos[bface.v1*3+2]);
		akVector3 v1(pos[bface.v2*3], pos[bface.v2*3+1], pos[bface.v2*3+2]);
		akVector3 v2(pos[bface.v3*3], pos[bface.v3*3+1], pos[bface.v3*3+2]);
		akVector3 v3(pos[bface.v4*3], pos[bface.v4*3+1], pos[bface.v4*3+2]);
		
		e0 = v0 - v2;
		e1 = v1 - v3;
		
		if (lengthSqr(e0) <lengthSqr(e1))
		{
			n1 = calcNormal(v0,v1,v2);
			n2 = calcNormal(v2,v3,v0);
		}
		else
		{
			n1 = calcNormal(v0, v1, v3);
			n2 = calcNormal(v3, v1, v2);
		}
		normal = normalize(n1+n2);
	}
	else
	{
		akVector3 v0(pos[bface.v1*3], pos[bface.v1*3+1], pos[bface.v1*3+2]);
		akVector3 v1(pos[bface.v2*3], pos[bface.v2*3+1], pos[bface.v2*3+2]);
		akVector3 v2(pos[bface.v3*3], pos[bface.v3*3+1], pos[bface.v3*3+2]);
		normal = calcNormal(v0,v1,v2);
	}
	return normal;
}

unsigned int akMeshLoader::packColour(const Blender::MCol& col, bool opengl)
{
	union
	{
		Blender::MCol col;
		unsigned int integer;
		unsigned char cp[4];
	} out_color, in_color;


	in_color.col = col;


	if (opengl) // abgr
	{
		out_color.cp[0] = in_color.cp[3]; // red
		out_color.cp[1] = in_color.cp[2]; // green
		out_color.cp[2] = in_color.cp[1]; // blue
		out_color.cp[3] = in_color.cp[0]; // alpha
	}
	else // argb
	{
		// vertex buffer is packed with VET_COLOUR_ABGR, swap b,r
		out_color.cp[2] = in_color.cp[3]; // red
		out_color.cp[1] = in_color.cp[2]; // green
		out_color.cp[0] = in_color.cp[1]; // blue
		out_color.cp[3] = in_color.cp[0]; // alpha
	}

	return out_color.integer;
}



int akMeshLoader::findTextureLayer(Blender::MTex* te)
{
	if (!(te->texco & TEXCO_UV) || te->uvname[0] == '\0')
		return 0;

	if (m_bmesh->fdata.layers)
	{
		Blender::CustomDataLayer* cd = (Blender::CustomDataLayer*)m_bmesh->fdata.layers;
		if (cd)
		{
			int layer = 0;
			for (int i = 0; i < m_bmesh->fdata.totlayer; i++)
			{
				if (cd[i].type == CD_MTFACE)
				{
					if (utCharEq(cd[i].name, te->uvname))
						return layer;
					++layer;
				}
			}
		}
	}
	return 0;

}


//void akMeshLoader::convertTextureFace(gkMaterialProperties& gma, akMeshHashKey& hk, Blender::Image** imas)
//{
//	gma.m_mode = hk.m_mode;
//	if (imas)
//	{
//		static char buf[32];
//		static int uid = 0;
	struct TempVert
	{
		akVector3       co;                 // vertex coordinates
		akVector3       no;                 // normals
		unsigned int    vcol;               // vertex color
		akScalar       uv[AK_UV_MAX][2];    // texture coordinates < GK_UV_MAX
	};

//		sprintf(buf, "TextureFace %i", (uid++));
//		gma.m_name = buf;
//	}

//	if (imas && gma.m_mode & gkMaterialProperties::MA_HASFACETEX)
//	{
//		gma.m_totaltex = 0;
//		for (int i = 0; i < 8; i++)
//		{
//			if (imas[i] != 0)
//			{
//				Blender::Image* ima = imas[i];
//				gkTextureProperties& gte = gma.m_textures[gma.m_totaltex++];
//				gte.m_layer = i;
//				gte.m_image = gte.m_name = GKB_IDNAME(ima);
//			}
//		}
//	}
//}

//int akMeshLoader::getRampBlendType(int blend)
//{
//	switch (blend)
//	{
//	case MA_RAMP_BLEND:   return GK_BT_MIXTURE;	
//	case MA_RAMP_ADD:     return GK_BT_ADDITIVE;	
//	case MA_RAMP_SUB:     return GK_BT_SUBTRACT;	
//	case MA_RAMP_SCREEN:  return GK_BT_SCREEN;	
//	case MA_RAMP_DARK:	  return GK_BT_DARKEN;	
//	case MA_RAMP_LIGHT:	  return GK_BT_LIGHTEN;	
//	case MA_RAMP_COLOR:   return GK_BT_COLOR;		
//	default:              break;
//	}

//	return GK_BT_MULTIPLY;	
//}

//int akMeshLoader::getTexBlendType(int blend)
//{
//	switch (blend)
//	{
//	case MTEX_BLEND:      return GK_BT_MIXTURE;	
//	case MTEX_ADD:        return GK_BT_ADDITIVE;	
//	case MTEX_SUB:        return GK_BT_SUBTRACT;	
//	case MTEX_SCREEN:	  return GK_BT_SCREEN;	
//	case MTEX_DARK:		  return GK_BT_DARKEN;	
//	case MTEX_LIGHT:	  return GK_BT_LIGHTEN;	
//	case MTEX_BLEND_VAL:  return GK_BT_COLOR;		
//	default:              break;
//	}

//	return GK_BT_MULTIPLY;	
//}

//void akMeshLoader::convertMaterial(Blender::Material* bma, gkMaterialProperties& gma, akMeshHashKey& hk)
//{
//	convertTextureFace(gma, hk, 0);

//	gma.m_name          = GKB_IDNAME(bma);
//	gma.m_hardness      = bma->har / 4.f;
//	gma.m_refraction    = bma->ref;
//	gma.m_emissive      = bma->emit;
//	gma.m_ambient       = bma->amb;
//	gma.m_spec          = bma->spec;
//	gma.m_alpha         = bma->alpha;
//	gma.m_diffuse       = gkColor(bma->r, bma->g, bma->b);
//	gma.m_specular      = gkColor(bma->specr, bma->specg, bma->specb);
//	gma.m_rblend		= getRampBlendType(bma->rampblend_col);

//	if (bma->mode & MA_ZTRA)        gma.m_mode |= gkMaterialProperties::MA_DEPTHWRITE;
//	if (bma->mode & MA_SHADOW)      gma.m_mode |= gkMaterialProperties::MA_RECEIVESHADOWS;
//	if (bma->mode & MA_WIRE)        gma.m_mode |= gkMaterialProperties::MA_WIREFRAME;
//	if (!(bma->mode & MA_SHLESS))   gma.m_mode |= gkMaterialProperties::MA_LIGHTINGENABLED;
//	if (bma->alpha <= 0.f)          gma.m_mode |= gkMaterialProperties::MA_INVISIBLE;
//	if (bma->mode & MA_RAMP_COL)	gma.m_mode |= gkMaterialProperties::MA_HASRAMPBLEND;


//	// textures
//	if (bma->mtex != 0)
//	{
//		gma.m_totaltex = 0;

//		for (int i = 0; i < MAX_MTEX; i++)
//		{
//			if (!bma->mtex[i] || !bma->mtex[i]->tex)
//				continue;

//			if (bma->mtex[i]->tex->type == TEX_IMAGE)
//			{
//				Blender::MTex* mtex = bma->mtex[i];

//				Blender::Image* ima = mtex->tex->ima;
//				if (!ima)
//					continue;

//				gkTextureProperties& gte = gma.m_textures[gma.m_totaltex++];
//				gte.m_image = gte.m_name = GKB_IDNAME(ima);

//				if (mtex->texflag & MTEX_STENCIL)
//				{
//					gte.m_mode |= gkTextureProperties::TM_SPLAT;
//					gte.m_texmode |= gkTextureProperties::TX_STENCIL;
//				}
//				if (mtex->texflag & MTEX_NEGATIVE)
//					gte.m_texmode |= gkTextureProperties::TX_NEGATIVE;
//				if (mtex->texflag &  MTEX_RGBTOINT)
//					gte.m_texmode |= gkTextureProperties::TX_RGBTOINTEN;

//				if (mtex->mapto & MAP_ALPHA)
//				{
//					gte.m_mode |= gkTextureProperties::TM_ALPHA;
//					gma.m_mode |= gkMaterialProperties::MA_ALPHABLEND;
//				}
//				if ((mtex->mapto & MAP_NORM) || (mtex->maptoneg & MAP_NORM))
//					gte.m_mode |= gkTextureProperties::TM_NORMAL;
//				if ((mtex->mapto & MAP_SPEC) || (mtex->maptoneg & MAP_SPEC))
//					gte.m_mode |= gkTextureProperties::TM_SPECULAR;
//				if ((mtex->mapto & MAP_REF)  || (mtex->maptoneg & MAP_REF))
//					gte.m_mode |= gkTextureProperties::TM_REFRACTION;
//				if ((mtex->mapto & MAP_EMIT) || (mtex->maptoneg & MAP_EMIT))
//					gte.m_mode |= gkTextureProperties::TM_EMMISIVE;

//				if (mtex->normapspace == MTEX_NSPACE_OBJECT) //else set to tagent space.
//					gte.m_texmode |= gkTextureProperties::TX_OBJ_SPACE;

//				gte.m_blend = getTexBlendType(mtex->blendtype);				

//				gte.m_layer = findTextureLayer(mtex);
//				gte.m_mix   = mtex->colfac;				

//				gte.m_normalFactor = mtex->norfac;
//				gte.m_diffuseColorFactor = mtex->colfac;
//				gte.m_diffuseAlpahFactor = mtex->alphafac;
//				gte.m_speculaColorFactor = mtex->colspecfac;
//				gte.m_speculaHardFactor = mtex->hardfac;
//			}
//		}
//	}
//}


void akMeshLoader::convertVertexGroups(akSubMeshPair *subpair)
{
	if(m_bmesh->dvert)
	{
		utArray<akVertexGroup*> vgroups;
		Blender::bDeformGroup* dg = (Blender::bDeformGroup*)m_bobj->defbase.first;
		while(dg)
		{
			akVertexGroup* vg = new akVertexGroup();
			vg->setName(dg->name);
			vgroups.push_back(vg);
			dg = dg->next;
		}
		
		akSubMesh* subm = subpair->item;
		for(int i=0; i<subm->getVertexCount(); i++)
		{
			UTuint32 oldi = subpair->idxmap.at(i);
			
			Blender::MDeformVert& dv = m_bmesh->dvert[oldi];
			for(int j=0;j<dv.totweight;j++)
			{
				UTuint32 vgi = dv.dw[j].def_nr;
				akVertexGroup* vg = vgroups.at(vgi);
				vg->add(i, dv.dw[j].weight);
			}
			
		}
		
		for(int i=0; i<vgroups.size(); i++)
		{
			if(vgroups[i]->getSize())
				subm->addVertexGroup(vgroups[i]);
			else
				delete vgroups[i];
		}
	}
}

void akMeshLoader::convertMorphTargets(akSubMeshPair *subpair)
{
	Blender::Key* bk = m_bmesh->key;
	if(bk)
	{
		akSubMesh* subm = subpair->item;
		const akBufferInfo* vbuffi = subm->getVertexBuffer();
		Blender::KeyBlock* bkb = (Blender::KeyBlock*)bk->block.first;
		
		// skip first shape key (basis)
		if(bkb) bkb = bkb->next;
		while(bkb)
		{
			if(bkb->type == KEY_RELATIVE)
			{
				Blender::KeyBlock* basis = (Blender::KeyBlock*)bk->block.first;
				for(int i=0; i<bkb->relative; i++) if(basis) basis = basis->next;
				
				if(basis)
				{
					akMorphTarget* mt = new akMorphTarget(true);
					mt->setName(bkb->name);
					
					float* kpos = (float*)bkb->data;
					float* bpos = (float*)basis->data;
					for(int bi=0; bi<bkb->totelem; bi++)
					{
						akVector3 k(kpos[0], kpos[1], kpos[2]);
						akVector3 b(bpos[0], bpos[1], bpos[2]);
						akVector3 offset = k-b;
						
						if(!akFuzzyT(lengthSqr(offset), 1e-10))
						{
							// position in shape key is different than in base
							// insert into target all vertices that was created form this original blender vertex
							for(int i=0; i<subpair->idxmap.size(); i++)
							{
								if(bi == subpair->idxmap[i])
								{
									UTuint32 bfi = subpair->facemap[i];
									const Blender::MFace& bface = m_bmesh->mface[bfi];
									float* pos = (float*)bkb->data;
									akVector3 normal(0,0,0);
									
									if(bface.flag & ME_SMOOTH)
									{
										btAlignedObjectArray<akVector3> norms;
										for (int bfi2 = 0; bfi2 < m_bmesh->totface; bfi2++)
										{
											const Blender::MFace& bface2 = m_bmesh->mface[bfi2];
											if( bface2.v1 == bi ||
												bface2.v2 == bi ||
												bface2.v3 == bi ||
												bface2.v4 == bi )
											{
												norms.push_back(calcMorphNormal(bface2, pos));
											}
										}
										for(int ni=0; ni<norms.size(); ni++)
										{
											normal += norms[ni];
										}
										normal = normalize( normal );
										norms.clear();
									}
									else
									{
										normal = calcMorphNormal(bface, pos);
									}
									
									akVector3* norms;
									UTuint32 normstride;
									vbuffi->getElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, 1, (void**)&norms, &normstride);
									akAdvancePointer(norms, normstride * i);
									akVector3 ndiff = normal - *norms;
									
									mt->add(i, offset, ndiff);
								}
							}
						}
						
						kpos+=3;
						bpos+=3;
					}
					
					if(mt->getSize())
					{
						subm->addMorphTarget(mt);
					}
					else
						delete mt;
				}
			}
			bkb = bkb->next;
		}
	}
}

void akMeshLoader::convert(bool sortByMat, bool openglVertexColor)
{
	Blender::MFace*  mface = m_bmesh->mface;
	Blender::MVert*  mvert = m_bmesh->mvert;
	Blender::MCol*   mcol =  0;
	Blender::MTFace* mtface[8] = {0, 0, 0, 0, 0, 0, 0, 0};


	if (!mface || !mvert)
		return;

	Blender::MVert          vpak[4];
	unsigned int            cpak[4];
	unsigned int            ipak[4];
	int                     totlayer;

	akSubMesh* curSubMesh = 0;
	utArray<akSubMeshPair*> meshtable;

	akMeshLoaderUtils_getLayers(m_bmesh, mtface, &mcol, totlayer);

	for (int fi = 0; fi < m_bmesh->totface; fi++)
	{
		const Blender::MFace& curface = mface[fi];

		// skip if face is not a triangle || quad
		if (!curface.v3)
			continue;

		const bool isQuad = curface.v4 != 0;

		TempFace t[2];
		PackedFace f;
		f.totlay = totlayer;

		if (isQuad)
		{
			vpak[0] = mvert[curface.v1];
			vpak[1] = mvert[curface.v2];
			vpak[2] = mvert[curface.v3];
			vpak[3] = mvert[curface.v4];

			ipak[0] = curface.v1;
			ipak[1] = curface.v2;
			ipak[2] = curface.v3;
			ipak[3] = curface.v4;

			if (mcol != 0)
			{
				cpak[0] = packColour(mcol[0], openglVertexColor);
				cpak[1] = packColour(mcol[1], openglVertexColor);
				cpak[2] = packColour(mcol[2], openglVertexColor);
				cpak[3] = packColour(mcol[3], openglVertexColor);
			}
			else
				cpak[0] = cpak[1] = cpak[2] = cpak[3] = 0xFFFFFFFF;


			for (int i = 0; i < totlayer; i++)
			{
				if (mtface[i] != 0)
				{
					VEC2CPY(f.uvLayers[i][0], mtface[i][fi].uv[0]);
					VEC2CPY(f.uvLayers[i][1], mtface[i][fi].uv[1]);
					VEC2CPY(f.uvLayers[i][2], mtface[i][fi].uv[2]);
					VEC2CPY(f.uvLayers[i][3], mtface[i][fi].uv[3]);
				}
			}
			f.verts     = vpak;
			f.index     = ipak;
			f.colors    = cpak;

			akVector3 e0, e1;
			akVector3 v0,v1,v2,v3;
			
			VEC3CPY(v0, mvert[curface.v1].co);
			VEC3CPY(v1, mvert[curface.v2].co);
			VEC3CPY(v2, mvert[curface.v3].co);
			VEC3CPY(v3, mvert[curface.v4].co);
			
			e0 = v0 - v2;
			e1 = v1 - v3;
			
			if (lengthSqr(e0) <lengthSqr(e1))
			{
				convertIndexedTriangle(&t[0], 0, 1, 2, f);
				convertIndexedTriangle(&t[1], 2, 3, 0, f);
			}
			else
			{
				convertIndexedTriangle(&t[0], 0, 1, 3, f);
				convertIndexedTriangle(&t[1], 3, 1, 2, f);
			}
		}
		else
		{
			for (int i = 0; i < totlayer; i++)
			{
				if (mtface[i] != 0)
				{
					VEC2CPY(f.uvLayers[i][0], mtface[i][fi].uv[0]);
					VEC2CPY(f.uvLayers[i][1], mtface[i][fi].uv[1]);
					VEC2CPY(f.uvLayers[i][2], mtface[i][fi].uv[2]);
				}
			}

			vpak[0] = mvert[curface.v1];
			vpak[1] = mvert[curface.v2];
			vpak[2] = mvert[curface.v3];

			ipak[0] = curface.v1;
			ipak[1] = curface.v2;
			ipak[2] = curface.v3;

			if (mcol != 0)
			{
				cpak[0] = packColour(mcol[0], openglVertexColor);
				cpak[1] = packColour(mcol[1], openglVertexColor);
				cpak[2] = packColour(mcol[2], openglVertexColor);
			}
			else
				cpak[0] = cpak[1] = cpak[2] = cpak[3] = 0xFFFFFFFF;

			f.verts     = vpak;
			f.index     = ipak;
			f.colors    = cpak;

			convertIndexedTriangle(&t[0], 0, 1, 2, f);
		}

		akSubMeshHashKey test;
		if (sortByMat)
		{
			int mode = 0;
			if (mtface[0])
				mode = mtface[0][fi].mode;

			test = akSubMeshHashKey(curface.mat_nr, mode);
		}
		else
		{
			Blender::Image* ima[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			for (int i = 0; i < totlayer; i++)
			{
				if (mtface[i] != 0)
					ima[i] = mtface[i][fi].tpage;
			}

			int mode = 0, alpha = 0;
			if (mtface[0])
			{
				mode    = mtface[0][fi].mode;
				alpha   = mtface[0][fi].transp;
			}

			test = akSubMeshHashKey(mode, alpha, ima);
		}

		// find submesh
		UTsize arpos = 0;
		akSubMeshPair* curSubMeshPair = 0;
		for(arpos=0; arpos<meshtable.size(); arpos++)
		{
			if(meshtable[arpos]->test == test)
				break;
		}
		if (arpos >= meshtable.size())
		{
			curSubMesh = new akSubMesh(akSubMesh::ME_TRIANGLES, true, true, totlayer);
			m_gmesh->addSubMesh(curSubMesh);
			
			curSubMeshPair = new akSubMeshPair(curSubMesh);
			meshtable.push_back(curSubMeshPair);
		}
		else
		{
			curSubMeshPair = meshtable.at(arpos);
			curSubMesh = curSubMeshPair->item;
		}

		if (curSubMesh == 0 || curSubMeshPair == 0) continue;


		if (!(curface.flag & ME_SMOOTH))
		{
			// face normal
			calcNormal(&t[0]);
			if (isQuad)
			{
				calcNormal(&t[1]);
				akVector3 n = normalize(t[0].v0.no + t[1].v0.no);
				t[0].v0.no = t[0].v1.no = t[0].v2.no = t[1].v0.no = t[1].v1.no = t[1].v2.no = n;
			}
		}

		addTriangle(curSubMeshPair, fi, t[0].v0, t[0].i0,
		                        t[0].v1, t[0].i1,
		                        t[0].v2, t[0].i2);

		if (isQuad)
		{
			addTriangle(curSubMeshPair, fi, t[1].v0, t[1].i0,
			                        t[1].v1, t[1].i1,
			                        t[1].v2, t[1].i2);

		}

		if (mcol)
			mcol += 4;

	}

	utArrayIterator<utArray<akSubMeshPair*> > iter(meshtable);

	while (iter.hasMoreElements())
	{
		akSubMeshPair* subpair = iter.peekNext();
		
		convertVertexGroups(subpair);
		convertMorphTargets(subpair);
		
//		akSubMeshHashKey& key  = iter.peekNext()->test;
//		akSubMesh* val      = iter.peekNext()->item;
//		Blender::Material* bmat = BlenderMaterial(m_bobj, key.m_matnr);
//		if (key.m_blenderMat)
//		{
//			if (bmat)
//				convertMaterial(bmat, val->getMaterial(), key);
//		}
//		else
//			convertTextureFace(val->getMaterial(), key, (Blender::Image**)key.m_images);

		delete subpair;
		iter.getNext();
	}
}


Blender::Material* akMeshLoader::getMaterial(Blender::Object* ob, int index)
{
	if (!ob || ob->totcol == 0) return 0;

	index = akClampi(index, 0, ob->totcol - 1);
	Blender::Material* ma = 0;

	int inObject = ob->matbits && ob->matbits[index] ? 1 : 0;

	if (!inObject)
		inObject = ob->colbits & (1 << index);


	if (inObject)
		ma = (Blender::Material*)ob->mat[index];
	else
	{
		Blender::Mesh* me = (Blender::Mesh*)ob->data;
		if (me && me->mat && me->mat[index])
			ma = me->mat[index];
	}

	return ma;
}


