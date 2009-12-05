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
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreHighLevelGpuProgram.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreGpuProgramManager.h"
#include "OgreGpuProgram.h"
#include "OgreMaterialSerializer.h"
#include "OgreMaterialManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreMaterialManager.h"

#include "gkMathUtils.h"
#include "gkMeshUtils.h"
#include "gkTextureLoader.h"
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkUtils.h"
#include "gkMaterialLoader.h"
#include "gkBlenderDefines.h"
#include "gkBlendFile.h"
#include "gkPath.h"
#include "gkUserDefs.h"

#include "blender.h"
#include "bBlenderFile.h"
#include "bMain.h"

/// Material Framework (WIP)
#define AMBIENT_VP  "BMF/Default/AmbientPassV"
#define AMBIENT_FP  "BMF/Default/AmbientPassFOneTex"
#define LIGHT_VP	"BMF/Default/LightPassV_std"
#define LIGHT_VPN   "BMF/Default/LightPassV_norm"
#define TEXTURE_VP  "BMF/Default/TexturePassStdV"
#define TEXTURE_FP  "BMF/Default/TexturePassStdF"

using namespace Ogre;



//-----------------------------------------------------------------------------
gkMaterialUtils::gkMaterialUtils(gkBlendFile *fp) :
		mFile(fp), mTech(0), mPass(0), mBlenderMat(0),
		mBlenderMesh(0), mOgreMat(0),
		mMultiPass(false), mIsModern(false)
{
	GK_ASSERT(mFile);
}

//-----------------------------------------------------------------------------
Pass *gkMaterialUtils::getOrCreatePass(size_t idx)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return 0;

	size_t passc= (size_t)mTech->getNumPasses();


	if (idx < passc)
		return mTech->getPass((unsigned short)idx);

	if (idx > passc)
		return mTech->getPass((unsigned short)(passc - 1));

	return mTech->createPass();
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::handleMultiPass(void)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return;

	applyAmbientPass();
	applyLightPass();
	applyTexturePass();
}

//-----------------------------------------------------------------------------
bool gkMaterialUtils::lampTest(void)
{
	GK_ASSERT(mFile);
	bParse::bMain *mp = mFile->getInternalFile()->getMain();
	return !mp->getLamp()->empty();
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::handleStd(void)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return;

	Pass *pass= getOrCreatePass(0);

	/// base colors
	ColourValue diffuse= ColourValue(mBlenderMat->r, mBlenderMat->g, mBlenderMat->b, mBlenderMat->alpha);
	ColourValue spec=   ColourValue(mBlenderMat->specr, mBlenderMat->specg, mBlenderMat->specb, 1.0);

	/// use light by default
	if (!(mBlenderMat->mode & MA_SHLESS) && lampTest())
	{
		pass->setLightingEnabled(true);

		pass->setShininess(mBlenderMat->har / 4.0);

		pass->setAmbient(diffuse * mBlenderMat->amb);
		pass->setDiffuse(diffuse * (mBlenderMat->emit + mBlenderMat->ref));
		pass->setSpecular(spec * mBlenderMat->spec);

		if (mBlenderMat->emit > 0)
			pass->setSelfIllumination(diffuse * mBlenderMat->emit);
		else
			pass->setSelfIllumination(ColourValue::Black);

		if (mBlenderMat->alpha < 1.0)
		{
			pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
			pass->setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0);
		}
		else
			pass->setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ZERO);
	}
	else
		pass->setLightingEnabled(false);

	if (mBlenderMat->mode & MA_WIRE)
		pass->setPolygonMode(Ogre::PM_WIREFRAME);
	else
		pass->setPolygonMode(Ogre::PM_SOLID);

	/// vertex color paint overide diffuse
	if (mBlenderMat->mode & MA_VERTEXCOLP)
		pass->setVertexColourTracking(Ogre::TVC_DIFFUSE);
	/// vertex color light
	else if (mBlenderMat->mode & MA_VERTEXCOL)
		pass->setVertexColourTracking(Ogre::TVC_DIFFUSE | Ogre::TVC_AMBIENT | Ogre::TVC_EMISSIVE | Ogre::TVC_SPECULAR);
	/// use normal diffuse
	else
		pass->setVertexColourTracking(Ogre::TVC_NONE);

	/// overide mist settings
	if (mBlenderMat->mode &MA_NOMIST)
		pass->setFog(true);
	else
		pass->setFog(false);

	size_t txcnt= getNumTextures();
	for (size_t i=0; i<txcnt; i++)
	{
		Blender::MTex* mt= getTexture(i);
		if (mt && mt->tex && mt->tex->ima)
		{
			TextureUnitState *st= addTextureUnit(pass, mt);
			if (st)
				st->setColourOperation(LBO_MODULATE);
		}
	}
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::applyAmbientPass(void)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return;

	Pass *ambient= getOrCreatePass(0);
	if (mIsModern)
	{
		if (gkIS_PROGRAM(AMBIENT_VP))
		{
			ambient->setVertexProgram(AMBIENT_VP);
			ambient->getVertexProgramParameters()->setNamedConstant("fac", Real(mBlenderMat->amb));
		}

		size_t txcnt= getNumTextures();
		if (txcnt > 0)
		{
			// one ambient map
			Blender::MTex *mt= this->getTexture(MAP_AMB, -1);
			if (mt != 0)
			{
				if (gkIS_PROGRAM(AMBIENT_FP))
				{
					ambient->setFragmentProgram(AMBIENT_FP);
					addTextureUnit(ambient, mt);
					ambient->getFragmentProgramParameters()->setNamedConstant("lay", Real(getUVLayer(mt)));
				}
			}
		}

	}
	else
	{
		// do fallback
	}
}

