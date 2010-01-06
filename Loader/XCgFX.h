/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <stdio.h>
#include <math.h>
#include <stdlib.h>    /* for exit */
#include <string>
using namespace std;

#include <Cg/cg.h>     /* Cg Core API: Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>   /* Cg OpenGL API (part of Cg Toolkit) */

#include "XMaterial.h"

static const string m_ProgramDir = "CgFX/basic-lighting/";

static const int m_ProgramNum = 4;
static const string m_ProgramNames[ m_ProgramNum ] = 
{"simple", "Phong", "Blinn", "Lambert"};

static const int m_TechniqueNum = 9;
static const string m_TechniqueNames[ m_TechniqueNum ] = 
{"Main", "Main_Blending", "Main_Alphatest", 
"No_Texture", "No_Texture_Blending", 
"Main_Specular", "Main_Specular_Blending",
"Main_Transp_Blending","Main_Specular_Transp_Blending"};

class XCgFX
{
public:
	XCgFX(void);
	~XCgFX(void);

	CGcontext   m_CgContext;
	CGeffect    m_CgEffects[m_ProgramNum];

	CGtechnique m_CgTechniques[m_ProgramNum][ m_TechniqueNum ];

	// matrix parameters
	CGparameter	WorldInverseTranspose[m_ProgramNum];
	CGparameter	WorldViewProjection[m_ProgramNum];
	CGparameter	World[m_ProgramNum];
	//CGparameter	ViewInverse[m_ProgramNum];

	// light parameters
	CGparameter	Lamp0Pos[m_ProgramNum];
	CGparameter	Lamp0Color[m_ProgramNum];
	CGparameter	AmbiColor[m_ProgramNum];

	/// material parameters
	CGparameter m_Ambient[m_ProgramNum];
	CGparameter m_Diffuse[m_ProgramNum];
	CGparameter m_Specular[m_ProgramNum];
	CGparameter m_Emission[m_ProgramNum];
	CGparameter m_Transparency[m_ProgramNum];
	CGparameter m_Shininess[m_ProgramNum];

	// blinn specific material parameters
	CGparameter	Ks[m_ProgramNum];
	CGparameter	Eccentricity[m_ProgramNum];

	CGpass		m_CurrentPasses[m_ProgramNum];

	int getProgramID( string name );
	int getTechniqueID(string techniqueName);

	void initCg(void);
	void checkForCgError(const char * situation, const char* m_ProgramName = NULL);

	void setCurrentPass(string programName, const CGtechnique &tech);
	bool resetCurrentPass(string programName);

	void updateGLstates(string programName);
	void useSamplerParameter(string programName, const char *paramName, GLuint texobj);

public:
	void bindMaterial(XMaterial* material);
public:
	void unbindMaterial(XMaterial* material);

};
