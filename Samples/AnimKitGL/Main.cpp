
#ifdef WIN32
#include <Windows.h>
#include <GL/glut.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "akDemo.h"

akDemoBase* demo;

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

int main(int argc, char** argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,30);
	glutInitWindowSize(800,800);
	glutCreateWindow("AnimKit OpenGL Demo");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	
	demo = new akDemo();
	demo->start();
	
	glutMainLoop();
}

