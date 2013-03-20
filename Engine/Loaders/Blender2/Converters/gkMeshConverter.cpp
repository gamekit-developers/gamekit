/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha, Alberto Torres Ruiz.
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
#include "Blender.h"
#include "gkBlenderDefines.h"
#include "gkMeshConverter.h"
#include "OgreKit.h"


#define VEC3CPY(a, b) {a.x= b[0]; a.y= b[1]; a.z= b[2];}
#define VEC2CPY(a, b) {a.x= b[0]; a.y= b[1];}

// float normal from short
#define VEC3CPN(a, b) {a.x= (b[0]/32767.f); a.y= (b[1]/32767.f); a.z= (b[2]/32767.f);}


static void gkLoaderUtils_getLayers_legacy(
    Blender::Mesh* mesh,
    Blender::MTFace** eightLayerArray,
    Blender::MCol** oneMCol,
    int& validLayers);

#if !defined(OGREKIT_USE_BPARSE) || BPARSE_FILE_FORMAT==BPARSE_FILEFORMAT_263
#define BMESH 1
static void gkLoaderUtils_getLayers_bmesh(
    Blender::Mesh* mesh,
    Blender::MTexPoly** eightLayerArray,
    Blender::MLoopUV** uvEightLayerArray,
    Blender::MLoopCol** oneMCol,
    int& validLayers);
#endif



class gkMeshHashKey
{
protected:

	friend class    gkBlenderMeshConverter;

	short           m_matnr, m_mode;
	bool            m_blenderMat;

	Blender::Image* m_images[8];

public:
	gkMeshHashKey()
		: m_matnr(0), m_mode(0), m_blenderMat(false)
	{
		for (int i = 0; i < 8; ++i) m_images[i] = 0;
	}

	// Blender materials
	gkMeshHashKey(int mat_nr, int mode)
		: m_matnr(mat_nr), m_mode(0), m_blenderMat(true)
	{
		for (int i = 0; i < 8; ++i) m_images[i] = 0;
		if (mode & TF_INVISIBLE)    m_mode |= gkMaterialProperties::MA_INVISIBLE;
		if (mode & TF_TWOSIDE)      m_mode |= gkMaterialProperties::MA_TWOSIDE;
	}


	// Blender texture materials
	gkMeshHashKey(int mode, int alpha, Blender::Image* images[8])
		: m_matnr(-1), m_mode(0), m_blenderMat(false)
	{
		m_mode |= gkMaterialProperties::MA_RECEIVESHADOWS;
		m_mode |= gkMaterialProperties::MA_DEPTHWRITE;

		if (mode & TF_INVISIBLE)    m_mode |= gkMaterialProperties::MA_INVISIBLE;
		if (mode & TF_LIGHT)        m_mode |= gkMaterialProperties::MA_LIGHTINGENABLED;
		if (mode & TF_TWOSIDE)      m_mode |= gkMaterialProperties::MA_TWOSIDE;
		if (alpha & TF_CLIP)        m_mode &= ~gkMaterialProperties::MA_DEPTHWRITE;
		if (alpha & TF_ADD)         m_mode |= gkMaterialProperties::MA_ADDITIVEBLEND;
		if (mode & TF_TEX)          m_mode |= gkMaterialProperties::MA_HASFACETEX;

		if ((alpha & TF_ALPHA) || (alpha & TF_CLIP) )
			m_mode |= gkMaterialProperties::MA_ALPHABLEND;
		for (int i = 0; i < 8; ++i) m_images[i] = images[i];
	}

	// Copy constructor
	gkMeshHashKey(const gkMeshHashKey& k)
		:   m_matnr(k.m_matnr), m_mode(k.m_mode),  m_blenderMat(k.m_blenderMat)

	{
		for (int i = 0; i < 8; ++i) m_images[i] = m_blenderMat ? 0 : k.m_images[i];
	}


	bool operator == (const gkMeshHashKey& rhs) const
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


class gkMeshPair
{
public:
	gkMeshHashKey test;
	gkSubMesh*     item;

	gkMeshPair() : test(), item(0)
	{
	}
	gkMeshPair(gkSubMesh* cur) : test(), item(cur)
	{
	}

	gkMeshPair& operator = (const gkMeshPair& p)
	{
		if (this != &p)
		{
			test = gkMeshHashKey(p.test);
			item = p.item;
		}
		return *this;
	}

