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

#include <assert.h>

#include "XCgFX.h"

XCgFX::XCgFX(void)
{
	for ( int i = 0; i < m_ProgramNum; i++ )
	{
		m_CgEffects[i] = NULL;

		for ( int j = 0; j < m_TechniqueNum; j++ )
		{
			m_CgTechniques[i][j] = NULL;
		}
		
		WorldInverseTranspose[i] = NULL;
		WorldViewProjection[i] = NULL;
		World[i] = NULL;
		//ViewInverse[i] = NULL;

		Lamp0Pos[i] = NULL;
		Lamp0Color[i] = NULL;
		AmbiColor[i] = NULL;

		m_Ambient[i] = NULL;
		m_Diffuse[i] = NULL;
		m_Specular[i] = NULL;
		m_Emission[i] = NULL;
		m_Transparency[i] = NULL;
		m_Shininess[i] = NULL;

		Ks[i] = NULL;
		Eccentricity[i] = NULL;

		m_CurrentPasses[i] = NULL;
	}
}

XCgFX::~XCgFX(void)
{
}

int XCgFX::getProgramID( string name )
{
	for ( int i = 0; i < m_ProgramNum; i++ )
		if ( m_ProgramNames[i].compare( name ) == 0 )
			return i;
	assert(0);
	return -1;
}

int XCgFX::getTechniqueID(string techniqueName)
{
	for ( int i = 0; i < m_TechniqueNum; i++ )
		if ( m_TechniqueNames[i].compare( techniqueName ) == 0 )
			return i;
	assert(0);
	return -1;
}


