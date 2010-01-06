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

#include "Animation.h"

#include <assert.h>

#include <maya/MAnimControl.h>
#include <maya/MFileIO.h>

Animation::Animation(void)
{
}

Animation::~Animation(void)
{
}

void OutputAnimation(std::ostream &ofs, BlendShapeList &blendShapes, SAnimation* anim, SkeletonWriter& skel)
{
	// get file name
	std::string filePath( MFileIO::currentFile().asChar() ); 
	std::string key("/");
	size_t found = filePath.rfind(key);
	assert(found != std::string::npos);
	std::string fileName = filePath.substr( found+1 );

	ofs << "ANIMATION " 
		<< fileName
		<< "\n";
	strcpy_s( anim->name, fileName.c_str() );
	
	// get start and end times
	int startFrame = static_cast<int>(MAnimControl::minTime().as(MTime::kNTSCField));
	int endFrame   = static_cast<int>(MAnimControl::maxTime().as(MTime::kNTSCField));
	float startTime = static_cast<float>(MAnimControl::minTime().as(MTime::kSeconds));
	float endTime   = static_cast<float>(MAnimControl::maxTime().as(MTime::kSeconds));

	ofs << "\t" << "StartTime " << (float)startTime << std::endl;
	ofs << "\t" << "EndTime "   << (float)endTime   << std::endl;
	ofs << "\t" << "NumFrames " << (endFrame-startFrame+1) << std::endl;
	ofs << "\t" << "NumBlendShapes " << blendShapes.m_NumBlendShapes << std::endl;

	anim->startTime = (float)startTime;
	anim->endTime = (float)endTime;
	anim->numFrames = (endFrame-startFrame+1);
	anim->numBlendAnims = blendShapes.m_NumBlendShapes;

	if ( anim->numBlendAnims > 0 )
		blendShapes.outputBlendShapeAnims(ofs, startFrame, endFrame, anim);

	skel.writeJointAnimation( ofs, startFrame, endFrame );

}