/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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

#include "gkCommon.h"
#include "gkResourceGroupManager.h"
#include "gkLogger.h"
#include "gkGroupManager.h"
#include "gkTextManager.h"
#include "gkAnimationManager.h"
#include "gkMeshManager.h"
#include "gkSkeletonManager.h"
#include "gkGameObjectManager.h"
#include "External/Ogre/gkOgreBlendArchive.h"
#include "External/Ogre/gkOgreMaterialLoader.h"

#include "OgreResourceGroupManager.h"
#include "OgreZip.h"
#include "OgreRoot.h"
#include "OgreGpuProgramManager.h"

#include "gkSceneManager.h"

#ifdef OGREKIT_OPENAL_SOUND
#include "gkSoundManager.h"
#endif

#ifdef OGREKIT_USE_LUA
#include "Script/Lua/gkLuaManager.h"
#endif

#ifdef OGREKIT_USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"

#include "User/gkRTShaderTemplates.inl"
#endif

#ifdef OGREKIT_USE_COMPOSITOR
#include "User/gkMaterialsTemplates.inl"
#endif

#ifdef OGREKIT_USE_COMPOSITOR_TEX
#include "User/gkCompositorTextures.inl"
#endif

UT_IMPLEMENT_SINGLETON(gkResourceGroupManager)


