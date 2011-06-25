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
#include "akBLoader.h"
#include "akTextureLoader.h"

#include "../akDemoBase.h"

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


Blender::Material* akMeshLoaderUtils_getMaterial(Blender::Object* ob, int index)
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

void akMeshLoaderUtils_getVertexGroups(Blender::Mesh* bmesh, Blender::Object* bobj, utArray<utString>& vgroups)
{
	if(bmesh->dvert)
	{
		Blender::bDeformGroup* dg = (Blender::bDeformGroup*)bobj->defbase.first;
		while(dg)
		{
			vgroups.push_back(dg->name);
			dg = dg->next;
		}
	}
}

akVector3 akMeshLoaderUtils_calcNormal(akVector3& v0, akVector3& v1, akVector3& v2)
{
	return normalize(cross((v1-v2),(v2-v0)));
}

akVector3 akMeshLoaderUtils_calcMorphNormal(const Blender::MFace& bface, float* pos)
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
			n1 = akMeshLoaderUtils_calcNormal(v0,v1,v2);
			n2 = akMeshLoaderUtils_calcNormal(v2,v3,v0);
		}
		else
		{
			n1 = akMeshLoaderUtils_calcNormal(v0, v1, v3);
			n2 = akMeshLoaderUtils_calcNormal(v3, v1, v2);
		}
		normal = normalize(n1+n2);
	}
	else
	{
		akVector3 v0(pos[bface.v1*3], pos[bface.v1*3+1], pos[bface.v1*3+2]);
		akVector3 v1(pos[bface.v2*3], pos[bface.v2*3+1], pos[bface.v2*3+2]);
		akVector3 v2(pos[bface.v3*3], pos[bface.v3*3+1], pos[bface.v3*3+2]);
		normal = akMeshLoaderUtils_calcNormal(v0,v1,v2);
	}
	return normal;
}

void akMeshLoaderUtils_getShapeKeys(Blender::Mesh* bmesh, utArray<utString>& shapes)
{
	Blender::Key* bk = bmesh->key;
	if(bk)
	{
		Blender::KeyBlock* bkb = (Blender::KeyBlock*)bk->block.first;
		
		// skip first shape key (basis)
		if(bkb) bkb = bkb->next;
		while(bkb)
		{
			if(bkb->type == KEY_RELATIVE)
			{
				Blender::KeyBlock* basis = (Blender::KeyBlock*)bk->block.first;
				for(int i=0; basis && i<bkb->relative; i++)
					basis = basis->next;
				
				if(basis)
					shapes.push_back(bkb->name);
			}
			bkb = bkb->next;
		}
	}
}

void akMeshLoaderUtils_getShapeKeysNormals(Blender::Mesh* bmesh, UTuint32 numshape, utArray<btAlignedObjectArray<akVector3> >& shapenormals)
{
	Blender::Key* bk = bmesh->key;
	if(bk)
	{
		
		shapenormals.resize(numshape);
		Blender::KeyBlock* bkb = (Blender::KeyBlock*)bk->block.first;
		
		// skip first shape key (basis)
		UTuint32 shape=0;
		if(bkb) bkb = bkb->next;
		while(bkb)
		{
			if(bkb->type == KEY_RELATIVE)
			{
				Blender::KeyBlock* basis = (Blender::KeyBlock*)bk->block.first;
				for(int i=0; basis && i<bkb->relative; i++)
					basis = basis->next;
				
				if(basis)
				{
					float* pos = (float*)bkb->data;
					shapenormals[shape].resize(bmesh->totface);
					for(int i=0; i<bmesh->totface; i++)
					{
						const Blender::MFace& bface = bmesh->mface[i];
						akVector3 normal = akMeshLoaderUtils_calcMorphNormal(bface, pos);
						shapenormals[shape][i]=normal;
					}
					shape++;
				}
			}
			bkb = bkb->next;
		}
	}
}

void akMeshLoaderUtils_getSmoothFaces(Blender::Mesh* bmesh, utArray<utArray<UTuint32> >& faces)
{
	faces.resize(bmesh->totvert);
	for (int i = 0; i< bmesh->totvert; i++)
	{
		for (int j = 0; j< bmesh->totface; j++)
		{
			const Blender::MFace& bface = bmesh->mface[j];
			if( (bface.flag & ME_SMOOTH) &&
					(bface.v1 == i ||
					 bface.v2 == i ||
					 bface.v3 == i ||
					 bface.v4 == i ))
			{
				faces[i].push_back(j);
			}
		}
	}
}

