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

#include <assert.h>
#include <vector>
#include <ostream>
 
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MPlug.h>
#include <maya/MObjectArray.h>
#include <maya/MFnMesh.h>
#include <maya/MItGeometry.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MPlugArray.h>
#include <maya/MFnGeometryFilter.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MAnimControl.h>

#include "BlendShape.h"
#include "Mesh.h"

void DisableBlendShapes()
{   
	MItDependencyNodes it(MFn::kBlendShape);

	while( !it.isDone() )
	{
		MFnBlendShapeDeformer fn(it.item()); 

		// get the envelope attribute plug
		MPlug plug = fn.findPlug("en");

		// set to 0 to disable FFD effect
		plug.setValue(0.0f);

		it.next();
	}
} 

void EnableBlendShapes()
{	
	MItDependencyNodes it(MFn::kBlendShape);
	while( !it.isDone() )
	{
		MFnBlendShapeDeformer fn(it.item()); 

		// get the envelope attribute plug
		MPlug plug = fn.findPlug("en");

		// set to 1 to enable FFD effect
		plug.setValue(1.0f);

		it.next();
	}
} 

BlendShape::BlendShape(void)
{
	m_WeightList = NULL;
}

BlendShape::~BlendShape(void)
{
	if (m_WeightList)
		delete [] m_WeightList;
}

void BlendShape::extractBlendShape(MObject& DeformerObj)
{
	// attach the function set to the object
	MFnBlendShapeDeformer fnBlendShape(DeformerObj);
	m_Name = fnBlendShape.name().asChar();

	// get a list of objects that this blend shape deforms
	MObjectArray base_objects;
	fnBlendShape.getBaseObjects(base_objects);
	assert( base_objects.length() == 1 ); // Assume only one base object

	// get the base shape
	MObject Base = base_objects[0]; 
	MFnDependencyNode fnDep(Base);
	m_BaseName = fnDep.name().asChar();

	// get the weight
	m_NumWeights = fnBlendShape.numWeights();
	MIntArray indexList;
	fnBlendShape.weightIndexList(indexList);
	assert( m_NumWeights == indexList.length() );

	if ( m_NumWeights > 0 )
	{
		m_WeightList = new BlendShapeWeight[ m_NumWeights ];

		// only want non-history items
		for(unsigned int i = 0; i != m_NumWeights; ++i) 
		{
			// get an array of target shapes
			MObjectArray targets;
			fnBlendShape.getTargets(Base, indexList[i], targets);
			
			m_WeightList[i].extractWeightAnimation( fnBlendShape, targets[0], indexList[i] );

			unsigned int numTarget = targets.length();
			if ( numTarget == 1 )
				m_WeightList[i].extractTarget( fnBlendShape, targets[0], indexList[i] );
			else if ( numTarget == 0 )
			{
				// oops, target is probably deleted
				// reconstruct target shape from base mesh by setting only this weight to 1.0f
				EnableBlendShapes();
				for ( int j = 0; j != m_NumWeights; ++j )
					fnBlendShape.setWeight( indexList[j], 0.0f );
				fnBlendShape.setWeight( indexList[i], 1.0f );
				m_WeightList[i].extractTarget( fnBlendShape, Base, indexList[i] );
				DisableBlendShapes();
			}
			else
			{
				assert(0); // more than two targets are not supported yet.
			}
		}
	}
}

void BlendShape::outputBlendAnim(std::ostream& ofs, int startFrame, int endFrame, SBlendAnim* blendAnim)
{
	// print blend shape name
	ofs << "\t\t" << "BLENDSHAPE " << m_Name << std::endl;

	// write base name and ID
	ofs << "\t\t\t" << "Base " << m_BaseName << std::endl;
	ofs << "\t\t\t" << "id= " << GetMeshID( m_BaseName ) << std::endl;
	ofs << "\t\t\t" << "NumWeights " << m_NumWeights << std::endl;

	strcpy_s( blendAnim->name, m_Name.c_str() );
	strcpy_s( blendAnim->baseName, m_BaseName.c_str() );
	blendAnim->baseMeshID = GetMeshID( m_BaseName );
	blendAnim->numWeights = m_NumWeights;

	int numFrames = endFrame - startFrame + 1;

	// set float format
	std::streamsize oldPrec = ofs.precision(5);
	ofs.setf(std::ios::fixed,std::ios::floatfield);	// floatfield set to fixed
	ofs.setf(std::ios::showpos);					// write + for positive 

	uint numFloat = numFrames * blendAnim->numWeights;
	if( numFloat > 0 )
	{
		blendAnim->weights = (float*)calloc( numFloat, sizeof(float) ); //new float[ numFloat ];
		int count = 0;
		for(int i = 0; i < numFrames; i++) 
		{
			ofs << "\t\t\t\t" ;
			for ( unsigned int j = 0; j < m_NumWeights; j++ )
			{
				BlendShapeWeight *weight = &m_WeightList[j];
				ofs << weight->m_Weights[i] << " ";
				blendAnim->weights[ count ] = weight->m_Weights[i];
				count++;
			}
			ofs << std::endl;
		}
		assert( count == numFloat );
	}

	// unset float format
	ofs.precision(oldPrec);
	ofs.unsetf(std::ios::floatfield);
	ofs.unsetf(std::ios::showpos);
}
