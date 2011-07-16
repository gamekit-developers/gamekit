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

#include "akGlutDemo.h"

#include "akDemoBase.h"

static akDemoBase* demo=0;

void display(void)
{
	demo->displayCallback();
}


void idle(void)
{
	demo->idleCallback();
}

void reshape(int w, int h)
{
	demo->reshapeCallback(w, h);
}

void mouse(int button,int state,int x,int y)
{
}

void motion(int x,int y)
{
}

void keyboard(unsigned char key,int x,int y)
{
	demo->keyboardCallback(key, x, y);
}

void special(int key,int x,int y)
{
}

int startGlutDemo(int argc, char** argv, akDemoBase* d, const char* title)
{
	demo = d;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//glutInitWindowPosition(100,30);
	glutInitWindowSize(demo->getWindowWidth(), demo->getWindowHeigth());
	glutCreateWindow(title);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glewInit();
	
	demo->start();
	
	glutMainLoop();
	return 0;
}
