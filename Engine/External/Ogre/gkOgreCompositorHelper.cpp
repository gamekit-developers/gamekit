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

/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/
#include "gkCommon.h"
#include "gkLogger.h"

#include "gkOgreCompositorManager.h"
#include "gkOgreCompositorHelper.h"

#include "Ogre.h"

#define COMP_HALFTONE_TEX_NAME     "HalftoneVolume"
#define COMP_DITHER_TEX_NAME       "DitherTex"


bool gkOgreCompositorHelper::createHalftoneTexture()
{
	using namespace Ogre;

	try 
	{
		if (TextureManager::getSingleton().resourceExists(COMP_HALFTONE_TEX_NAME)) 
			return true; //already created

		TexturePtr tex = TextureManager::getSingleton().createManual(
			COMP_HALFTONE_TEX_NAME,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_3D,
			64,64,64,
			0,
			PF_A8
		);

		HardwarePixelBufferSharedPtr ptr = tex->getBuffer(0,0);
		ptr->lock(HardwareBuffer::HBL_DISCARD);
		const PixelBox &pb = ptr->getCurrentLock();
		uint8 *data = static_cast<uint8*>(pb.data);

		size_t height = pb.getHeight();
		size_t width = pb.getWidth();
		size_t depth = pb.getDepth();
		size_t rowPitch = pb.rowPitch;
		size_t slicePitch = pb.slicePitch;

		for (size_t z = 0; z < depth; ++z)
		{
			for (size_t y = 0; y < height; ++y)
			{
				for(size_t x = 0; x < width; ++x)
				{
					float fx = 32-(float)x+0.5f;
					float fy = 32-(float)y+0.5f;
					float fz = 32-((float)z)/3+0.5f;
					float distanceSquare = fx*fx+fy*fy+fz*fz;
					data[slicePitch*z + rowPitch*y + x] =  0x00;
					if (distanceSquare < 1024.0f)
						data[slicePitch*z + rowPitch*y + x] +=  0xFF;
				}
			}
		}
		ptr->unlock();

		
	} 
	catch (Exception &e) 
	{
		gkPrintf("[CMP] FAILED - Halftone Texture Creation. %s", e.getFullDescription().c_str()); 
		return false;
	}
	
	return true;
}

bool gkOgreCompositorHelper::createDitherTexture(int width, int height)
{
	using namespace Ogre;

	try 
	{
		if (TextureManager::getSingleton().resourceExists(COMP_DITHER_TEX_NAME)) 
			return true; //already created

		TexturePtr tex = TextureManager::getSingleton().createManual(
			COMP_DITHER_TEX_NAME,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_2D,
			width, height, 1,
			0,
			PF_A8
		);

		HardwarePixelBufferSharedPtr ptr = tex->getBuffer(0,0);
		ptr->lock(HardwareBuffer::HBL_DISCARD);
		const PixelBox &pb = ptr->getCurrentLock();
		uint8 *data = static_cast<uint8*>(pb.data);
		
		size_t height = pb.getHeight();
		size_t width = pb.getWidth();
		size_t rowPitch = pb.rowPitch;

		for (size_t y = 0; y < height; ++y)
			for(size_t x = 0; x < width; ++x)
				data[rowPitch*y + x] = (uint8)Ogre::Math::RangeRandom(64.0,192);

		ptr->unlock();

	} 
	catch (Exception &e) 
	{
		gkPrintf("[CMP] FAILED - Dither Texture Creation. %s", e.getFullDescription().c_str()); 
		return false;
	}

	return true;
}


bool gkOgreCompositorHelper::createMotionBlurCompositor()
{
	using namespace Ogre;

	try 
	{
		CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create(
			GK_COMPOSITOR_MOTION_BLUR, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);

		Ogre::CompositionTechnique *t = comp3->createTechnique();
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		/// Render scene
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("scene");
		}
		/// Initialisation pass for sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("sum");
			tp->setOnlyInitial(true);
		}
		/// Do the motion blur
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("temp");
			{ 
				Ogre::CompositionPass *pass = tp->createPass();
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setMaterialName("Ogre/Compositor/Combine");
				pass->setInput(0, "scene");
				pass->setInput(1, "sum");
			}
		}
		/// Copy back sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("sum");
			{ 
				Ogre::CompositionPass *pass = tp->createPass();
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setMaterialName("Ogre/Compositor/Copyback");
				pass->setInput(0, "temp");
			}
		}
		/// Display result
		{
			Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			{ 
				Ogre::CompositionPass *pass = tp->createPass();
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setMaterialName("Ogre/Compositor/MotionBlur");
				pass->setInput(0, "sum");
			}
		}
	} 
	catch (Exception &e) 
	{
		gkPrintf("[CMP] FAILED %s", e.getFullDescription().c_str()); 
		return false;
	}


	return true;
}