gkResourceGroupManager::gkResourceGroupManager()
	:	m_materialLoader(0)
{
	try
	{
		Ogre::ResourceGroupManager& mgr = Ogre::ResourceGroupManager::getSingleton();

		mgr.addResourceLocation("", gkBlendArchiveFactory::ARCHIVE_TYPE,  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		gkString type = "EmbeddedZip";
		gkString group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

#ifdef OGREKIT_USE_RTSHADER_SYSTEM	
	    if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
        {
			Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("RTShaderLib_GLSLES.zip", RTSHADERLIB_GLSLES, RTSHADERLIB_GLSLES_SIZE, 0);
			mgr.addResourceLocation("RTShaderLib_GLSLES.zip", type, group);
        }

        else if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
        {
			Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("RTShaderLib_GLSL.zip", RTSHADERLIB_GLSL, RTSHADERLIB_GLSL_SIZE, 0);
			mgr.addResourceLocation("RTShaderLib_GLSL.zip", type, group);

            if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
            {
				Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("RTShaderLib_GLSL150.zip", RTSHADERLIB_GLSL150, RTSHADERLIB_GLSL150_SIZE, 0);
				mgr.addResourceLocation("RTShaderLib_GLSL150.zip", type, group);
            }
        }
        else if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
        {
			Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("RTShaderLib_HLSL.zip", RTSHADERLIB_HLSL, RTSHADERLIB_HLSL_SIZE, 0);
			mgr.addResourceLocation("RTShaderLib_HLSL.zip", type, group);
        }
#ifdef OGRE_BUILD_PLUGIN_CG
		Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("RTShaderLib_Cg.zip", RTSHADERLIB_CG, RTSHADERLIB_CG_SIZE, 0);
		mgr.addResourceLocation("RTShaderLib_Cg.zip", type, group);
#endif
		
#endif //OGREKIT_USE_RTSHADER_SYSTEM

#ifdef OGREKIT_USE_COMPOSITOR
	    if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
        {
			Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_programs_GLSLES.zip", MATERIALS_PROGRAMS_GLSLES, MATERIALS_PROGRAMS_GLSLES_SIZE, 0);
			mgr.addResourceLocation("materials_programs_GLSLES.zip", type, group);
        }

        else if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
        {
            if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
            {
				Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_programs_GLSL150.zip", MATERIALS_PROGRAMS_GLSL150, MATERIALS_PROGRAMS_GLSL150_SIZE, 0);
				mgr.addResourceLocation("materials_programs_GLSL150.zip", type, group);
            }
			else
			{
				Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_programs_GLSL.zip", MATERIALS_PROGRAMS_GLSL, MATERIALS_PROGRAMS_GLSL_SIZE, 0);
				mgr.addResourceLocation("materials_programs_GLSL.zip", type, group);
			}

			if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
            {
				Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_programs_GLSL400.zip", MATERIALS_PROGRAMS_GLSL400, MATERIALS_PROGRAMS_GLSL400_SIZE, 0);
				mgr.addResourceLocation("materials_programs_GLSL400.zip", type, group);
            }
        }
        else if(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
        {
			Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_programs_HLSL.zip", MATERIALS_PROGRAMS_HLSL, MATERIALS_PROGRAMS_HLSL_SIZE, 0);
			mgr.addResourceLocation("materials_programs_HLSL.zip", type, group);
        }
#ifdef OGRE_BUILD_PLUGIN_CG
		Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_programs_Cg.zip", MATERIALS_PROGRAMS_CG, MATERIALS_PROGRAMS_CG_SIZE, 0);
		mgr.addResourceLocation("materials_programs_Cg.zip", type, group);
#endif

		Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_scripts.zip", MATERIALS_SCRIPTS, MATERIALS_SCRIPTS_SIZE, 0);
		mgr.addResourceLocation("materials_scripts.zip", type, group);

		//Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("materials_scripts_SSAO.zip", MATERIALS_SCRIPTS_SSAO, MATERIALS_SCRIPTS_SSAO_SIZE, 0);
		//mgr.addResourceLocation("materials_scripts_SSAO.zip", type, group);
#endif

#ifdef OGREKIT_USE_COMPOSITOR_TEX
		Ogre::EmbeddedZipArchiveFactory::addEmbbeddedFile("CompositorTextures.zip", COMPOSITORTEXTURES, COMPOSITORTEXTURES_SIZE, 0);
		mgr.addResourceLocation("CompositorTextures.zip", "EmbeddedZip",  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
#endif
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("ResourceGroupManager: " << e.getDescription());
	}
}

gkResourceGroupManager::~gkResourceGroupManager()
{
	Ogre::ResourceGroupManager::getSingleton().removeResourceLocation("",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	delete m_materialLoader; 
}

bool gkResourceGroupManager::createResourceGroup(const gkResourceNameString& group, bool inGlobalPool)
{
	if (existResourceGroup(group)) return false;

	try 
	{
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup(group.str(), inGlobalPool);
		m_groups.push_back(group);
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("ResourceGroupManager: " << e.getDescription());
		return false;
	}

	return true;
}


void gkResourceGroupManager::destroyResourceGroup(const gkResourceNameString& group)
{
	if (!existResourceGroup(group)) return;

	gkSceneManager::getSingleton().destroyGroupInstances(group.str());
	gkGameObjectManager::getSingleton().destroyGroupInstances(group.str());

	gkSceneManager::getSingleton().destroyGroup(group);
	gkGameObjectManager::getSingleton().destroyGroup(group);

	gkGroupManager::getSingleton().destroyGroup(group);
	gkTextManager::getSingleton().destroyGroup(group);
	gkMeshManager::getSingleton().destroyGroup(group);
	gkSkeletonManager::getSingleton().destroyGroup(group);
	gkAnimationManager::getSingleton().destroyGroup(group);

#ifdef OGREKIT_OPENAL_SOUND
	gkSoundManager::getSingleton().stopAllSounds();
	gkSoundManager::getSingleton().destroyGroup(group);
#endif

#ifdef OGREKIT_USE_LUA
	gkLuaManager::getSingleton().destroyGroup(group);
#endif

	Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(group.str());
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(group.str());

	m_groups.erase(m_groups.find(group));
}

void gkResourceGroupManager::destroyAllResourceGroup(void)
{
	gkGameObjectManager::getSingleton().destroyAll();
	gkGroupManager::getSingleton().destroyAll();
	gkTextManager::getSingleton().destroyAll();
	gkMeshManager::getSingleton().destroyAll();
	gkSkeletonManager::getSingleton().destroyAll();

#ifdef OGREKIT_OPENAL_SOUND
	gkSoundManager::getSingleton().destroyAll();
#endif

#ifdef OGREKIT_USE_LUA
	gkLuaManager::getSingleton().destroyAll();
#endif

	gkAnimationManager::getSingleton().destroyAll();

}

bool gkResourceGroupManager::existResourceGroup(const gkResourceNameString& group)
{
	return Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(group.str());
}


void gkResourceGroupManager::clearResourceGroup(const gkResourceNameString& group)
{
	if (existResourceGroup(group))
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(group.str());
}

void gkResourceGroupManager::initialiseAllResourceGroups()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void gkResourceGroupManager::initialiseResourceGroup(const gkString& group)
{

}

bool gkResourceGroupManager::initRTShaderSystem(const gkString& shaderLang, const gkString& shaderCachePath, bool hasFixedCapability)
{
#ifdef OGREKIT_USE_RTSHADER_SYSTEM
	GK_ASSERT( m_materialLoader == 0);

	m_materialLoader = new gkMaterialLoader();

	Ogre::RTShader::ShaderGenerator::initialize();
	
	Ogre::RTShader::ShaderGenerator::getSingleton().setTargetLanguage(shaderLang);
	if (!shaderCachePath.empty())
		Ogre::RTShader::ShaderGenerator::getSingleton().setShaderCachePath(shaderCachePath);

	Ogre::RTShader::ShaderGenerator* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	GK_ASSERT(shaderGenerator);

	if (!hasFixedCapability)
	{
		gkMaterialLoader::createRTSSMaterial("BaseWhite", true);
		gkMaterialLoader::createRTSSMaterial("BaseWhiteNoLighting", false);
	}
	return true;

#else
	return false;
#endif

}