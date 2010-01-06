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

#include "../SharedStructures.h"

#include "XBlendShape.h"
#include "XJointAnim.h"

typedef std::vector< XBlendShape* > XBlendShapes;
typedef XBlendShapes::iterator XItBlendShape;

class XAnimation : SAnimation
{
	friend std::istream& operator >> (std::istream& ifs, XAnimation &anim);

public:
	std::string		m_AnimName;
	XTimeInfo		m_Time;

	int				m_NumBlendShapes;
	XBlendShapes	m_BlendShapes;
	XJointAnim		m_JointAnim;


	XAnimation(void);
	~XAnimation(void);

	// update time
	void Update(float dt);

	// update blendshaped meshes
	void updateBlendShapes(void);

	// update transformations of skeleton
	void updateSkeleton(XSkeleton& skeleton);

};