int akMeshLoaderUtils_getRampBlendType(int blend)
{
	switch (blend)
	{
	case MA_RAMP_BLEND:   return akColor::BT_MIXTURE;	
	case MA_RAMP_ADD:     return akColor::BT_ADDITIVE;	
	case MA_RAMP_SUB:     return akColor::BT_SUBTRACT;	
	case MA_RAMP_SCREEN:  return akColor::BT_SCREEN;	
	case MA_RAMP_DARK:	  return akColor::BT_DARKEN;	
	case MA_RAMP_LIGHT:	  return akColor::BT_LIGHTEN;	
	case MA_RAMP_COLOR:   return akColor::BT_COLOR;		
	default:              break;
	}
	return akColor::BT_MULTIPLY;	
}

int akMeshLoaderUtils_getTexBlendType(int blend)
{
	switch (blend)
	{
	case MTEX_BLEND:      return akColor::BT_MIXTURE;	
	case MTEX_ADD:        return akColor::BT_ADDITIVE;	
	case MTEX_SUB:        return akColor::BT_SUBTRACT;	
	case MTEX_SCREEN:	  return akColor::BT_SCREEN;	
	case MTEX_DARK:		  return akColor::BT_DARKEN;	
	case MTEX_LIGHT:	  return akColor::BT_LIGHTEN;	
	case MTEX_BLEND_VAL:  return akColor::BT_COLOR;		
	default:              break;
	}
	return akColor::BT_MULTIPLY;	
}


