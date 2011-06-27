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
#include "akMesh.h"
#include "akSkeleton.h"
#include "akAnimationClip.h"

#include "btAlignedAllocator.h"

#include <sstream>

akDemoBase::akDemoBase() : m_frame(0), m_time(0), m_fpsLastTime(0), m_stepLastTime(0),
	m_lastfps(0), m_canUseVbo(false), m_drawNormals(false), m_wireframe(false), m_textured(true),
	m_shaded(true), m_drawColor(true), m_useVbo(true), m_dualQuatUse(0), m_normalMethod(2),
	m_drawSkeleton(true)
{
	m_camera = (akCamera*) btAlignedAlloc(sizeof(akCamera), 16);
}

akDemoBase::~akDemoBase()
{
	unsigned int i;
	
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
	
	for( i=0; i<m_textures.size(); i++)
	{
		glDeleteTextures( 1, &m_textures[i] );
	}
	
	m_meshes.clear();
	m_skeletons.clear();
	m_animations.clear();
	m_objects.clear();
	m_textures.clear();

	btAlignedFree(m_camera);
}

void akDemoBase::start(void)
{
	init();
	
	if (GLEW_ARB_vertex_buffer_object)
		m_canUseVbo = true;
	
	m_meshCount = m_objects.size();
	m_subCount = 0;
	m_vertexCount = 0;
	m_triCount = 0;
	for(unsigned int i=0; i<m_objects.size(); i++)
	{
		akEntity* object = m_objects.at(i);
		object->init(m_canUseVbo, this);
		
		m_subCount += object->getMesh()->getNumSubMeshes();
		m_vertexCount += object->getMesh()->getVertexCount();
		m_triCount  += object->getMesh()->getTriangleCount();
	}
	
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	GLfloat LightAmbient[]= { 1.0f, 1.0f, 01.0f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 01.0f, 1.0f };
	GLfloat LightPosition[]= { 1.0f, 2.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

void akDemoBase::step(akScalar time)
{
	unsigned int i;
	for( i=0; i<m_objects.size(); i++)
	{		
		akEntity* object = m_objects.at(i);
		object->step(time, m_dualQuatUse, m_normalMethod);
	}
}

void akDemoBase::drawString(float x, float y, const char *s)
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
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	
	
	glRasterPos2f(x, y);
	
	for (c = s; *c != '\0'; c++) {
		if( *c != '\n' )
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
		else
		{
			y += 12;
			glRasterPos2f(x, y);
		}
	}
	
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	
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
	if (m_time - m_fpsLastTime > 1000) {
		m_lastfps = m_frame*1000.0f/(m_time-m_fpsLastTime);
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

void akDemoBase::keyboardCallback(unsigned char key,int x, int y)
{
//	printf("%d\n", key);
	switch(key)
	{
		case 110: // N
			m_drawNormals = m_drawNormals? false:true;
			break;
		case 116: // T
			m_textured = m_textured? false:true;
			break;
		case 115: // S
			m_shaded = m_shaded? false:true;
			break;
		case 100: // D
			m_dualQuatUse +=1;
			if(m_dualQuatUse>3) m_dualQuatUse = 0;
			break;
		case 119: // W
			m_wireframe = m_wireframe? false:true;
			break;
		case 99: // C
			m_drawColor = m_drawColor? false:true;
			break;
		case 118: // V
			m_useVbo = m_useVbo? false:true;
			break;
		case 109: // M
			m_normalMethod +=1;
			if(m_normalMethod>3) m_normalMethod = 0;
			break;
		case 112: // P
			m_drawSkeleton = m_drawSkeleton? false:true;
			break;
	}
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

void akDemoBase::addTexture(const utHashedString &name, GLuint tex)
{
	m_textures.insert(name.hash(), tex);
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

GLuint akDemoBase::getTexture(const utHashedString &name)
{
	UTsize pos = m_textures.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_textures.at(pos);
}

int akDemoBase::getFps(void)
{
	return m_lastfps;
}

void akDemoBase::render()
{
	glViewport(0, 0, m_windowx, m_windowy);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	if( m_wireframe )
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_camera->m_fov, (float)m_windowx/m_windowy, m_camera->m_clipStart, m_camera->m_clipEnd);
	
	glMatrixMode(GL_MODELVIEW);
	akMatrix4 cam_inv_m = inverse(m_camera->m_transform.toMatrix());
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
	bool shaded = m_shaded && !m_wireframe;
	unsigned int i;
	for( i=0; i<m_objects.size(); i++)
	{
		m_objects.at(i)->draw(m_drawNormals, m_drawColor, m_textured, m_useVbo, shaded, m_drawSkeleton);
	}
	
	// Stats
	std::stringstream UIString;
	UIString << "FPS: " << getFps() << "\n\n";
	UIString << "Meshes: " << m_meshCount <<"\n";
	UIString << "Submeshes: " << m_subCount <<"\n";
	UIString << "Triangles: " << m_triCount <<"\n";
	UIString << "Vertices: " << m_vertexCount <<"\n";
	utString str = UIString.str();
	glColor3f(0.2f, 0.2f, 0.2f);
	drawString(10, 15, str.c_str());
	
	// Info text
	str.clear();
	
	str += "P: Togle draw pose (";
	str += (m_drawSkeleton? "ON":"OFF");
	str += ")\n";
	
	str += "W: Togle wireframe (";
	str += (m_wireframe? "ON":"OFF");
	str += ")\n";
	
	str += "S: Togle shading (";
	str += (m_shaded? "ON":"OFF");
	str += ")\n";
	
	str += "T: Togle textures (";
	str += (m_textured? "ON":"OFF");
	str += ")\n";
	
	str += "C: Togle vertex color (";
	str += (m_drawColor? "ON":"OFF");
	str += ")\n";
	
	str += "N: Togle draw normals (";
	str += (m_drawNormals? "ON":"OFF");
	str += ")\n";
	
	str += "D: Cycle dual quaternion skinning (";
	switch(m_dualQuatUse)
	{
	case 0:
		str += "File setting";
		break;
	case 1:
		str += "Matrix";
		break;
	case 2:
		str += "Dual Quat";
		break;
	case 3:
		str += "Dual Quat Antiposality";
		break;
	}
	str += ")\n";
	
	str += "M: Cycle normal skinning (";
	switch(m_normalMethod)
	{
	case 0:
		str += "No normal skinning";
		break;
	case 1:
		str += "No scaling";
		break;
	case 2:
		str += "Uniform scaling";
		break;
	case 3:
		str += "Full";
		break;
	}
	str += ")\n";
	
	if(m_canUseVbo)
	{
		str += "V: Togle VBO (";
		str += (m_useVbo? "ON":"OFF");
		str += ")\n";
	}
	drawString(10, m_windowy-100, str.c_str());
	
	glFlush();
	glutSwapBuffers();
}