//-----------------------------------------------------------------------------
String gkMaterialUtils::getSpecularDef(void)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return "";

	switch (mBlenderMat->spec_shader)
	{
	case MA_SPEC_COOKTORR:
		return "COOKTORR_SPEC";
	case MA_SPEC_PHONG:
		return "PHONG_SPEC";
	case MA_SPEC_TOON:
		return "TOON_SPEC";
	case MA_SPEC_WARDISO:
		return "WARDISO_SPEC";
	case MA_SPEC_BLINN:
		return "BLINN_SPEC";
	}
	return "";
}

//-----------------------------------------------------------------------------
String gkMaterialUtils::getDiffuseDef(void)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return "";

	switch (mBlenderMat->diff_shader)
	{
	case MA_DIFF_LAMBERT:
		return "LAMBERT_DIFF";
	case MA_DIFF_ORENNAYAR:
		return "ORENNAYER_DIFF";
	case MA_DIFF_FRESNEL:
		return "FRESNEL_DIFF";
	case MA_DIFF_TOON:
		return "TOON_DIFF";
	case MA_DIFF_MINNAERT:
		return "MINNAERT_DIFF";
	}
	return "";
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::applyParameters(GpuProgramParametersSharedPtr params)
{
	if (!mOgreMat || !mBlenderMat || !mTech || params.isNull())
		return;
#if 0
	// light params
	params->setNamedAutoConstant("lamp.diffuse",	GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR);
	params->setNamedAutoConstant("lamp.power",	  GpuProgramParameters::ACT_LIGHT_POWER_SCALE);
	params->setNamedAutoConstant("lamp.extra",	  GpuProgramParameters::ACT_LIGHT_ATTENUATION);

	params->setNamedConstant("mat.diffuse",		 ColourValue(mBlenderMat->r, mBlenderMat->g, mBlenderMat->b));
	params->setNamedConstant("mat.specular",		ColourValue(mBlenderMat->specr, mBlenderMat->specg, mBlenderMat->specb));
	params->setNamedConstant("mat.spec",			Real(mBlenderMat->spec));
	params->setNamedConstant("mat.ref",			 Real(mBlenderMat->ref));
	params->setNamedConstant("mat.emit",			Real(mBlenderMat->emit));

	switch (mBlenderMat->spec_shader)
	{
	case MA_SPEC_PHONG:
	case MA_SPEC_COOKTORR:
		params->setNamedConstant("mat.har", Real(mBlenderMat->har));
		break;
	case MA_SPEC_TOON:
		params->setNamedConstant("mat.param0", Real(mBlenderMat->param[2]));
		params->setNamedConstant("mat.param1", Real(mBlenderMat->param[3]));
		break;
	case MA_SPEC_WARDISO:
		params->setNamedConstant("mat.param0", Real(mBlenderMat->rms));
		break;
	case MA_SPEC_BLINN:
		params->setNamedConstant("mat.har", Real(mBlenderMat->har));
		params->setNamedConstant("mat.param0", Real(mBlenderMat->refrac));
		break;
	}

	switch (mBlenderMat->diff_shader)
	{
	case MA_DIFF_LAMBERT:
		break;
	case MA_DIFF_ORENNAYAR:
		params->setNamedConstant("mat.param2", Real(mBlenderMat->roughness));
		break;
	case MA_DIFF_FRESNEL:
	case MA_DIFF_TOON:
		params->setNamedConstant("mat.param2", Real(mBlenderMat->param[0]));
		params->setNamedConstant("mat.param3", Real(mBlenderMat->param[1]));
		break;
	case MA_DIFF_MINNAERT:
		params->setNamedConstant("mat.param2", Real(mBlenderMat->darkness));
		break;
	}
#endif
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::applyLightPass(void)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return;

#if 0
	if (mFile->getInternalFile()->getMain()->getLamp().empty() || (mBlenderMat->mode & MA_SHLESS) !=0)
		return;

	Pass *light= getOrCreatePass(1);
	if (mIsModern)
	{
		light->setAmbient(ColourValue::Black);
		light->setDiffuse(ColourValue::Black);
		light->setSpecular(ColourValue::Black);
		light->setShininess(0.0);

		light->setIteratePerLight(true, false);
		light->setSceneBlending(SBT_ADD);

		Blender::MTex *spec_map= getTexture(MAP_SPEC, -1);
		Blender::MTex *normal_map= getTexture(MAP_NORM, -1);


		bool isNormal= normal_map != 0;
		bool isSpec= spec_map != 0;

		if (isNormal)
		{
			if (gkIS_PROGRAM(LIGHT_VPN))
			{
				light->setVertexProgram(LIGHT_VPN);
				GpuProgramParametersSharedPtr params= light->getVertexProgramParameters();
				params->setNamedAutoConstant("cameraPosition",  GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
				params->setNamedAutoConstant("lampPosition", GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
			}

			String name= "BMF/Default/LightPassFNorm_" + getDiffuseDef() + "_" + getSpecularDef();
			if (gkIS_PROGRAM(name))
			{
				light->setFragmentProgram(name);

				GpuProgramParametersSharedPtr params= light->getFragmentProgramParameters();
				params->setIgnoreMissingParams(true);

				applyParameters(params);
				addTextureUnit(light, normal_map);
				params->setNamedConstant("layer", Real(getUVLayer(normal_map)));

			}
		}

		if (!isNormal && !isSpec)
		{
			/// standard blender lighting ( no normal maps )
			if (gkIS_PROGRAM(LIGHT_VP))
			{
				light->setVertexProgram(LIGHT_VP);
				GpuProgramParametersSharedPtr params= light->getVertexProgramParameters();
				params->setNamedAutoConstant("camPos",  GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
				params->setNamedAutoConstant("lampPos", GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
			}

			String name= "BMF/Default/LightPassF_" + getDiffuseDef() + "_" + getSpecularDef();
			if (gkIS_PROGRAM(name))
			{
				light->setFragmentProgram(name);
				applyParameters(light->getFragmentProgramParameters());
			}
		}
	}
	else
	{
		// do fallback
	}
#endif
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::applyTexturePass(void)
{
#if 0
	if (!mOgreMat || !mBlenderMat || !mTech)
		return;

	Pass *tex= getOrCreatePass(2);
	if (mIsModern)
	{
		tex->setLightingEnabled(false);
		tex->setSceneBlending(SBT_MODULATE);

		if (gkIS_PROGRAM(TEXTURE_VP))
			tex->setVertexProgram(TEXTURE_VP);

		if (gkIS_PROGRAM(TEXTURE_FP))
		{
			tex->setFragmentProgram(TEXTURE_FP);

			GpuProgramParametersSharedPtr params= tex->getFragmentProgramParameters();

			int cnt= 0;

			size_t txcnt= getNumTextures();
			for (size_t i=0; i<txcnt; i++)
			{
				Blender::MTex* mt= getTexture(i);
				if (mt && mt->tex && mt->tex->ima)
				{
					if (!(mt->mapto & MAP_AMB) && !(mt->mapto & MAP_NORM)) /* handled elsewhere */
					{
						addTextureUnit(tex, mt);
						params->setNamedConstant(String("lay[") + StringConverter::toString(cnt) + String("]"), Real(getUVLayer(mt)));
						cnt ++;
					}
				}
			}

			params->setNamedConstant("tot", Real(cnt));
		}
	}
	else
	{
		// do fallback
	}
#endif
}


//-----------------------------------------------------------------------------
size_t gkMaterialUtils::getNumTextures(void)
{
	if (!mOgreMat || !mBlenderMat || !mTech)
		return 0;

	if (mTextures.empty())
	{
		if (mBlenderMat->mtex != 0)
		{
			for (int i= 0; i<MAX_MTEX; i++)
			{
				if (mBlenderMat->mtex[i] != 0)
				{
					Blender::MTex *mtex= mBlenderMat->mtex[i];
					if (mtex && mtex->tex != 0)
					{
						if (mtex->tex->type == TEX_IMAGE)
							mTextures.push_back(mtex);
					}
				}
			}
		}
	}

	return mTextures.size();
}

//-----------------------------------------------------------------------------
Blender::MTex* gkMaterialUtils::getTexture(size_t i)
{
	return (i < mTextures.size()) ? mTextures.at(i) : 0;
}


//-----------------------------------------------------------------------------
// Extract a specality texture
// MAP_AMB	 == texture in ambient pass / for (shadow maps && ao maps)
// MAP_NORM	== lighting pass normal map
// MAP_SPEC	== lighting pass specular map
//-----------------------------------------------------------------------------
Blender::MTex* gkMaterialUtils::getTexture(int mapto, int fallback)
{
	for (size_t i=0; i<mTextures.size(); i++)
	{
		Blender::MTex *mt= mTextures[i];
		if (mt && mt->tex && mt->tex->ima)
		{
			if (mt->mapto & mapto)
				return mt;
		}
	}
	if (fallback != -1)
		return getTexture(fallback);
	return 0;
}

//-----------------------------------------------------------------------------
gkMaterialUtils::MTexList gkMaterialUtils::getTextures(int mapto)
{
	MTexList ret;
	for (size_t i=0; i<mTextures.size(); i++)
	{
		Blender::MTex *mt= mTextures[i];

		if (mt && mt->tex && mt->tex->ima)
		{
			if (mt->mapto & mapto)
				ret.push_back(mt);
		}
	}
	return ret;
}

//-----------------------------------------------------------------------------
int gkMaterialUtils::getUVLayer(Blender::MTex *te)
{
	if (!mBlenderMesh || !te || !mOgreMat || !mBlenderMat || !mTech)
		return TL_UV0;

	if (!(te->texco & TEXCO_UV))
		return TL_UV0;

	StringVector names;
	if (mBlenderMesh->fdata.layers)
	{
		Blender::CustomDataLayer *cd= (Blender::CustomDataLayer*)mFile->_findPtr(mBlenderMesh->fdata.layers);

		if (cd)
		{
			for (int i= 0; i < mBlenderMesh->fdata.totlayer; i++)
			{
				if (cd[i].type == CD_MTFACE)
					names.push_back(String(cd[i].name));
			}
		}
	}

	int layer= TL_UV0;
	for (size_t i= 0; i < names.size(); i++)
	{
		if (names[i] == te->uvname)
		{
			layer= TL_UV0 + (int)i;
			break;
		}
	}
	return layer;
}

//-----------------------------------------------------------------------------
TextureUnitState* gkMaterialUtils::addTextureUnit(Pass* ptr, Blender::MTex *tex)
{
	if (!tex) return 0;
	if (!tex->tex) return 0;

	return addTextureUnit(ptr, tex->tex->ima, getUVLayer(tex));

}

//-----------------------------------------------------------------------------
void gkMaterialUtils::addTextureUnit(Ogre::MaterialPtr ptr, Blender::Image *ima, int layer_nr)
{
	/// creat for pass
	addTextureUnit(ptr->getTechnique(0)->getPass(0), ima, layer_nr);
}

//-----------------------------------------------------------------------------
TextureUnitState* gkMaterialUtils::addTextureUnit(Pass* pass, Blender::Image *ima, int layer_nr)
{
	/// can only use packed file or file path
	/// ImBuf is internal to blender
	gkPath p(ima->name);
	const String& base= p.base();

	if (!ima && !ima->packedfile && !gkUtils::isResource(base))
		return 0;


	mFile->_registerImage(ima);

	TextureUnitState *state= 0;
	if (gkUtils::isResource(base))
		state= pass->createTextureUnitState(base);
	else
		state= pass->createTextureUnitState(GKB_IDNAME(ima));

	if (state && layer_nr >= TL_UV0 && layer_nr <= TL_UV7)
		state->setTextureCoordSet(layer_nr);
	if (state)
	{
		state->setTextureAddressingMode(ima->tpageflag & IMA_CLAMP_U ? TextureUnitState::TAM_CLAMP : TextureUnitState::TAM_WRAP,
										ima->tpageflag & IMA_CLAMP_V ? TextureUnitState::TAM_CLAMP : TextureUnitState::TAM_WRAP,
										TextureUnitState::TAM_WRAP
									   );
	}
	return state;
}

//-----------------------------------------------------------------------------
bool gkMaterialUtils::applyTexFace(int flags, int alpha)
{
	if (!mOgreMat || !mTech)
		return false;

	if (flags &TF_INVISIBLE)
	{
		mOgreMat->setReceiveShadows(false);
		mTech->setColourWriteEnabled(false);
		mTech->setDepthWriteEnabled(false);
		mTech->setLightingEnabled(false);

		// skip the rest
		return false;
	}

	if (flags & TF_LIGHT)
		mTech->setLightingEnabled(lampTest());

	if (flags &TF_TWOSIDE)
	{
		mTech->setCullingMode(CULL_NONE);
		mTech->setManualCullingMode(MANUAL_CULL_NONE);
	}

	if (alpha &TF_ALPHA)
	{
		mTech->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
		mTech->setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
	}
	if (alpha &TF_ADD)
		mTech->setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ONE);
	
	return true;
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::getOgreMaterialFromMaterial(Ogre::MaterialPtr ptr, Blender::Material *matr, Blender::Mesh *me, int flags)
{
	if (ptr.isNull()) return;

	mOgreMat= ptr.getPointer();
	mBlenderMat= matr;
	mBlenderMesh= me;
	mTech= mOgreMat->getTechnique(0);

	mOgreMat->setReceiveShadows(false);//(mBlenderMat->mode & MA_SHADOW) != 0);

	gkUserDefs& defs= gkEngine::getSingleton().getUserDefs();
	mMultiPass= defs.multipassmat;
	mIsModern= false;//mMultiPass && HASCAPABILITY(RSC_VERTEX_PROGRAM) && HASCAPABILITY(RSC_FRAGMENT_PROGRAM);

	if (mIsModern)// test profiles
	{
		// Written on: Radeon X1300/X1550
		//  using vertex:   vs_1_1 arbvp1
		//  using fragment: ps_2_x arbfp1
		if (gkIS_OPENGL())
		{
			if (mIsModern && !GpuProgramManager::getSingleton().isSyntaxSupported(defs.gl_vert_profile))
				mIsModern= false;
			if (mIsModern && !GpuProgramManager::getSingleton().isSyntaxSupported(defs.gl_frag_profile))
				mIsModern= false;
		}
		else
		{
			if (mIsModern && !GpuProgramManager::getSingleton().isSyntaxSupported(defs.dx_vert_profile))
				mIsModern= false;
			if (mIsModern && !GpuProgramManager::getSingleton().isSyntaxSupported(defs.dx_frag_profile))
				mIsModern= false;
		}

		if (!mIsModern)
		{
			gkLogMessage("Needed vertex and fragment profiles are not suppoted with your graphics card.\n"
						 "No fancy shaders will be used :(");
		}
	}


	if (!applyTexFace(flags, 0))
		return;

	getNumTextures();
	// std materials
	handleStd();
}

//-----------------------------------------------------------------------------
void gkMaterialUtils::setOgreMaterialDefault(Ogre::MaterialPtr ptr, bool lighting, int flags, int alpha)
{
	if (ptr.isNull()) return;

	mOgreMat= ptr.getPointer();
	mTech= mOgreMat->getTechnique(0);

	ptr->setLightingEnabled(lighting && lampTest());
	ptr->setDiffuse(0.8, 0.8, 0.8, 1.0);
	applyTexFace(flags, alpha);
}