class akSubMeshHashKey
{
public:
	short           m_matnr, m_mode;
	bool            m_blenderMat;
	Blender::Image* m_images[8];

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
		if (mode & TF_INVISIBLE)    m_mode |= akMaterial::MA_INVISIBLE;
		if (mode & TF_TWOSIDE)      m_mode |= akMaterial::MA_TWOSIDE;
	}


	// Blender texture materials
	akSubMeshHashKey(int mode, int alpha, Blender::Image* images[8])
		: m_matnr(-1), m_mode(0), m_blenderMat(false)
	{
		m_mode |= akMaterial::MA_RECEIVESHADOWS;
		m_mode |= akMaterial::MA_DEPTHWRITE;

		if (mode & TF_INVISIBLE)    m_mode |= akMaterial::MA_INVISIBLE;
		if (mode & TF_LIGHT)        m_mode |= akMaterial::MA_LIGHTINGENABLED;
		if (mode & TF_TWOSIDE)      m_mode |= akMaterial::MA_TWOSIDE;
		if (alpha & TF_CLIP)        m_mode &= ~akMaterial::MA_DEPTHWRITE;
		if (alpha & TF_ADD)         m_mode |= akMaterial::MA_ADDITIVEBLEND;
		if (mode & TF_TEX)          m_mode |= akMaterial::MA_HASFACETEX;

		if (alpha & TF_ALPHA || alpha & TF_CLIP)
			m_mode |= akMaterial::MA_ALPHABLEND;
		for (int i = 0; i < 8; ++i) m_images[i] = images[i];
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
	Blender::Mesh* m_bmesh;
	utArray<UTuint32> idxmap; // store the old index (in the blender mesh) of the vertex
	
	utArray<utArray<UTuint32> >* smoothfacesarray;
	utArray<btAlignedObjectArray<akVector3> >* shapekeysnormals;
	
	akSubMeshPair() : test(), item(0)
	{
	}
	
	akSubMeshPair(akSubMesh* cur, Blender::Mesh* bmesh, utArray<utArray<UTuint32> >* smoothfaces, utArray<btAlignedObjectArray<akVector3> >* shapenorm)
		: test(), item(cur), m_bmesh(bmesh), smoothfacesarray(smoothfaces), shapekeysnormals(shapenorm)
	{
	}

	~akSubMeshPair()
	{
		idxmap.clear();
	}

	akSubMeshPair& operator = (const akSubMeshPair& p)
	{
		if (this != &p)
		{
			test = akSubMeshHashKey(p.test);
			item = p.item;
			idxmap = p.idxmap;
			smoothfacesarray = p.smoothfacesarray;
			shapekeysnormals = p.shapekeysnormals;
		}
		return *this;
	}

	bool operator == (const akSubMeshPair& rhs) const
	{
		return test == rhs.test;
	}

	unsigned int getVertexIndex(unsigned int fi, unsigned int bindex, const akMeshLoader::TempVert& ref)
	{
		UTsize size = item->getVertexCount();
		for(unsigned int i=0; i<size; i++)
		{
			if(idxmap[i] == bindex && vertEq(i,ref))
				return i;
		}
		addVertex(fi, bindex, ref);
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
			if (!akFuzzyT(lengthSqr(co[0] - b.co), 1e-10f))
				return false;
		}
		
		if(vbi->getElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, 1, (void**)&no, &nos))
		{
			akAdvancePointer(no, nos * index);
			if (!akFuzzyT(lengthSqr(no[0] - b.no), 1e-10f))
				return false;
		}
		
		if(vbi->getElement(akBufferInfo::BI_DU_COLOR, akBufferInfo::VB_DT_UINT32, 1, (void**)&vcol, &vcols))
		{
			akAdvancePointer(vcol, vcols * index);
			if (vcol[0] != b.vcol)
				return false;
		}
		
		for (unsigned int i = 0; i < item->getUVLayerCount(); i++)
		{
			if(vbi->getElement(akBufferInfo::BI_DU_UV, akBufferInfo::VB_DT_2FLOAT32, i+1, (void**)&uvlayer, &uvlayers))
			{
				float* uv = uvlayer;
				akAdvancePointer(uv, uvlayers * index);;
				akScalar d1 = uv[0] - b.uv[i][0];
				akScalar d2 = uv[1] - b.uv[i][1];
				if (!akFuzzyT(d1*d1+d2*d2, 1e-10f))
					return false;
			}
		}
		return true;
	}
	
	void addVertex(unsigned int fi, unsigned int bindex, const akMeshLoader::TempVert& ref)
	{
		utArray<float> uvs;
		for(int j=0; j<AK_UV_MAX; j++)
		{
			uvs.push_back(ref.uv[j][0]);
			uvs.push_back(ref.uv[j][1]);
		}
		UTuint32 id = item->addVertex(ref.co, ref.no, ref.vcol, uvs);
		idxmap.push_back(bindex);
		
		// vgroups
		if(m_bmesh->dvert)
		{
			Blender::MDeformVert& dv = m_bmesh->dvert[bindex];
			for(int j=0;j<dv.totweight;j++)
			{
				UTuint32 vgi = dv.dw[j].def_nr;
				if( vgi < item->getNumVertexGroups() )
				{
					akVertexGroup* vg = item->getVertexGroup(vgi);
					vg->add(id, dv.dw[j].weight);
				}
			}
		}
		
		// morphtargets
		if(m_bmesh->key)
		{
			Blender::KeyBlock* bkb = (Blender::KeyBlock*)m_bmesh->key->block.first;
			
			// skip first shape key (basis)
			int mti=0;
			if(bkb) bkb = bkb->next;
			while(bkb)
			{
				if(bkb->type == KEY_RELATIVE)
				{
					Blender::KeyBlock* basis = (Blender::KeyBlock*)m_bmesh->key->block.first;
					for(int i=0; basis && i<bkb->relative; i++)
						basis = basis->next;
					
					if(basis)
					{
						//akMorphTarget* mt = item->getMorphTarget(bkb->name);
						akMorphTarget* mt = item->getMorphTarget(mti);
						
						float* kpos = (float*)bkb->data;
						float* bpos = (float*)basis->data;
						
						akVector3 k(kpos[3*bindex+0], kpos[3*bindex+1], kpos[3*bindex+2]);
						akVector3 b(bpos[3*bindex+0], bpos[3*bindex+1], bpos[3*bindex+2]);
						k = k-b;
						
						btAlignedObjectArray<akVector3>& norms = shapekeysnormals->at(mti);
						akVector3 normal(0,0,0);
						
						const Blender::MFace& bface = m_bmesh->mface[fi];
						
						if(bface.flag & ME_SMOOTH)
						{
							utArray<UTuint32>& smoothfaces = smoothfacesarray->at(bindex);
							for (int j = 0; j< smoothfaces.size(); j++)
							{
								UTuint32 bface2id = smoothfaces[j];
								normal += norms.at(bface2id);
								
							}
							normal = normalize(normal);
						}
						else
						{
							normal = norms.at(fi);
						}
						normal = normal - ref.no;
						
						if(!akFuzzyT(lengthSqr(k), 1e-10f) || !akFuzzyT(lengthSqr(normal), 1e-10f))
							mt->add(id, k, normal);
						
						mti++;
					}
				}
				bkb = bkb->next;
			}
		}
	}
};