	bool operator == (const gkMeshPair& rhs) const
	{
		return test == rhs.test;
	}

};


gkBlenderMeshConverter::gkBlenderMeshConverter(gkMesh* gmesh, Blender::Object* bobject, Blender::Mesh* bmesh)
	:   m_gmesh(gmesh), m_bmesh(bmesh), m_bobj(bobject)
{
}


gkBlenderMeshConverter::~gkBlenderMeshConverter()
{
}



void gkBlenderMeshConverter::convertIndexedTriangle(
    TempFace* dest,
    unsigned int i0,
    unsigned int i1,
    unsigned int i2,
    PackedFace& face)
{
	gkVertex& v0 = dest->v0;
	gkVertex& v1 = dest->v1;
	gkVertex& v2 = dest->v2;

	dest->i0 = face.index[i0];
	dest->i1 = face.index[i1];
	dest->i2 = face.index[i2];

	v0.vba = face.index[i0];
	v1.vba = face.index[i1];
	v2.vba = face.index[i2];

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


void gkBlenderMeshConverter::calcNormal(TempFace* tri)
{
	gkVector3 n = (tri->v1.co - tri->v2.co).crossProduct(tri->v2.co - tri->v0.co).normalisedCopy();
	tri->v0.no = tri->v1.no = tri->v2.no = n;
}



unsigned int gkBlenderMeshConverter::packColourABGR(const Blender::MCol& col)
{
	union
	{
		Blender::MCol col;
		unsigned int integer;
		unsigned char cp[4];
	} out_color, in_color;

	in_color.col = col;

//	if (opengl) // abgr
	
	out_color.cp[0] = in_color.cp[3]; // red
	out_color.cp[1] = in_color.cp[2]; // green
	out_color.cp[2] = in_color.cp[1]; // blue
	out_color.cp[3] = in_color.cp[0]; // alpha

// TODO: check how it works on DirectX (ARGB), also may need to
//       modify gkOgreMeshLoader.cpp
// NOTE: convert_bmesh is not using this function at the moment,
//       it's using a simple cast instead (assuming little endian)

// 	else // argb
// 	{
// 		// vertex buffer is packed with VET_COLOUR_ABGR, swap b,r
// 		out_color.cp[2] = in_color.cp[3]; // red
// 		out_color.cp[1] = in_color.cp[2]; // green
// 		out_color.cp[0] = in_color.cp[1]; // blue
// 		out_color.cp[3] = in_color.cp[0]; // alpha
// 	}

	return out_color.integer;
}



int gkBlenderMeshConverter::findTextureLayer(Blender::MTex* te)
{
	if (!(te->texco & TEXCO_UV) || te->uvname[0] == '\0')
		return 0;

	Blender::CustomData* data=0;

	if (m_bmesh->fdata.layers)
		data = &m_bmesh->fdata;
	if (m_bmesh->pdata.layers)
		data = &m_bmesh->pdata;

	if (data)
	{
		Blender::CustomDataLayer* cd = (Blender::CustomDataLayer*)data->layers;
		if (cd)
		{
			int layer = 0;
			for (int i = 0; i < data->totlayer; i++)
			{
				if (cd[i].type == CD_MTFACE || cd[i].type == CD_MTEXPOLY)
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


void gkBlenderMeshConverter::convertTextureFace(gkMaterialProperties& gma, gkMeshHashKey& hk, Blender::Image** imas)
{
	gma.m_mode = hk.m_mode;
	if (imas)
	{
		static char buf[32];
		static int uid = 0;

		sprintf(buf, "TextureFace %i", (uid++));
		gma.m_name = buf;
	}

	if (imas && gma.m_mode & gkMaterialProperties::MA_HASFACETEX)
	{
		gma.m_totaltex = 0;
		for (int i = 0; i < 8; i++)
		{
			if (imas[i] != 0)
			{
				Blender::Image* ima = imas[i];
				gkTextureProperties& gte = gma.m_textures[gma.m_totaltex++];
				gte.m_layer = i;
				gte.m_image = gte.m_name = GKB_IDNAME(ima);
			}
		}
	}
}

int gkBlenderMeshConverter::getRampBlendType(int blend)
{
	switch (blend)
	{
	case MA_RAMP_BLEND:   return GK_BT_MIXTURE;	
	case MA_RAMP_ADD:     return GK_BT_ADDITIVE;	
	case MA_RAMP_SUB:     return GK_BT_SUBTRACT;	
	case MA_RAMP_SCREEN:  return GK_BT_SCREEN;	
	case MA_RAMP_DARK:	  return GK_BT_DARKEN;	
	case MA_RAMP_LIGHT:	  return GK_BT_LIGHTEN;	
	case MA_RAMP_COLOR:   return GK_BT_COLOR;		
	default:              break;
	}

	return GK_BT_MULTIPLY;	
}

int gkBlenderMeshConverter::getTexBlendType(int blend)
{
	switch (blend)
	{
	case MTEX_BLEND:      return GK_BT_MIXTURE;	
	case MTEX_ADD:        return GK_BT_ADDITIVE;	
	case MTEX_SUB:        return GK_BT_SUBTRACT;	
	case MTEX_SCREEN:	  return GK_BT_SCREEN;	
	case MTEX_DARK:		  return GK_BT_DARKEN;	
	case MTEX_LIGHT:	  return GK_BT_LIGHTEN;	
	case MTEX_BLEND_VAL:  return GK_BT_COLOR;		
	default:              break;
	}

	return GK_BT_MULTIPLY;	
}

void gkBlenderMeshConverter::convertMaterial(Blender::Material* bma, gkMaterialProperties& gma, gkMeshHashKey& hk)
{
	convertTextureFace(gma, hk, 0);

	gma.m_name          = GKB_IDNAME(bma);
	gma.m_hardness      = bma->har / 4.f;
	gma.m_refraction    = bma->ref;
	gma.m_emissive      = bma->emit;
	gma.m_ambient       = bma->amb;
	gma.m_spec          = bma->spec;
	gma.m_alpha         = bma->alpha;
	gma.m_diffuse       = gkColor(bma->r, bma->g, bma->b);
	gma.m_specular      = gkColor(bma->specr, bma->specg, bma->specb);
	gma.m_rblend        = getRampBlendType(bma->rampblend_col);

	if (bma->mode & MA_ZTRA)        		gma.m_mode |= gkMaterialProperties::MA_DEPTHWRITE;
	if (bma->mode & MA_SHADOW)      		gma.m_mode |= gkMaterialProperties::MA_RECEIVESHADOWS;
	if (bma->mode & MA_WIRE)        		gma.m_mode |= gkMaterialProperties::MA_WIREFRAME;
	if (!(bma->mode & MA_SHLESS))   		gma.m_mode |= gkMaterialProperties::MA_LIGHTINGENABLED;
	// TODO: this need to be checked, as there are cases where material-alpha is set to zero to give the texture-alpha full control
	if (bma->alpha <= 0.f)          		gma.m_mode |= gkMaterialProperties::MA_INVISIBLE;
	if (bma->mode & MA_RAMP_COL)			gma.m_mode |= gkMaterialProperties::MA_HASRAMPBLEND;
	if (!(bma->game.flag & GEMAT_BACKCULL)) gma.m_mode |= gkMaterialProperties::MA_TWOSIDE;
	if (bma->game.flag & GEMAT_INVISIBLE)   gma.m_mode |= gkMaterialProperties::MA_INVISIBLE;

	if (bma->game.alpha_blend & GEMAT_ALPHA)
		gma.m_mode |= gkMaterialProperties::MA_ALPHABLEND;

	if (bma->game.alpha_blend & GEMAT_CLIP)
		gma.m_mode |= gkMaterialProperties::MA_ALPHACLIP;

	// textures
	if (bma->mtex != 0)
	{
		gma.m_totaltex = 0;

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

				gkTextureProperties& gte = gma.m_textures[gma.m_totaltex++];
				gte.m_image = gte.m_name = GKB_IDNAME(ima);

				if (mtex->texflag & MTEX_STENCIL)
				{
					gte.m_mode |= gkTextureProperties::TM_SPLAT;
					gte.m_texmode |= gkTextureProperties::TX_STENCIL;
				}
				if (mtex->texflag & MTEX_NEGATIVE)
					gte.m_texmode |= gkTextureProperties::TX_NEGATIVE;
				if (mtex->texflag &  MTEX_RGBTOINT)
					gte.m_texmode |= gkTextureProperties::TX_RGBTOINTEN;

				if (mtex->mapto & MAP_ALPHA)
				{
					gte.m_mode |= gkTextureProperties::TM_ALPHA;
					gma.m_mode |= gkMaterialProperties::MA_ALPHABLEND;
				}
				if ((mtex->mapto & MAP_NORM) || (mtex->maptoneg & MAP_NORM))
				{
					gte.m_mode |= gkTextureProperties::TM_NORMAL;
					gma.m_tangentLayer = i;
				}
				if ((mtex->mapto & MAP_SPEC) || (mtex->maptoneg & MAP_SPEC))
					gte.m_mode |= gkTextureProperties::TM_SPECULAR;
				if ((mtex->mapto & MAP_REF)  || (mtex->maptoneg & MAP_REF))
					gte.m_mode |= gkTextureProperties::TM_REFRACTION;
				if ((mtex->mapto & MAP_EMIT) || (mtex->maptoneg & MAP_EMIT))
					gte.m_mode |= gkTextureProperties::TM_EMMISIVE;

				if (mtex->normapspace == MTEX_NSPACE_OBJECT) //else set to tagent space.
					gte.m_texmode |= gkTextureProperties::TX_OBJ_SPACE;

				gte.m_blend = getTexBlendType(mtex->blendtype);				

				gte.m_layer = findTextureLayer(mtex);
				gte.m_mix   = mtex->colfac;				

				gte.m_normalFactor = mtex->norfac;
				gte.m_diffuseColorFactor = mtex->colfac;
				gte.m_diffuseAlpahFactor = mtex->alphafac;
				gte.m_speculaColorFactor = mtex->colspecfac;
				gte.m_speculaHardFactor = mtex->hardfac;

				gte.m_scale.x = 1/mtex->size[0];
				gte.m_scale.y = 1/mtex->size[1];
				gte.m_scale.z = 1/mtex->size[2];
			}
		}
	}
}



void gkBlenderMeshConverter::convertBoneAssignments(int dgi, AssignmentListMap& dest)
{

	Blender::MDeformVert* dvert = m_bmesh->dvert;

	for (int n = 0; n < m_bmesh->totvert; n++)
	{
		const Blender::MDeformVert& dv = dvert[n];
		for (int w = 0; w < dv.totweight; w++)
		{
			if (dv.dw[w].def_nr == dgi)
			{
				gkDeformVertex gdw;
				gdw.group       = dgi;
				gdw.weight      = dv.dw[w].weight;
				gdw.vertexId    = 0;

				UTsize pos;
				if ((pos = dest.find(n)) == GK_NPOS)
				{
					dest.insert(n, AssignmentList());
					pos = dest.find(n);
				}
				if (pos != GK_NPOS)
					dest.at(pos).push_back(gdw);
			}
		}
	}
}


void gkBlenderMeshConverter::findWeight(int index, gkSubMesh* me, AssignmentListMap& assign)
{
	gkVertex* vbuf = me->getVertexBuffer().ptr();
	if (vbuf[index].vba == -1)
		return;

	UTsize pos = assign.find(vbuf[index].vba);
	if (pos != GK_NPOS)
	{
		AssignmentList& list = assign.at(pos);
		for (UTsize a = 0; a < list.size(); a++)
		{
			gkDeformVertex& dv = list.at(a);
			dv.vertexId = index;
			me->addDeformVert(dv);
		}
	}
	vbuf[index].vba = -1;
}




void gkBlenderMeshConverter::assignBoneAssignments(gkSubMesh* me, AssignmentListMap& dest)
{
	UTsize totface = me->getIndexBuffer().size();
	gkTriangle* ibuf = me->getIndexBuffer().ptr();

	for (UTsize f = 0; f < totface; ++f)
	{
		const gkTriangle& i = ibuf[f];

		findWeight((int)i.i0, me, dest);
		findWeight((int)i.i1, me, dest);
		findWeight((int)i.i2, me, dest);
	}
}


bool gkBlenderMeshConverter::convert(void)
{
	if (!m_bmesh->mvert)
		return false;
	if (m_bmesh->mface)
		convert_legacy();
#ifdef BMESH
	else if(m_bmesh->mpoly)
		convert_bmesh();
#endif
	else
		return false;
	
	AssignmentListMap assignMap;
	bool canAssign = m_bmesh->dvert && m_bobj->parent && m_bobj->parent->type == OB_ARMATURE;
	if (canAssign)
	{
		int dgi = 0;
		for (Blender::bDeformGroup* dg = (Blender::bDeformGroup*)m_bobj->defbase.first; dg; dg = dg->next, ++dgi)
		{
			m_gmesh->createVertexGroup(dg->name);
			convertBoneAssignments(dgi, assignMap);
		}
	}
	
	// build materials
	utArrayIterator<utArray<gkMeshPair> > iter(m_meshtable);

	while (iter.hasMoreElements())
	{
		gkMeshHashKey& key  = iter.peekNext().test;
		gkSubMesh* val      = iter.peekNext().item;



		Blender::Material* bmat = BlenderMaterial(m_bobj, key.m_matnr);
		if (key.m_blenderMat)
		{
			if (bmat)
				convertMaterial(bmat, val->getMaterial(), key);
		}
		else
			convertTextureFace(val->getMaterial(), key, (Blender::Image**)key.m_images);

		if (canAssign)
		{
			// build def groups
			assignBoneAssignments(val, assignMap);

		}

		iter.getNext();
	}

	return true;
}


#if BMESH
void gkBlenderMeshConverter::convert_bmesh(void)
{
	Blender::MVert*  mvert = m_bmesh->mvert;
	Blender::MPoly*  mpoly = m_bmesh->mpoly;
	Blender::MLoop*   mloop = m_bmesh->mloop;
	Blender::MTexPoly* mtexpoly = m_bmesh->mtpoly;
	Blender::MLoopUV* muvloop = m_bmesh->mloopuv;
	Blender::MLoopCol* mloopCol =  m_bmesh->mloopcol;
	// UV-Layer-Data
	Blender::MTexPoly* mtpoly[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	Blender::MLoopUV* muvs[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	bool hasUV = muvloop != 0;


	Blender::MVert          vpak[4];
	unsigned int            cpak[4];
	unsigned int            ipak[4];
	int                     totlayer;


	gkSubMesh* curSubMesh = 0;
	m_meshtable.clear();

	gkLoaderUtils_getLayers_bmesh(m_bmesh, mtpoly,muvs, &mloopCol, totlayer);

	bool sortByMat          = gkEngine::getSingleton().getUserDefs().blendermat;
	bool openglVertexColor  = gkEngine::getSingleton().getUserDefs().rendersystem == OGRE_RS_GL;


	bool hasTexPoly = mtexpoly != 0;

	for (int fi = 0; fi < m_bmesh->totpoly; fi++)
	{
		const Blender::MPoly& curpoly = mpoly[fi];
		const Blender::MTexPoly& curTexPol = mtexpoly[fi];

		// skip if face is not a triangle || quad
		if (curpoly.totloop<3)
			continue;

		if (curpoly.totloop>4){
			gkLogger::write("Using poly with more than 4 verts is not supported by gkMeshConverter!");
			continue;
		}


		const bool isQuad = curpoly.totloop==4;

		TempFace t[2];
		PackedFace f;
		f.totlay = totlayer;

		const Blender::MLoop& v1 = mloop[curpoly.loopstart];
		const Blender::MLoop& v2 = mloop[curpoly.loopstart+1];
		const Blender::MLoop& v3 = mloop[curpoly.loopstart+2];

		if (isQuad)
		{

			const Blender::MLoop& v4 = mloop[curpoly.loopstart+3];

			const Blender::MLoopUV& uv1 = muvloop[curpoly.loopstart];
			const Blender::MLoopUV& uv2 = muvloop[curpoly.loopstart+1];
			const Blender::MLoopUV& uv3 = muvloop[curpoly.loopstart+2];
			const Blender::MLoopUV& uv4 = muvloop[curpoly.loopstart+3];

			vpak[0] = mvert[v1.v];
			vpak[1] = mvert[v2.v];
			vpak[2] = mvert[v3.v];
			vpak[3] = mvert[v4.v];

			ipak[0] = v1.v;
			ipak[1] = v2.v;
			ipak[2] = v3.v;
			ipak[3] = v4.v;

			if (mloopCol != 0)
			{
				cpak[0] = *(unsigned int*)(mloopCol + curpoly.loopstart);
				cpak[1] = *(unsigned int*)(mloopCol + curpoly.loopstart+1);
				cpak[2] = *(unsigned int*)(mloopCol + curpoly.loopstart+2);
				cpak[3] = *(unsigned int*)(mloopCol + curpoly.loopstart+3);
			}
			else
				cpak[0] = cpak[1] = cpak[2] = cpak[3] = 0xFFFFFFFF;


//TODO: Multitexture!?

			for (int i = 0; i < totlayer; i++)
			{
				if (mtpoly[i] != 0)
				{
					f.uvLayers[i][0] = gkVector2((float*)&muvs[i][curpoly.loopstart].uv[0]);
					f.uvLayers[i][1] = gkVector2((float*)&muvs[i][curpoly.loopstart+1].uv[0]);
					f.uvLayers[i][2] = gkVector2((float*)&muvs[i][curpoly.loopstart+2].uv[0]);
					f.uvLayers[i][3] = gkVector2((float*)&muvs[i][curpoly.loopstart+3].uv[0]);
				}
			}

			f.verts     = vpak;
			f.index     = ipak;
			f.colors    = cpak;

			// what is this?
			gkVector3 e0, e1;

			e0 = (gkVector3(mvert[v1.v].co) - gkVector3(mvert[v2.v].co));
			e1 = (gkVector3(mvert[v3.v].co) - gkVector3(mvert[v4.v].co));

			if (e0.squaredLength() < e1.squaredLength())
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
				if (mtpoly[i] != 0)
				{
					f.uvLayers[i][0] = gkVector2((float*)&muvs[i][curpoly.loopstart].uv[0]);
					f.uvLayers[i][1] = gkVector2((float*)&muvs[i][curpoly.loopstart+1].uv[0]);
					f.uvLayers[i][2] = gkVector2((float*)&muvs[i][curpoly.loopstart+2].uv[0]);
				}
			}

			vpak[0] = mvert[v1.v];
			vpak[1] = mvert[v2.v];
			vpak[2] = mvert[v3.v];

			ipak[0] = v1.v;
			ipak[1] = v2.v;
			ipak[2] = v3.v;

			if (mloopCol != 0)
			{
				cpak[0] = *(unsigned int*)(mloopCol + curpoly.loopstart);
				cpak[1] = *(unsigned int*)(mloopCol + curpoly.loopstart+1);
				cpak[2] = *(unsigned int*)(mloopCol + curpoly.loopstart+2);
			}
			else
				cpak[0] = cpak[1] = cpak[2] = 0xFFFFFFFF;

			f.verts     = vpak;
			f.index     = ipak;
			f.colors    = cpak;

			convertIndexedTriangle(&t[0], 0, 1, 2, f);
		}


		gkMeshPair tester(curSubMesh);

		if (sortByMat)
		{
			int mode = 0;
			if (mtpoly[0])
				mode = mtpoly[0][fi].mode;


			tester.test = gkMeshHashKey(curpoly.mat_nr, mode);
		}
		else
		{
			Blender::Image* ima[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			for (int i = 0; i < totlayer; i++)
			{
				if (mtpoly[i] != 0)
					ima[i] = mtpoly[i][fi].tpage;
			}


			int mode = 0, alpha = 0;
			if (mtpoly[0])
			{
				mode    = mtpoly[0][fi].mode;
				alpha   = mtpoly[0][fi].transp;
			}


			tester.test = gkMeshHashKey(mode, alpha, ima);
		}

		// find submesh
		UTsize arpos = 0;
		if ((arpos = m_meshtable.find(tester)) == UT_NPOS)
		{
			curSubMesh = new gkSubMesh();

			curSubMesh->setTotalLayers(totlayer);
			curSubMesh->setVertexColors(mloopCol != 0);

			m_gmesh->addSubMesh(curSubMesh);
			tester.item = curSubMesh;
			m_meshtable.push_back(tester);
		}
		else
			curSubMesh = m_meshtable.at(arpos).item;

		if (curSubMesh == 0) continue;


		if (!(curpoly.flag & ME_SMOOTH))
		{
			// face normal
			calcNormal(&t[0]);
			if (isQuad)
				t[1].v0.no = t[1].v1.no = t[1].v2.no = t[0].v0.no;
		}


// ---- warning -----
// mtpoly[0][fi].mode is always 0 which makes the mesh invisible
// For now setting the standardvalue to collider (which means visible)
// This is afaik the only data I couldn't get in import

		int triflag = gkTriangle::TRI_COLLIDER;


		if (mtpoly[0])
		{
			if (mtpoly[0][fi].mode & TF_DYNAMIC)
				triflag |= gkTriangle::TRI_COLLIDER;
			if (mtpoly[0][fi].mode & TF_INVISIBLE)
				triflag |= gkTriangle::TRI_INVISIBLE;
		}
		else
			triflag = gkTriangle::TRI_COLLIDER;

// ---- end warning ------

		curSubMesh->addTriangle(t[0].v0, t[0].i0,
		                        t[0].v1, t[0].i1,
		                        t[0].v2, t[0].i2, triflag);

		if (isQuad)
		{
			curSubMesh->addTriangle(t[1].v0, t[1].i0,
			                        t[1].v1, t[1].i1,
			                        t[1].v2, t[1].i2, triflag);

		}


	}
}
#endif


void gkBlenderMeshConverter::convert_legacy(void)
{
	Blender::MFace*  mface = m_bmesh->mface;
	Blender::MVert*  mvert = m_bmesh->mvert;
	Blender::MCol*   mcol =  0;
	Blender::MTFace* mtface[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	Blender::MVert          vpak[4];
	unsigned int            cpak[4];
	unsigned int            ipak[4];
	int                     totlayer;


	gkSubMesh* curSubMesh = 0;
	m_meshtable.clear();

	gkLoaderUtils_getLayers_legacy(m_bmesh, mtface, &mcol, totlayer);

	bool sortByMat          = gkEngine::getSingleton().getUserDefs().blendermat;
	bool openglVertexColor  = gkEngine::getSingleton().getUserDefs().rendersystem == OGRE_RS_GL;


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
				cpak[0] = packColourABGR(mcol[0]);
				cpak[1] = packColourABGR(mcol[1]);
				cpak[2] = packColourABGR(mcol[2]);
				cpak[3] = packColourABGR(mcol[3]);
			}
			else
				cpak[0] = cpak[1] = cpak[2] = cpak[3] = 0xFFFFFFFF;


			for (int i = 0; i < totlayer; i++)
			{
				if (mtface[i] != 0)
				{
					f.uvLayers[i][0] = gkVector2((float*)mtface[i][fi].uv[0]);
					f.uvLayers[i][1] = gkVector2((float*)mtface[i][fi].uv[1]);
					f.uvLayers[i][2] = gkVector2((float*)mtface[i][fi].uv[2]);
					f.uvLayers[i][3] = gkVector2((float*)mtface[i][fi].uv[3]);
				}
			}
			f.verts     = vpak;
			f.index     = ipak;
			f.colors    = cpak;

			gkVector3 e0, e1;

			e0 = (gkVector3(mvert[curface.v1].co) - gkVector3(mvert[curface.v2].co));
			e1 = (gkVector3(mvert[curface.v3].co) - gkVector3(mvert[curface.v4].co));

			if (e0.squaredLength() < e1.squaredLength())
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
					f.uvLayers[i][0] = gkVector2((float*)mtface[i][fi].uv[0]);
					f.uvLayers[i][1] = gkVector2((float*)mtface[i][fi].uv[1]);
					f.uvLayers[i][2] = gkVector2((float*)mtface[i][fi].uv[2]);
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
				cpak[0] = packColourABGR(mcol[0]);
				cpak[1] = packColourABGR(mcol[1]);
				cpak[2] = packColourABGR(mcol[2]);
			}
			else
				cpak[0] = cpak[1] = cpak[2] = cpak[3] = 0xFFFFFFFF;

			f.verts     = vpak;
			f.index     = ipak;
			f.colors    = cpak;

			convertIndexedTriangle(&t[0], 0, 1, 2, f);
		}

		gkMeshPair tester(curSubMesh);
		if (sortByMat)
		{
			int mode = 0;
			if (mtface[0])
				mode = mtface[0][fi].mode;

			tester.test = gkMeshHashKey(curface.mat_nr, mode);
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

			tester.test = gkMeshHashKey(mode, alpha, ima);
		}

		// find submesh
		UTsize arpos = 0;
		if ((arpos = m_meshtable.find(tester)) == UT_NPOS)
		{
			curSubMesh = new gkSubMesh();

			curSubMesh->setTotalLayers(totlayer);
			curSubMesh->setVertexColors(mcol != 0);

			m_gmesh->addSubMesh(curSubMesh);
			tester.item = curSubMesh;
			m_meshtable.push_back(tester);
		}
		else
			curSubMesh = m_meshtable.at(arpos).item;

		if (curSubMesh == 0) continue;


		if (!(curface.flag & ME_SMOOTH))
		{
			// face normal
			calcNormal(&t[0]);
			if (isQuad)
				t[1].v0.no = t[1].v1.no = t[1].v2.no = t[0].v0.no;
		}

		int triflag = 0;
		if (mtface[0])
		{
			if (mtface[0][fi].mode & TF_DYNAMIC)
				triflag |= gkTriangle::TRI_COLLIDER;
			if (mtface[0][fi].mode & TF_INVISIBLE)
				triflag |= gkTriangle::TRI_INVISIBLE;
		}
		else
			triflag = gkTriangle::TRI_COLLIDER;



		curSubMesh->addTriangle(t[0].v0, t[0].i0,
		                        t[0].v1, t[0].i1,
		                        t[0].v2, t[0].i2, triflag);

		if (isQuad)
		{
			curSubMesh->addTriangle(t[1].v0, t[1].i0,
			                        t[1].v1, t[1].i1,
			                        t[1].v2, t[1].i2, triflag);

		}

		if (mcol)
			mcol += 4;

	}
}


Blender::Material* gkBlenderMeshConverter::getMaterial(Blender::Object* ob, int index)
{
	if (!ob || ob->totcol == 0) return 0;

	index = gkClamp<int>(index, 0, ob->totcol - 1);
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



//#define DEBUG_BPARSE_PTRPTR_ARRAY
#ifdef DEBUG_BPARSE_PTRPTR_ARRAY

	printf("Object(%s):\n\t"
	       "Total Materials = %i\n\t"
	       "In Object Data  = %i\n\t"
	       "Material Index  = %i\n\t"
	       "Material Name   = %s\n",
	       ob->id.name, ob->totcol, inObject, index, (ma ? ma->id.name : "NULL"));
#endif

	return ma;
}


static void gkLoaderUtils_getLayers_legacy(
    Blender::Mesh* mesh,
    Blender::MTFace** eightLayerArray,
    Blender::MCol** oneMCol,
    int& validLayers)
{
	GK_ASSERT(mesh);

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


#if BMESH
static void gkLoaderUtils_getLayers_bmesh(
    Blender::Mesh* mesh,
    Blender::MTexPoly** eightLayerArray,
    Blender::MLoopUV** uvEightLayerArray,
    Blender::MLoopCol** oneMCol,
    int& validLayers)
{
	GK_ASSERT(mesh);

	validLayers = 0;

	// Poly-Data: Textures,...
	Blender::CustomDataLayer* layers = (Blender::CustomDataLayer*)mesh->pdata.layers;
	if (layers)
	{
		// push valid layers
		for (int i = 0; i < mesh->pdata.totlayer && validLayers < 8; i++)
		{
			Blender::CustomDataLayer l = layers[i];
			if (layers[i].type == CD_MTEXPOLY && eightLayerArray)
			{
				void* a = layers[i].data;
				Blender::MTexPoly* mtf = (Blender::MTexPoly*)layers[i].data;
				if (mtf)
					eightLayerArray[validLayers++] = mtf;
			}
		}
	}
	else
	{
		// TODO: Check this
		if (eightLayerArray && mesh->mtpoly)
			eightLayerArray[validLayers++] = mesh->mtpoly;
		// TODO: Check this
		if (oneMCol && mesh->mloopcol)
			*oneMCol = mesh->mloopcol;
	}

	// this have to be equal to validLayers afterwards,right?
	int uvValidLayers = 0;
	// Loop-Data: UV-Data,..
	layers = (Blender::CustomDataLayer*)mesh->ldata.layers;
	if (layers)
	{
		// push valid layers
		for (int i = 0; i < mesh->ldata.totlayer && uvValidLayers < 8; i++)
		{
			Blender::CustomDataLayer l = layers[i];
			if (layers[i].type == CD_MLOOPUV && uvEightLayerArray)
			{
				void* a = layers[i].data;
				Blender::MLoopUV* mtf = (Blender::MLoopUV*)layers[i].data;
				if (mtf)
					uvEightLayerArray[uvValidLayers++] = mtf;
			}
		}
	}
	else
	{
		// TODO: Check this
		if (oneMCol && mesh->mloopcol)
			*oneMCol = mesh->mloopcol;
	}

	if (uvValidLayers!=validLayers){
		gkLogger::write("Warning! gkMeshConverter gkLoaderUtilsGetLayers: validLayers!=validUvLayers!");
	}

}
#endif
