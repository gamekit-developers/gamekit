/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifdef WIN32
#include <Windows.h>
#include <GL/glut.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "akEntity.h"
#include "akMesh.h"
#include "akSkeleton.h"
#include "akSkeletonPose.h"
#include "akDualQuat.h"


akEntity::akEntity() : m_mesh(0), m_skeleton(0), m_pose(0), m_useDualQuatSkinning(false)
{
}

akEntity::~akEntity()
{
	if(m_pose)
		delete m_pose;
	
	if(m_matrixPalette.size()>0)
		m_matrixPalette.clear();
	
	if(m_dualquatPalette.size()>0)
		m_dualquatPalette.clear();
}

void akEntity::setSkeleton(akSkeleton *skel)
{
	m_skeleton = skel;
	
	if(m_pose)
		delete m_pose;
	m_pose = new akSkeletonPose(m_skeleton, akSkeletonPose::SP_BINDING_SPACE);
	
	if(m_matrixPalette.size()>0)
		m_matrixPalette.clear();
	m_matrixPalette.reserve(m_skeleton->getNumJoints());
	m_matrixPalette.resize(m_skeleton->getNumJoints());
	
	if(m_dualquatPalette.size()>0)
		m_dualquatPalette.clear();
	m_dualquatPalette.reserve(m_skeleton->getNumJoints());
	m_dualquatPalette.resize(m_skeleton->getNumJoints());
}


void akEntity::draw(void)
{
	glPushMatrix();
	
	akMatrix4 mat = m_transform.toMatrix();
	glMultMatrixf((GLfloat*)&mat);
	
	if(m_mesh)
	{
		int j, tot;
		unsigned int stride;
		akVector3* positions;
		
		for(j=0; j< m_mesh->getNumSubMeshes(); j++)
		{
			akSubMesh* sm = m_mesh->getSubMesh(j);
			const akVertexBuffer* vbuf = sm->getVertexBuffer();
			
			tot = vbuf->getVerticesNumber();
			
			positions = 0;
			
			if(m_skeleton)
				vbuf->getElement(akVertexBuffer::VB_DU_POSITION, akVertexBuffer::VB_DT_3FLOAT32, 2, (void**)&positions, &stride);
			else
				vbuf->getElement(akVertexBuffer::VB_DU_POSITION, akVertexBuffer::VB_DT_3FLOAT32, 1, (void**)&positions, &stride);
			
			if(positions)
			{			
				glEnable(GL_DEPTH_TEST);
				glBegin(GL_TRIANGLES);
				glColor3f(0, 0, 0);
				
				int i;
				for(i=0; i<tot; i++)
				{
					glVertex3f(positions[i].getX(), positions[i].getY(), positions[i].getZ());
				}
				glEnd();
			}
		}
	}
	
	if(m_pose)
	{
		
		glDisable(GL_DEPTH_TEST);
		glLineWidth(1.5);
		
		int i, tot;
		tot = m_pose->getNumJoints();
		for(i=0; i<tot; i++)
		{
			glPushMatrix();
			
			akTransformState* jointpose = m_pose->getJointPose(i);
			akMatrix4 mat = jointpose->toMatrix();
			glMultMatrixf((GLfloat*)&mat);
	
			glBegin(GL_LINES);
			glColor3f(1,0,0);
			glVertex3f(0.1,0,0);
			glVertex3f(0,0,0);
			
			glColor3f(0,1,0);
			glVertex3f(0,0.1,0);
			glVertex3f(0,0,0);
			
			glColor3f(0,0,1);
			glVertex3f(0,0,0.1);
			glVertex3f(0,0,0);
			glEnd();
			
			glPopMatrix();
		}
		
		glLineWidth(1);
	}
	
	glPopMatrix();
}

