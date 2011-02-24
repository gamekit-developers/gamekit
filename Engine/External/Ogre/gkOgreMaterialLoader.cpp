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
#include "gkMesh.h"
#include "gkScene.h"
#include "gkOgreMaterialLoader.h"
#include "gkOgreSkyBoxGradient.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "OgreMeshManager.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreSceneManager.h"

gkSkyBoxGradient* gkMaterialLoader::loadSceneSkyMaterial(class gkScene* sc, const gkSceneMaterial& material)
{
	//skybox material should be exist in the global resource pool.
	//multiple skybox materials don't working in multi window/scene.
	gkString groupName = sc->getGroupName(); //GK_BUILTIN_GROUP; //

	
	// use user defined
	Ogre::MaterialPtr matptr = Ogre::MaterialManager::getSingleton().getByName(material.m_name);
	if (!matptr.isNull())
	{
		sc->getManager()->setSkyBox(true, material.m_name, material.m_distance, true, gkEuler(-90, 0, 0).toQuaternion());
		return 0;
	}

	if (material.m_type == gkSceneMaterial::FLAT)
		return 0;

	gkSkyBoxGradient* grad = new gkSkyBoxGradient(material, groupName); //sc->getGroupName());
	sc->getManager()->setSkyBox(true, material.m_name, material.m_distance, true);
	return grad;
}



void gkMaterialLoader::loadSubMeshMaterial(gkSubMesh* mesh, const gkString& group)
{
	gkMaterialProperties& gma = mesh->getMaterial();
	if (gma.m_name.empty())
		gma.m_name = "<gkBuiltin/DefaultMaterial>";

	Ogre::MaterialPtr oma = Ogre::MaterialManager::getSingleton().getByName(gma.m_name.c_str(), group);
	if (!oma.isNull())
		return;

	oma = Ogre::MaterialManager::getSingleton().create(gma.m_name, group);

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

	bool enableLights = (gma.m_mode & gkMaterialProperties::MA_LIGHTINGENABLED) != 0;
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

	Ogre::Pass* pass = oma->getTechnique(0)->getPass(0);

	bool matBlending = gkEngine::getSingleton().getUserDefs().matblending;

	if (matBlending && (gma.m_mode & gkMaterialProperties::MA_HASRAMPBLEND))
	{
		using namespace Ogre;

		switch (gma.m_rblend)
		{
		case GK_BT_MULTIPLY:			
			pass->setSceneBlending(SBT_MODULATE);			
			break;
		case GK_BT_SUBTRACT:			
			pass->setSceneBlending(SBF_ONE_MINUS_SOURCE_COLOUR, SBF_ONE);
			break;
		case GK_BT_DARKEN:
			pass->setSceneBlendingOperation(SBO_MIN);
			pass->setSceneBlending(SBF_ONE, SBF_ONE);
			break;
		case GK_BT_LIGHTEN:
			pass->setSceneBlendingOperation(SBO_MAX);
			pass->setSceneBlending(SBF_ONE, SBF_ONE);
			break;
		case GK_BT_SCREEN:			
			pass->setSceneBlending(SBF_ONE_MINUS_DEST_COLOUR, SBF_ONE);
			break;
		case GK_BT_ADDITIVE:
			pass->setSceneBlending(SBT_ADD);
			break;
		case GK_BT_MIXTURE:
		default:
			pass->setSceneBlending(SBF_ONE, SBF_ZERO);
			break;
		}
	}

	for (int i = 0; i < gma.m_totaltex; ++i)
	{
		using namespace Ogre;

		gkTextureProperties& gte = gma.m_textures[i];
		Ogre::TextureUnitState* otus = pass->createTextureUnitState(gte.m_name, gte.m_layer);

		LayerBlendOperationEx op = LBX_MODULATE;

		switch (gte.m_blend)
		{
		case GK_BT_ADDITIVE:
			op = LBX_ADD;
			break;

		case GK_BT_SUBTRACT:			
			op = LBX_SUBTRACT;
			break;

		case GK_BT_DARKEN:	
		case GK_BT_LIGHTEN:	
		case GK_BT_SCREEN:
		case GK_BT_COLOR:
			//break; TODO: support more mode

		case GK_BT_MULTIPLY:
		case GK_BT_MIXTURE:
		default:
			op = LBX_MODULATE;
			break;
		}

		if (i == 0)
			otus->setColourOperationEx(op, LBS_DIFFUSE, LBS_TEXTURE);		
		else
			otus->setColourOperationEx(op);
	}


	if (gma.m_mode & gkMaterialProperties::MA_ALPHABLEND)
	{
		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
		pass->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 150);
		pass->setDepthWriteEnabled(false);
	}
}
