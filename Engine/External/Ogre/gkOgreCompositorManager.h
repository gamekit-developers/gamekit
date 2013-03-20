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
#ifndef _gkOgreCompositorManager_h_
#define _gkOgreCompositorManager_h_

#include "utSingleton.h"


#define GK_COMPOSITOR_BLOOM				"Bloom"
#define GK_COMPOSITOR_BW				"B&W"
#define GK_COMPOSITOR_EMBOSSED			"Embossed"
#define GK_COMPOSITOR_SHARPEN_EDGES		"Sharpen Edges"
#define GK_COMPOSITOR_INVERT			"Invert"
#define GK_COMPOSITOR_POSTERIZE			"Posterize"
#define GK_COMPOSITOR_LAPLACE			"Laplace"
#define GK_COMPOSITOR_TILING			"Tiling"
#define GK_COMPOSITOR_HDR				"HDR"				//custom listener
#define GK_COMPOSITOR_GAUSSIAN_BLUR		"Gaussian Blur"		//custom listener
#define GK_COMPOSITOR_RADIAL_BLUR		"Radial Blur"
#define GK_COMPOSITOR_HALFTONE			"Halftone"			//hlsl, halftone texture
#define GK_COMPOSITOR_DITHER			"Dither"			//dither texture

//need Compositor Textures
#define GK_COMPOSITOR_GLASS				"Glass"				//WaterNormal1.tga(193K)
//#define GK_COMPOSITOR_OLD_TV			"Old TV"			//hlsl only, NoiseVolume.dds(2M)
#define GK_COMPOSITOR_OLD_MOVIE			"Old Movie"			//hlsl, 8x8PagesSplotches2.png(56K), 1D_Noise.png(2K), Sepia1D.tga(1K)
#define GK_COMPOSITOR_ASCII				"ASCII"				//hlsl only, ASCII.dds(130K)
#define GK_COMPOSITOR_NIGHT_VISION		"Night Vision"		//hlsl, Random3D.dds(257K)
#define GK_COMPOSITOR_MOTION_BLUR		"Motion Blur"		//hlsl only, manual compositor 
#define GK_COMPOSITOR_HEAT_VISION		"Heat Vision"		//hlsl only, HeatNoise.tga(191K), HeatLookup.tga(1K), custom listener, manual compositor

#define GK_USE_COMPOSITOR_HEAT_VISION	1

enum gkCompositorOp 
{ 
	GK_COMPOSITOR_OP_ADD, 
	GK_COMPOSITOR_OP_DEL, 
	GK_COMPOSITOR_OP_REPLACE, 
	GK_COMPOSITOR_OP_RESET 
};

class gkOgreCompositorListenerFactoryLogic;

class gkCompositorManager : utSingleton<gkCompositorManager>
{
	bool m_halftonInited;
	bool m_ditherInited;
	bool m_heatVisionInited;
	bool m_motionBlurInited;

	utArray<gkOgreCompositorListenerFactoryLogic*> m_logics;

public:
	gkCompositorManager();
	virtual ~gkCompositorManager();

	void initialize();

	bool setCompositorChain(gkCompositorOp op, const gkString& compositorName, int viewportIndex = 0);
	bool setCompositorChain(gkCompositorOp op, const gkString& compositorName, gkViewport* viewport);
	void removeCompositorChain(gkViewport* viewport);

	UT_DECLARE_SINGLETON(gkCompositorManager)
};

#endif//_gkOgreCompositorManager_h_