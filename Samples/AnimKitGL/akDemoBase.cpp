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

#include <stdio.h>

#include "akDemoBase.h"

#include "akEntity.h"

#ifdef WIN32
#include <Windows.h>
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include "akMesh.h"
#include "akSkeleton.h"
#include "akAnimationClip.h"


akDemoBase::akDemoBase() : m_frame(0), m_time(0), m_fpsLastTime(0), m_stepLastTime(0), m_lastfps(0)
{

}

akDemoBase::~akDemoBase()
{
	int i;
	
	for( i=0; i<m_objects.size(); i++)
	{
		delete m_objects[i];
	}
	
	for( i=0; i<m_animations.size(); i++)
	{
		delete m_animations[i];
	}
	
	for( i=0; i<m_meshes.size(); i++)
	{
		delete m_meshes[i];
	}
	
	for( i=0; i<m_skeletons.size(); i++)
	{
		delete m_skeletons[i];
	}
		
	m_meshes.clear();
	m_skeletons.clear();
	m_animations.clear();
	m_objects.clear();
}

void akDemoBase::drawString(float x, float y, char *s)
{
	const char *c;
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, m_windowx, 0, m_windowy);
	glScalef(1, -1, 1);
	glTranslatef(0, -m_windowy, 0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glLoadIdentity();
	
	
	glRasterPos2f(x, y);
	
	for (c = s; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
	}
	
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
}



void akDemoBase::reshapeCallback(int w, int h)
{
	m_windowx = w;
	m_windowy = h ? h : 1;
}

void akDemoBase::idleCallback(void)
{
	//avoid steping time for the 1rst frame
	if(m_stepLastTime == 0.f)
	{
		m_stepLastTime = glutGet(GLUT_ELAPSED_TIME);
		m_fpsLastTime = m_stepLastTime;
	}
	
	m_time = glutGet(GLUT_ELAPSED_TIME);
	
	// fps
	m_frame++;
	if (m_time - m_fpsLastTime > 500) {
		m_lastfps = m_frame*1000.0/(m_time-m_fpsLastTime);
		m_fpsLastTime = m_time;		
		m_frame = 0;
	}
	
	// step time
	akScalar dt;
	dt = (m_time - m_stepLastTime)/1000.f;
	m_stepLastTime = m_time;
	
	//avoid too big steps while debugging
	if(dt >= 0.2)
		dt = 0.016666;
	
	step(dt);
	
	render();
}

void akDemoBase::displayCallback(void)
{
	render();
}

void akDemoBase::addAnimation(const utHashedString& name, akAnimationClip* anim)
{
	m_animations.insert(name.hash(), anim);
}

void akDemoBase::addMesh(const utHashedString& name, akMesh *mesh)
{
	m_meshes.insert(name.hash(), mesh);
}

void akDemoBase::addSkeleton(const utHashedString &name, akSkeleton *skel)
{
	m_skeletons.insert(name.hash(), skel);
}

void akDemoBase::addEntity(const utHashedString &name, akEntity *ent)
{
	m_objects.insert(name.hash(), ent);
}

akAnimationClip* akDemoBase::getAnimation(const utHashedString &name)
{
	UTsize pos = m_animations.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_animations.at(pos);
}

akMesh * akDemoBase::getMesh(const utHashedString &name)
{
	UTsize pos = m_meshes.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_meshes.at(pos);
}

akSkeleton * akDemoBase::getSkeleton(const utHashedString &name)
{
	UTsize pos = m_skeletons.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_skeletons.at(pos);
}

akEntity * akDemoBase::getEntity(const utHashedString &name)
{
	UTsize pos = m_objects.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_objects.at(pos);
}


int akDemoBase::getFps(void)
{
	return m_lastfps;
}

void akDemoBase::render()
{

	glViewport(0, 0, m_windowx, m_windowy);
	
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	glEnable(GL_DEPTH_TEST);
	
//	if( options.m_bWireFrame )
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
//	else
//		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
//	if( options.m_bCulling )
		glEnable(GL_CULL_FACE);
//	else
//		glDisable(GL_CULL_FACE);
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_camera.m_fov, (float)m_windowx/m_windowy, m_camera.m_clipStart, m_camera.m_clipEnd);
	
	glMatrixMode(GL_MODELVIEW);
	Matrix4 cam_inv_m = inverse(m_camera.m_transform.toMatrix());
	glLoadMatrixf((GLfloat*)&cam_inv_m);

	// world origin axes
//	glBegin(GL_LINES);
//		glColor3f(1,0,0);
//		glVertex3f(1,0,0);
//		glVertex3f(0,0,0);
		
//		glColor3f(0,1,0);
//		glVertex3f(0,1,0);
//		glVertex3f(0,0,0);
		
//		glColor3f(0,0,1);
//		glVertex3f(0,0,1);
//		glVertex3f(0,0,0);
//	glEnd();
	
	// objects
	int i;
	for( i=0; i<m_objects.size(); i++)
	{
		m_objects.at(i)->draw();
	}
	
	// FPS
	sprintf( m_fpsString, "FPS: %4.0d",getFps());
	glColor3f(0.0f, 0.0f, 0.0f);
	drawString(30, 35, m_fpsString);
	
	
	glFlush();
	glutSwapBuffers();
}