bool gkOgreCompositorHelper::createHeatVisionCompositor()
{
#if GK_USE_COMPOSITOR_HEAT_VISION
	using namespace Ogre;

	/// Heat vision effect
	try 
	{
		Ogre::CompositorPtr comp4 = Ogre::CompositorManager::getSingleton().create(
			GK_COMPOSITOR_HEAT_VISION, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);		

		Ogre::CompositionTechnique *t = comp4->createTechnique();

		t->setCompositorLogicName(GK_COMPOSITOR_HEAT_VISION);
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
			def->width = 256;
			def->height = 256;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
			def->width = 256;
			def->height = 256;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		/// Render scene
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("scene");
		}
		/// Light to heat pass
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("temp");
			{
				Ogre::CompositionPass *pass = tp->createPass();
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setIdentifier(0xDEADBABE); /// Identify pass for use in listener
				pass->setMaterialName("Fury/HeatVision/LightToHeat");
				pass->setInput(0, "scene");
			}
		}
		/// Display result
		{
			Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			{
				Ogre::CompositionPass *pass = tp->createPass();
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setMaterialName("Fury/HeatVision/Blur");
				pass->setInput(0, "temp");
			}
		}
	} 
	catch (Exception &e) 
	{
		gkPrintf("[CMP] FAILED %s", e.getFullDescription().c_str()); 
		return false;
	}
	
	return true;
#else
	return false;
#endif
}

//--

Ogre::CompositorInstance::Listener* gkOgreCompositorHDRLogic::createListener(Ogre::CompositorInstance* instance)
{
	gkOgreCompositorHDRListener* listener = new gkOgreCompositorHDRListener;
	Ogre::Viewport* vp = instance->getChain()->getViewport();
	listener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
	listener->notifyCompositor(instance);
	return listener;
}

Ogre::CompositorInstance::Listener* gkOgreCompositorHeatVisionLogic::createListener(Ogre::CompositorInstance* instance)
{
	return new gkOgreCompositorHeatVisionListener;
}

Ogre::CompositorInstance::Listener* gkOgreCompositorGaussianBlurLogic::createListener(Ogre::CompositorInstance* instance)
{
	gkOgreCompositorGaussianListener* listener = new gkOgreCompositorGaussianListener;
	Ogre::Viewport* vp = instance->getChain()->getViewport();
	listener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
	return listener;
}

//--

gkOgreCompositorHeatVisionListener::gkOgreCompositorHeatVisionListener()
{
	timer = new Ogre::Timer();
    start = end = curr = 0.0f;
}

gkOgreCompositorHeatVisionListener::~gkOgreCompositorHeatVisionListener()
{
   delete timer;
}

void gkOgreCompositorHeatVisionListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    if(pass_id == 0xDEADBABE)
    {
        timer->reset();
        fpParams =
            mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
    }
}

void gkOgreCompositorHeatVisionListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    if(pass_id == 0xDEADBABE)
    {
        // "random_fractions" parameter
        fpParams->setNamedConstant("random_fractions", Ogre::Vector4(Ogre::Math::RangeRandom(0.0, 1.0), Ogre::Math::RangeRandom(0, 1.0), 0, 0));

        // "depth_modulator" parameter
        float inc = ((float)timer->getMilliseconds())/1000.0f;
        if ( (fabs(curr-end) <= 0.001) ) {
            // take a new value to reach
            end = Ogre::Math::RangeRandom(0.95f, 1.0f);
            start = curr;
        } else {
            if (curr > end) curr -= inc;
            else curr += inc;
        }
        timer->reset();

        fpParams->setNamedConstant("depth_modulator", Ogre::Vector4(curr, 0, 0, 0));
    }
}

//--

gkOgreCompositorHDRListener::gkOgreCompositorHDRListener()
{
}

gkOgreCompositorHDRListener::~gkOgreCompositorHDRListener()
{
}

void gkOgreCompositorHDRListener::notifyViewportSize(int width, int height)
{
	mVpWidth = width;
	mVpHeight = height;
}

