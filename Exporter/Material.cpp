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

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	
	#pragma warning(disable:4786)
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <assert.h>

#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MColor.h>

#include "Material.h"
#include "Texture.h"

std::map<std::string, unsigned int> g_MaterialToID_Map; 
unsigned int g_MatID_gen = 0;

unsigned int GetMaterialID(const MString &material) 
{
	std::map<std::string, unsigned int>::iterator it = g_MaterialToID_Map.find(material.asChar());

	if (it != g_MaterialToID_Map.end()) 
	{
		return it->second;
	}
	return 0;
}

void ClearMaterials(SModel* model)
{
	g_MaterialToID_Map.clear();	g_MatID_gen = 0;

	free( model->materials );
}

void ExtractAttribute(MFnDependencyNode& fn, std::string name, SMatAttrib& attrib)
{
	MPlug p;

	// extract attribute color
	std::string r = name + "R";
	std::string g = name + "G";
	std::string b = name + "B";

	p = fn.findPlug(r.c_str());	p.getValue(attrib.color[0]);
	p = fn.findPlug(g.c_str());	p.getValue(attrib.color[1]);
	p = fn.findPlug(b.c_str());	p.getValue(attrib.color[2]);

	// extract attribute texture
	MString texname;

	p = fn.findPlug(name.c_str());
	
	MPlugArray plugs;
	p.connectedTo(plugs,true,true);

	for(int i = 0; i != plugs.length(); ++i)
	{
		if (plugs[i].node().apiType() == MFn::kFileTexture) 
		{
			MFnDependencyNode fnDep(plugs[i].node());
			texname = fnDep.name();
			break;
		}
	}

	attrib.textureID = GetTextureID( texname );
}

void ExtractNormalMap(MFnDependencyNode& fn, unsigned int* tex = NULL)
{
	MString texname;

	MPlug p = fn.findPlug( "normalCamera" );
	MPlugArray plugs;
	p.connectedTo(plugs,true,true);

	for(int i = 0; i != plugs.length(); ++i)
	{
		if (plugs[i].node().apiType() == MFn::kBump) 
		{
			MFnDependencyNode fnDepBump( plugs[i].node() );
			MPlug pp = fnDepBump.findPlug( "bumpValue" );

			MPlugArray pplugs;
			pp.connectedTo(pplugs, true, true);
			
			for(int j = 0; j != pplugs.length(); ++j)
			{
				if (pplugs[j].node().apiType() == MFn::kFileTexture) 
				{
					MFnDependencyNode fnDep(pplugs[j].node());
					texname = fnDep.name();
					break;
				}
			}
		}
	}

	*tex = GetTextureID( texname );
}

void ExtractMaterials(SModel* model)
{
	MItDependencyNodes itDep(MFn::kLambert);

	// count materials
	model->numMaterials = 0;
	while (!itDep.isDone()) 
	{
		model->numMaterials++;
		itDep.next();
	}

	// extract each material data
	if ( model->numMaterials > 0 )
	{
		model->materials = (SMaterial*)calloc(model->numMaterials, sizeof(SMaterial));
		unsigned int count = 0;
		itDep.reset(MFn::kLambert);
		while (!itDep.isDone()) 
		{
			SMaterial* mat = &model->materials[ count ];
			mat->type = MATERIAL_LAMBERT;
			MFnLambertShader fnLambert( itDep.item() );
			strcpy_s( mat->name, fnLambert.name().asChar() );
			g_MaterialToID_Map.insert( std::make_pair( mat->name, ++g_MatID_gen ) );
			mat->materialID = g_MatID_gen;

			ExtractAttribute(fnLambert, "ambientColor",  mat->ambient);
			ExtractAttribute(fnLambert, "color",		 mat->diffuse);
			ExtractAttribute(fnLambert, "incandescence", mat->incandescence);
			ExtractAttribute(fnLambert, "transparency",  mat->transparency);

			ExtractNormalMap(fnLambert, &mat->normalmapTex);

			switch(itDep.item().apiType()) 
			{
				case MFn::kPhong:
					{
						MFnPhongShader fnPhong( itDep.item() );
						mat->type = MATERIAL_PHONG;
						ExtractAttribute(fnPhong, "specularColor", mat->specular);
						mat->shininess = (float)(fnPhong.cosPower());
					}
					break;
				case MFn::kBlinn:
					{
						MFnBlinnShader fnBlinn( itDep.item() );
						mat->type = MATERIAL_BLINN;
						ExtractAttribute(fnBlinn, "specularColor", mat->specular);
						mat->eccentricity = (float)(fnBlinn.eccentricity()); 
						mat->specularRollOff = (float)(fnBlinn.specularRollOff()); 
					}
					break;
			}

			itDep.next();
			count++;
		}
	}
}

void OutputAttribute(std::ostream& ofs, std::string name, const SMatAttrib& attrib)
{
	ofs << "\t\t" << name << " " << attrib.color[0] << " " << attrib.color[1] << " " << attrib.color[2] << " tex= " << attrib.textureID << "\n";
}

void OutputMaterials(std::ostream &ofs, SModel* model)
{
	ofs << "METERIALS" << std::endl;
	ofs << "NumMaterials= " << model->numMaterials << std::endl;

	if ( model->numMaterials == 0 )
	{
		model->materials = NULL;
		return;
	}

	for ( uint i = 0; i < model->numMaterials; i++ )
	{
		SMaterial* mat = &model->materials[ i ];
		ofs << "\t" << "MaterialType= " << mat->type << std::endl;
		ofs << "\t" << SMatNames[mat->type] << " " << mat->name << "\n";
		ofs << "\t\t" << "materialID= " << mat->materialID << std::endl;

		// set float format
		std::streamsize oldPrec = ofs.precision(5);
		ofs.setf(std::ios::fixed,std::ios::floatfield);	// floatfield set to fixed

		OutputAttribute(ofs, "ambient       ", mat->ambient);
		OutputAttribute(ofs, "diffuse       ", mat->diffuse);
		OutputAttribute(ofs, "incandescence ", mat->incandescence);
		OutputAttribute(ofs, "transparency  ", mat->transparency);

		ofs << "\t\t" << "normalmap      tex= " << mat->normalmapTex << "\n";

		switch(mat->type) 
		{
			case MATERIAL_PHONG:
				{
					OutputAttribute(ofs, "specular      ", mat->specular);
					ofs	<< "\t\t" << "shininess      " << mat->shininess << std::endl; 
				}
				break;
			case MATERIAL_BLINN:
				{
					OutputAttribute(ofs, "specular      ", mat->specular);
					ofs	<< "\t\t" << "eccentricity   " << mat->eccentricity << std::endl; 
					ofs	<< "\t\t" << "specularRollOff " << mat->specularRollOff << std::endl; 
				}
				break;
			case MATERIAL_LAMBERT:
				{
					// nothing
				}
				break;
			default:
				assert(0); exit(1);
		}
	
		// unset float format
		ofs.precision(oldPrec);
		ofs.unsetf(std::ios::floatfield);
	}
}