akMeshLoader::akMeshLoader(akDemoBase* demo, akMesh* gmesh, Blender::Object* bobject, Blender::Mesh* bmesh)
	: m_demo(demo), m_gmesh(gmesh), m_bmesh(bmesh), m_bobj(bobject)
{
}


akMeshLoader::~akMeshLoader()
{

}

void akMeshLoader::calcNormal(TempFace* tri)
{
	akVector3 n = akMeshLoaderUtils_calcNormal(tri->v0.co, tri->v1.co, tri->v2.co);
	tri->v0.no = tri->v1.no = tri->v2.no = n;
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

void akMeshLoader::convertTextureFace(akSubMeshPair* subpair)
{
	akMaterial& ma = subpair->item->getMaterial();
	akSubMeshHashKey& hk = subpair->test;
	Blender::Image** imas = subpair->test.m_images;
	
	ma.m_mode = hk.m_mode;

	if (imas && ma.m_mode & akMaterial::MA_HASFACETEX)
	{
//		ma.m_name = "TextureFace";
		ma.m_totaltex = 0;
		for (int i = 0; i < 8; i++)
		{
			if (imas[i] != 0)
			{
				akTexture tex;
//				tex.m_name = AKB_IDNAME(imas[i]);
				tex.m_image = AKB_IDNAME(imas[i]);
				tex.m_layer = i;
				ma.m_textures.push_back(tex);
				ma.m_totaltex++;
				
				if (!m_demo->getTexture(AKB_IDNAME(imas[i])))
				{
					akTextureLoader teconv(m_demo, imas[i]);
					teconv.load();
				}
			}
		}
	}
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

void akMeshLoader::convertMaterial(Blender::Material* bma, akSubMeshPair* subpair)
{
	akMaterial& ma = subpair->item->getMaterial();
	akSubMeshHashKey& hk = subpair->test;
	
	ma.m_mode = hk.m_mode;

//	ma.m_name          = GKB_IDNAME(bma);
	ma.m_hardness      = bma->har / 4.f;
	ma.m_refraction    = bma->ref;
	ma.m_emissive      = bma->emit;
	ma.m_ambient       = bma->amb;
	ma.m_spec          = bma->spec;
	ma.m_alpha         = bma->alpha;
	ma.m_diffuse       = akColor(bma->r, bma->g, bma->b);
	ma.m_specular      = akColor(bma->specr, bma->specg, bma->specb);
	ma.m_blend		= akMeshLoaderUtils_getRampBlendType(bma->rampblend_col);

	if (bma->mode & MA_ZTRA)        ma.m_mode |= akMaterial::MA_DEPTHWRITE;
	if (bma->mode & MA_SHADOW)      ma.m_mode |= akMaterial::MA_RECEIVESHADOWS;
	if (bma->mode & MA_WIRE)        ma.m_mode |= akMaterial::MA_WIREFRAME;
	if (!(bma->mode & MA_SHLESS))   ma.m_mode |= akMaterial::MA_LIGHTINGENABLED;
	if (bma->alpha <= 0.f)          ma.m_mode |= akMaterial::MA_INVISIBLE;
	if (bma->mode & MA_RAMP_COL)	ma.m_mode |= akMaterial::MA_HASRAMPBLEND;


	// textures
	if (bma->mtex != 0)
	{
		ma.m_totaltex = 0;

		for (int i = 0; i < MAX_MTEX; i++)
		{
			if (!bma->mtex[i] || !bma->mtex[i]->tex)
				continue;

			if (bma->mtex[i]->tex->type == TEX_IMAGE)
			{
				Blender::MTex* mtex = bma->mtex[i];

				Blender::Image* ima = mtex->tex->ima;
				if (!ima)
					continue;
				
				akTexture tex;
//				tex.m_name = AKB_IDNAME(ima);
				tex.m_image = AKB_IDNAME(ima);

				if (mtex->texflag & MTEX_STENCIL)
				{
					tex.m_mode |= akTexture::TM_SPLAT;
					tex.m_texmode |= akTexture::TX_STENCIL;
				}
				if (mtex->texflag & MTEX_NEGATIVE)
					tex.m_texmode |= akTexture::TX_NEGATIVE;
				if (mtex->texflag &  MTEX_RGBTOINT)
					tex.m_texmode |= akTexture::TX_RGBTOINTEN;

				if (mtex->mapto & MAP_ALPHA)
				{
					tex.m_mode |= akTexture::TM_ALPHA;
					ma.m_mode |= akMaterial::MA_ALPHABLEND;
				}
				if ((mtex->mapto & MAP_NORM) || (mtex->maptoneg & MAP_NORM))
					tex.m_mode |= akTexture::TM_NORMAL;
				if ((mtex->mapto & MAP_SPEC) || (mtex->maptoneg & MAP_SPEC))
					tex.m_mode |= akTexture::TM_SPECULAR;
				if ((mtex->mapto & MAP_REF)  || (mtex->maptoneg & MAP_REF))
					tex.m_mode |= akTexture::TM_REFRACTION;
				if ((mtex->mapto & MAP_EMIT) || (mtex->maptoneg & MAP_EMIT))
					tex.m_mode |= akTexture::TM_EMMISIVE;

				if (mtex->normapspace == MTEX_NSPACE_OBJECT) //else set to tagent space.
					tex.m_texmode |= akTexture::TX_OBJ_SPACE;

				tex.m_blend = akMeshLoaderUtils_getTexBlendType(mtex->blendtype);

				tex.m_layer = findTextureLayer(mtex);
				tex.m_mix   = mtex->colfac;

				tex.m_normalFactor = mtex->norfac;
				tex.m_diffuseColorFactor = mtex->colfac;
				tex.m_diffuseAlphaFactor = mtex->alphafac;
				tex.m_specularColorFactor = mtex->colspecfac;
				tex.m_specularHardFactor = mtex->hardfac;
				
				ma.m_textures.push_back(tex);
				ma.m_totaltex++;
			}
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
	utArray<utString> vgroups;
	utArray<utString> shapekeys;
	utArray<btAlignedObjectArray<akVector3> > shapekeysnormals;
	utArray<utArray<UTuint32> > smoothfacesarray;

	akMeshLoaderUtils_getLayers(m_bmesh, mtface, &mcol, totlayer);
	akMeshLoaderUtils_getVertexGroups(m_bmesh, m_bobj, vgroups);
	akMeshLoaderUtils_getShapeKeys(m_bmesh, shapekeys);
	if(shapekeys.size()>0)
	{
		akMeshLoaderUtils_getShapeKeysNormals(m_bmesh, shapekeys.size(), shapekeysnormals);
		akMeshLoaderUtils_getSmoothFaces(m_bmesh, smoothfacesarray);
	}
	
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
			
			curSubMeshPair = new akSubMeshPair(curSubMesh, m_bmesh, &smoothfacesarray, &shapekeysnormals);
			curSubMeshPair->test = test;
			
			for(int i=0;i<vgroups.size(); i++)
			{
				akVertexGroup* vg = new akVertexGroup();
				vg->setName(vgroups[i]);
				curSubMesh->addVertexGroup(vg);
			}
			
			for(int i=0;i<shapekeys.size(); i++)
			{
				akMorphTarget* mt = new akMorphTarget(true);
				mt->setName(shapekeys[i]);
				curSubMesh->addMorphTarget(mt);
			}
			
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
		
		if(subpair->test.m_blenderMat)
		{
			Blender::Material* bmat = akMeshLoaderUtils_getMaterial(m_bobj, subpair->test.m_matnr);
			if (bmat)
				convertMaterial(bmat, subpair);
		}
		else
			convertTextureFace(subpair);

		delete subpair;
		iter.getNext();
	}
	meshtable.clear();
	
	for(int i=0;i<vgroups.size(); i++)
	{
		vgroups[i].clear();
	}
	vgroups.clear();
	
	for(int i=0;i<shapekeys.size(); i++)
	{
		shapekeys[i].clear();
	}
	shapekeys.clear();
	
	for(int i=0;i<smoothfacesarray.size(); i++)
	{
		smoothfacesarray[i].clear();
	}
	smoothfacesarray.clear();
	
	for(int i=0;i<shapekeysnormals.size(); i++)
	{
		shapekeysnormals[i].clear();
	}
	shapekeysnormals.clear();
}