void gkOgreCompositorHDRListener::notifyCompositor(Ogre::CompositorInstance* instance)
{
	// Get some RTT dimensions for later calculations
	Ogre::CompositionTechnique::TextureDefinitionIterator defIter =
		instance->getTechnique()->getTextureDefinitionIterator();
	while (defIter.hasMoreElements())
	{
		Ogre::CompositionTechnique::TextureDefinition* def =
			defIter.getNext();
		if(def->name == "rt_bloom0")
		{
			mBloomSize = (int)def->width; // should be square
			// Calculate gaussian texture offsets & weights
			float deviation = 3.0f;
			float texelSize = 1.0f / (float)mBloomSize;

			// central sample, no offset
			mBloomTexOffsetsHorz[0][0] = 0.0f;
			mBloomTexOffsetsHorz[0][1] = 0.0f;
			mBloomTexOffsetsVert[0][0] = 0.0f;
			mBloomTexOffsetsVert[0][1] = 0.0f;
			mBloomTexWeights[0][0] = mBloomTexWeights[0][1] =
				mBloomTexWeights[0][2] = Ogre::Math::gaussianDistribution(0, 0, deviation);
			mBloomTexWeights[0][3] = 1.0f;

			// 'pre' samples
			for(int i = 1; i < 8; ++i)
			{
				mBloomTexWeights[i][0] = mBloomTexWeights[i][1] =
					mBloomTexWeights[i][2] = 1.25f * Ogre::Math::gaussianDistribution((Ogre::Real)i, 0, deviation);
				mBloomTexWeights[i][3] = 1.0f;
				mBloomTexOffsetsHorz[i][0] = i * texelSize;
				mBloomTexOffsetsHorz[i][1] = 0.0f;
				mBloomTexOffsetsVert[i][0] = 0.0f;
				mBloomTexOffsetsVert[i][1] = i * texelSize;
			}
			// 'post' samples
			for(int i = 8; i < 15; ++i)
			{
				mBloomTexWeights[i][0] = mBloomTexWeights[i][1] =
					mBloomTexWeights[i][2] = mBloomTexWeights[i - 7][0];
				mBloomTexWeights[i][3] = 1.0f;

				mBloomTexOffsetsHorz[i][0] = -mBloomTexOffsetsHorz[i - 7][0];
				mBloomTexOffsetsHorz[i][1] = 0.0f;
				mBloomTexOffsetsVert[i][0] = 0.0f;
				mBloomTexOffsetsVert[i][1] = -mBloomTexOffsetsVert[i - 7][1];
			}

		}
	}
}

void gkOgreCompositorHDRListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
	// Prepare the fragment params offsets
	switch(pass_id)
	{
	//case 994: // rt_lum4
	case 993: // rt_lum3
	case 992: // rt_lum2
	case 991: // rt_lum1
	case 990: // rt_lum0
		break;
	case 800: // rt_brightpass
		break;
	case 701: // rt_bloom1
		{
			// horizontal bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams =
				mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsHorz[0], 15);
			fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

			break;
		}
	case 700: // rt_bloom0
		{
			// vertical bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams =
				mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsVert[0], 15);
			fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

			break;
		}
	}
}

void gkOgreCompositorHDRListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
}

//--

gkOgreCompositorGaussianListener::gkOgreCompositorGaussianListener()
{
}

gkOgreCompositorGaussianListener::~gkOgreCompositorGaussianListener()
{
}

void gkOgreCompositorGaussianListener::notifyViewportSize(int width, int height)
{
	mVpWidth = width;
	mVpHeight = height;
	// Calculate gaussian texture offsets & weights
	float deviation = 3.0f;
	float texelSize = 1.0f / (float)std::min(mVpWidth, mVpHeight);

	// central sample, no offset
	mBloomTexOffsetsHorz[0][0] = 0.0f;
	mBloomTexOffsetsHorz[0][1] = 0.0f;
	mBloomTexOffsetsVert[0][0] = 0.0f;
	mBloomTexOffsetsVert[0][1] = 0.0f;
	mBloomTexWeights[0][0] = mBloomTexWeights[0][1] =
		mBloomTexWeights[0][2] = Ogre::Math::gaussianDistribution(0, 0, deviation);
	mBloomTexWeights[0][3] = 1.0f;

	// 'pre' samples
	for(int i = 1; i < 8; ++i)
	{
		mBloomTexWeights[i][0] = mBloomTexWeights[i][1] =
			mBloomTexWeights[i][2] = Ogre::Math::gaussianDistribution((Ogre::Real)i, 0, deviation);
		mBloomTexWeights[i][3] = 1.0f;
		mBloomTexOffsetsHorz[i][0] = i * texelSize;
		mBloomTexOffsetsHorz[i][1] = 0.0f;
		mBloomTexOffsetsVert[i][0] = 0.0f;
		mBloomTexOffsetsVert[i][1] = i * texelSize;
	}
	// 'post' samples
	for(int i = 8; i < 15; ++i)
	{
		mBloomTexWeights[i][0] = mBloomTexWeights[i][1] =
			mBloomTexWeights[i][2] = mBloomTexWeights[i - 7][0];
		mBloomTexWeights[i][3] = 1.0f;

		mBloomTexOffsetsHorz[i][0] = -mBloomTexOffsetsHorz[i - 7][0];
		mBloomTexOffsetsHorz[i][1] = 0.0f;
		mBloomTexOffsetsVert[i][0] = 0.0f;
		mBloomTexOffsetsVert[i][1] = -mBloomTexOffsetsVert[i - 7][1];
	}
}

void gkOgreCompositorGaussianListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
	// Prepare the fragment params offsets
	switch(pass_id)
	{
	case 701: // blur horz
		{
			// horizontal bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams =
				mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsHorz[0], 15);
			fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

			break;
		}
	case 700: // blur vert
		{
			// vertical bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams =
				mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
			fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsVert[0], 15);
			fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

			break;
		}
	}
}

void gkOgreCompositorGaussianListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
}