void XCgFX::initCg(void)
{
	m_CgContext = cgCreateContext();
	checkForCgError("creating context");

#ifdef CG_DEFERRED_PARAMETER_SETTING
	// this is not in older Cg
	cgGLSetDebugMode( CG_FALSE );
	cgSetParameterSettingMode(m_CgContext, CG_DEFERRED_PARAMETER_SETTING);
#endif

	cgGLRegisterStates(m_CgContext);
	checkForCgError("registering standard CgFX states");
	cgGLSetManageTextureParameters(m_CgContext, CG_TRUE);
	checkForCgError("manage texture parameters");

	for ( int i = 0; i < m_ProgramNum; i++ )
	{
		string filePath( m_ProgramDir );
		filePath += m_ProgramNames[i];
		filePath += ".cgfx";

		m_CgEffects[i] = cgCreateEffectFromFile(m_CgContext, filePath.c_str(), NULL);
		checkForCgError("creating cgfx effect", m_ProgramNames[i].c_str());
		assert(m_CgEffects[i]);

		// initialize techniques
		for ( int j = 0; j < m_TechniqueNum; j++ )
		{
			m_CgTechniques[i][j] = cgGetNamedTechnique( m_CgEffects[i], m_TechniqueNames[j].c_str() );
			if (m_CgTechniques[i][j] && cgValidateTechnique(m_CgTechniques[i][j]) == CG_FALSE) 
			{
				fprintf(stderr, "%s: Main Technique %s did not validate.\n",	m_ProgramNames[i].c_str(), cgGetTechniqueName(m_CgTechniques[i][j]));
				assert(0); exit(1);
			}
			//fprintf(stderr, "%s: Use Main technique %s.\n", m_ProgramNames[i].c_str(), cgGetTechniqueName(m_CgTechniques[i][j]));
		}

		// initialize matrix parameters
		WorldInverseTranspose[i] =
		  cgGetEffectParameterBySemantic(m_CgEffects[i], "WorldInverseTranspose");//"ModelViewProjection");
		WorldViewProjection[i] =
		  cgGetEffectParameterBySemantic(m_CgEffects[i], "WorldViewProjection");//"ModelViewProjection");
		World[i] =
		  cgGetEffectParameterBySemantic(m_CgEffects[i], "World");//"ModelViewProjection");
		//ViewInverse[i] =
		//  cgGetEffectParameterBySemantic(m_CgEffects[i], "ViewInverse");//"ModelViewProjection");

		// initialize light parameters
		Lamp0Pos[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "Lamp0Pos");
		Lamp0Color[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "Lamp0Color");
		AmbiColor[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "AmbiColor");

		// initialize material parameters
		m_Ambient[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "m_Ambient");
		m_Diffuse[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "m_Diffuse");
		m_Specular[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "m_Specular");
		m_Emission[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "m_Emission");
		m_Transparency[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "m_Transparency");
		m_Shininess[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "m_Shininess");

		// blinn specific materlials
		Ks[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "Ks");
		Eccentricity[i] =
		  cgGetNamedEffectParameter(m_CgEffects[i], "Eccentricity");
#ifdef _DEBUG
		// report unfound params
		if (!WorldInverseTranspose[i]) {
		  fprintf(stderr,
		    "%s: must find parameter with WorldInverseTranspose semantic\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!WorldViewProjection[i]) {
		  fprintf(stderr,
		    "%s: must find parameter with WorldViewProjection semantic\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!World[i]) {
		  fprintf(stderr,
		    "%s: must find parameter with World semantic\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		//if (!ViewInverse[i]) {
		//  fprintf(stderr,
		//    "%s: must find parameter with ViewInverse semantic\n",
		//    m_ProgramNames[i].c_str());
		//  //exit(1);
		//}
		if (!Lamp0Pos[i]) {
		  fprintf(stderr, "%s: must find parameter named Lamp0Pos\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!Lamp0Color[i]) {
		  fprintf(stderr, "%s: must find parameter named Lamp0Color\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!AmbiColor[i]) {
		  fprintf(stderr, "%s: must find parameter named AmbiColor\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!m_Ambient[i]) {
		  fprintf(stderr, "%s: must find parameter named m_Ambient\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!m_Diffuse[i]) {
		  fprintf(stderr, "%s: must find parameter named m_Diffuse\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!m_Specular[i]) {
		  fprintf(stderr, "%s: must find parameter named m_Specular\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!m_Emission[i]) {
		  fprintf(stderr, "%s: must find parameter named m_Emission\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!m_Transparency[i]) {
		  fprintf(stderr, "%s: must find parameter named m_Transparency\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!m_Shininess[i]) {
		  fprintf(stderr, "%s: must find parameter named m_Shininess\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!Ks[i]) {
		  fprintf(stderr, "%s: must find parameter named Ks\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
		if (!Eccentricity[i]) {
		  fprintf(stderr, "%s: must find parameter named Eccentricity\n",
		    m_ProgramNames[i].c_str());
		  //exit(1);
		}
#endif
	}
		
}

void XCgFX::checkForCgError(const char * situation, const char* m_ProgramName)
{
	CGerror error;
	const char *string = cgGetLastErrorString(&error);

	if (error != CG_NO_ERROR) {
		if (error == CG_COMPILER_ERROR) {
			fprintf(stderr,
				"Program: %s\n"
				"Situation: %s\n"
				"Error: %s\n\n"
				"Cg compiler output...\n%s",
				m_ProgramName, situation, string,
				cgGetLastListing(m_CgContext));
		} else {
			fprintf(stderr,
				"Program: %s\n"
				"Situation: %s\n"
				"Error: %s",
				m_ProgramName, situation, string);
		}
		exit(1);
	}
}


void XCgFX::setCurrentPass(string programName, const CGtechnique &tech)
{
	int progID = getProgramID( programName );
	if ( !m_CurrentPasses[ progID ] )
		m_CurrentPasses[ progID ] = cgGetFirstPass( tech );
	cgSetPassState( m_CurrentPasses[ progID ] );
}

bool XCgFX::resetCurrentPass(string programName)
{
	int progID = getProgramID( programName );
	cgResetPassState(m_CurrentPasses[ progID ]);
	m_CurrentPasses[ progID ] = cgGetNextPass(m_CurrentPasses[ progID ]);
	return m_CurrentPasses[ progID ] ? true : false;
}


void XCgFX::updateGLstates(string programName)
{
	int progID = getProgramID( programName );
	/* Set Cg parameters for the technique's effect. */
	// Object -> World -> View -> Projection
	cgGLSetStateMatrixParameter(
		WorldInverseTranspose[progID],
		CG_GL_MODELVIEW_MATRIX,
		CG_GL_MATRIX_INVERSE_TRANSPOSE  );
	cgGLSetStateMatrixParameter(
		WorldViewProjection[progID],
		CG_GL_MODELVIEW_PROJECTION_MATRIX,
		CG_GL_MATRIX_IDENTITY );
	cgGLSetStateMatrixParameter(
		World[progID],
		CG_GL_MODELVIEW_MATRIX ,
		CG_GL_MATRIX_IDENTITY );
	//cgGLSetStateMatrixParameter(
	//	ViewInverse[progID],
	//	CG_GL_PROJECTION_MATRIX ,
	//	CG_GL_MATRIX_INVERSE );

}

void XCgFX::useSamplerParameter(string programName, const char *paramName, GLuint texobj)
{
	int progID = getProgramID( programName );
	CGparameter param = cgGetNamedEffectParameter(m_CgEffects[progID], paramName);

	if (!param) {
		fprintf(stderr, "%s: expected effect parameter named %s\n", programName.c_str(), paramName);
		assert(0);
		exit(1);
	}
	cgGLSetTextureParameter(param, texobj);
	//cgSetSamplerState(param);
}

void XCgFX::bindMaterial(XMaterial* material)
{
	string programName;
	switch( material->m_MaterialType )
	{
	case MATERIAL_PHONG:
		programName = "Phong";
		break;
	case MATERIAL_LAMBERT:
		programName = "Lambert";
		break;
	case MATERIAL_BLINN:
		programName = "Blinn";
		break;
	default:
		programName = "simple";
		break;
	}	

	updateGLstates( programName );

	string techniqueName = "No_Texture";
	// set texture
	unsigned int diffuseTex = material->m_Diffuse.m_TextureObject;
	if ( diffuseTex != 0 ) {
		useSamplerParameter( programName, "ColorSampler", material->m_Diffuse.m_TextureObject);
		techniqueName = "Main";
	}
	if ( material->m_Specular.m_TextureObject != 0 ) {
		useSamplerParameter( programName, "SpecularSampler", material->m_Specular.m_TextureObject);
		techniqueName += "_Specular";
	}
	unsigned int transpTex = material->m_Transparency.m_TextureObject;
	if ( transpTex != 0 && transpTex != diffuseTex ) {
		useSamplerParameter( programName, "TranspSampler", material->m_Transparency.m_TextureObject);
		techniqueName += "_Transp";
	}	
	//if (material->m_Ambient.m_TextureObject!=0) {
		//useSamplerParameter( programName, "ColorSampler", material->m_Ambient.m_TextureObject);
		//techniqueName = "Main";
	//}
	//if (material->m_Emission.m_TextureObject!=0) {
		//useSamplerParameter( programName, "ColorSampler", material->m_Emission.m_TextureObject);
		//techniqueName = "Main";
	//}
	//if ( techniqueName.compare( "No_Texture" ) == 0 )
	//{
	//	useSamplerParameter( programName, "ColorSampler", 0);
	//	useSamplerParameter( programName, "SpecularSampler", 0);
	//}
	// should do this when unbind this material?

	// set material parameters
	int progID = getProgramID( programName );
	if ( m_Ambient[progID] )
		cgSetParameter4fv(m_Ambient[progID], &material->m_Ambient.m_color[0]);
	if ( m_Diffuse[progID] )
		cgSetParameter4fv(m_Diffuse[progID], &material->m_Diffuse.m_color[0]);
	if ( m_Specular[progID] )
		cgSetParameter4fv(m_Specular[progID], &material->m_Specular.m_color[0]);
	if ( m_Emission[progID] )
		cgSetParameter4fv(m_Emission[progID], &material->m_Emission.m_color[0]);
	if ( m_Transparency[progID] )
		cgSetParameter4fv(m_Transparency[progID], &material->m_Transparency.m_color[0]);
	if ( m_Shininess[progID] )
		cgSetParameter1f(m_Shininess[progID], material->m_Shininess);

	// set blinn specific material parameters
	if ( programName.compare( "Blinn" ) == 0 )
	{
		if ( Ks[progID] )
			cgSetParameter4fv(Ks[progID], &material->m_Specular.m_color[0]);
		if ( Eccentricity[progID] )
			cgSetParameter1f(Eccentricity[progID], material->m_Shininess);
	}

	size_t nameLength = material->m_MaterialName.size();
	if ( nameLength > 10 && material->m_MaterialName.compare( nameLength-10,10,"_Alphatest" ) == 0 )
		techniqueName += "_Alphatest";  // HACKY
	else if ( material->m_IsTransparent )
		techniqueName += "_Blending";

	int techniqueID = getTechniqueID( techniqueName );
	setCurrentPass( programName, m_CgTechniques[progID][techniqueID] );
}

void XCgFX::unbindMaterial(XMaterial* material)
{
	string programName;
	switch( material->m_MaterialType )
	{
	case MATERIAL_PHONG:
		programName = "Phong";
		break;
	case MATERIAL_LAMBERT:
		programName = "Lambert";
		break;
	case MATERIAL_BLINN:
		programName = "Blinn";
		break;
	default:
		programName = "simple";
		break;
	}

	resetCurrentPass( programName );
}

// light parameters
	//if ( AmbiColor[progID] )
	//	cgSetParameter4fv(AmbiColor[progID], &material->m_Ambient.m_color[0]);

