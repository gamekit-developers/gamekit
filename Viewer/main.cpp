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
	#include <Windows.h>
	#include <GL/glut.h>
#else
	#include <GLUT/glut.h>
#endif

#include <iostream>
#include <vector>
#include <assert.h>

#include "GLExtensions.h"
#include "FrameTimer.h"

#include "XModel.h"

#ifdef WIN32
	#include "XCgFX.h"
	/// The global cgfx instance
	XCgFX cgfx;
#endif

using namespace std;

/// The global model list
vector<XModel*> modelList;

float offset = -1.0f;

float windowWidth;
float windowHeight;

XOptions options;

// camera variables [default]
float zoom = 12.800001;
float rotx = 21.500000;
float roty = -51.000000;
float tx = 0.050000;
float ty = -0.800000;
float lastx = 0.000000;
float lasty = 0.000000;
float zoomStep = 0.050000;
float panStep = 0.050000;
float ROTATION = 0.000000;

bool g_MultiSample = false;

#define NUM_CAMERA_ATTRIBS 10
char* cameraAttribs[NUM_CAMERA_ATTRIBS] = 
{
	"zoom=",
	"rotx=",
	"roty=",
	"tx=",
	"ty=",
	"lastx=",
	"lasty=",
	"zoomStep=",
	"panStep=",
	"ROTATION=", 
};

void InitCamera( const string &fileName )
{
	char buffer[256];
	std::ifstream ifs( fileName.c_str() );

	int count = 0;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> zoom;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> rotx;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> roty;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> tx;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> ty;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> lastx;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> lasty;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> zoomStep;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> panStep;
	ifs >> buffer; assert( strcmp( buffer, cameraAttribs[count++] ) == 0 );
	ifs >> ROTATION;

	ifs.close();
	return;
}

// mouse button states
unsigned char Buttons[3] = {0};

// mouse handler
void Mouse(int button,int state,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(button)
	{
	case GLUT_LEFT_BUTTON:
			Buttons[0] = ((GLUT_DOWN==state)?1:0);
			break;
	case GLUT_MIDDLE_BUTTON:
			Buttons[1] = ((GLUT_DOWN==state)?1:0);
			break;
	case GLUT_RIGHT_BUTTON:
			Buttons[2] = ((GLUT_DOWN==state)?1:0);
			break;
	default:
		break;		
	}

}

// motion handler
void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;
	
	if( Buttons[2] )
	{
		zoom -= (float) zoomStep * diffx;
	}
	else
	if( Buttons[0] )
	{
		rotx += (float) 0.5f * diffy;
		roty += (float) 0.5f * diffx;		
	}
	else
	if( Buttons[1] )
	{
		tx += (float) panStep * diffx;
		ty -= (float) panStep * diffy;
	}
}

void InitGL() 
{
#ifdef WIN32
	initGLExtensions();

	SetVSync(false);

	cgfx.initCg();
#endif

	// check multisample
	GLint buffers, samples; 
	glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
	glGetIntegerv(GL_SAMPLES, &samples);
	//printf("buffers = %d, samples = %d\n", buffers, samples);
	if ( buffers > 0 && samples > 1 )
	{
		g_MultiSample = true;
		glEnable(GL_MULTISAMPLE);
	}
}


/*
 * drawText
 *
 */
void drawText(const char * message)
{
	/* 
	 * set viewing window in world coordinates 
	 */
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); /* init projection matrix */

    //if (windowWidth <= windowHeight)
    //    glOrtho(-2.0, 2.0, -2.0 * (GLfloat) windowHeight / (GLfloat) windowWidth,
    //        2.0 * (GLfloat) windowHeight / (GLfloat) windowWidth, -1.0, 1.0);
    //else
    //    glOrtho(-2.0 * (GLfloat) windowWidth / (GLfloat) windowHeight,
    //        2.0 * (GLfloat) windowWidth / (GLfloat) windowHeight, -2.0, 2.0, -1.0, 1.0);

	/* set matrix mode to modelview */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* transformations to scale the stroke text -
     * notice how these only change stroke and raster text
	 * differently...
     */
    glScalef(0.001f, 0.001f, 0.001f);
    //glRotatef(g_rotate, 0, 0, 1.0);
    /* glTranslatef(-180, -180, 0.0); */

    glClear( GL_COLOR_BUFFER_BIT );

	/* raster pos sets the current raster position
	 * mapped via the modelview and projection matrices
	 */
	glRasterPos2f((GLfloat)-900, (GLfloat)900);//-400);

	/*
	 * write using bitmap and stroke chars
	 */
	while (*message) 
	{
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *message++ );
		//glutStrokeCharacter(GLUT_STROKE_ROMAN,*message++);
	}
}



// display
void display()
{
	// set viewport and clear it
	glViewport(0,0,windowWidth,windowHeight);

	glClearColor(0.65f,0.65f,0.65f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// report FPS
	static float frameTime = 0.0f;
	static int numFrames = 0;
	static char buffer[128];

	frameTime += FrameTime();
	++numFrames;
	if(frameTime > 1.0f) 
	{
		sprintf_s( buffer, "FPS %.1f", numFrames/frameTime );
		frameTime -= 1.0f;
		numFrames = 0;
	}
	drawText( buffer );

	// set gl states
	glEnable(GL_DEPTH_TEST);

	if( g_MultiSample && options.m_bSampleAlphaToCoverage ) 
	{
		// enable multisample and alpha to coverage
		// http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20081122
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}		
	else
	{
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}

	if( options.m_bWireFrame )
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	if( options.m_bCulling )
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	// set gl matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(10, (float)windowWidth/windowHeight, zoom/2.0f, zoom*2.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// camera transformation	
	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty,0,1,0);

	if (options.m_bAutoRotate)
		roty += ROTATION;
		
	// draw grid
	if(options.m_bGrid)
	{
		glColor3f(0.45f,0.45f,0.45f);
		glBegin(GL_LINES);
			for (int i=-10;i<=10;++i)
			{
				glVertex3i(i,0,-10);
				glVertex3i(i,0,10);

				glVertex3i(-10,0,i);
				glVertex3i(10,0,i);
			}
		glEnd();
	}

	// draw axes
	if(options.m_bAxes)
	{
		glBegin(GL_LINES);
			glColor3f(1,0,0);
			glVertex3f(1,0,0);
			glVertex3f(0,0,0);
		
			glColor3f(0,1,0);
			glVertex3f(0,1,0);
			glVertex3f(0,0,0);
		
			glColor3f(0,0,1);
			glVertex3f(0,0,1);
			glVertex3f(0,0,0);
		glEnd();
	}

	// draw models
	glTranslatef( -7.5*offset, 0, 5.0*offset );
	for ( size_t i = 0; i < modelList.size(); i++ )
	{
		XModel* model = modelList[i];
		model->gl();
		glTranslatef( 15, 0, -10 );
	}

	glutSwapBuffers();
}

// reshape func
void reshape(int x, int y)
{
	if(y==0) y = 1;
	windowWidth = x;
	windowHeight = y;

}


// anim speed scalar
float scalar=1.0f;

// idle func
void idle()
{
	SortFrameTimer();

	// update the models animation
	for ( size_t i = 0; i < modelList.size(); i++ )
	{
		XModel* model = modelList[i];
		model->Update(scalar*FrameTime());
	}

	//roty += scalar*FrameTime()*ROTATION;
	
	glutPostRedisplay();
}

// key func - just used for changing anim speed
void NormalKey(unsigned char key,int x,int y)
{
	switch( key )
	{
	case 'h':
		std::cout << "Help" << std::endl;
		std::cout << std::endl;
		std::cout << "F1:   ";
		std::cout << "Toggle AutoRotation " << std::endl;
		std::cout << "F2:   ";
		std::cout << "Toggle SampleAlphaToCoverage " << std::endl;
		std::cout << "F3:   ";
		std::cout << "Toggle DrawMeshes " << std::endl;
		std::cout << "F4:   ";
		std::cout << "Toggle DrawJoints " << std::endl;
		std::cout << "F5:   ";
		std::cout << "Toggle UpdateAnimation " << std::endl;
		std::cout << "F6:   ";
		std::cout << "Cycle animations " << std::endl;
		std::cout << "F7:   ";
		std::cout << "Toggle Wireframe " << std::endl;
		std::cout << "F8:   ";
		std::cout << "Toggle Culling " << std::endl;
		std::cout << "F9:   ";
		std::cout << "Toggle ViewAxes " << std::endl;
		std::cout << "F10:  ";
		std::cout << "Toggle ViewGrid " << std::endl;
		std::cout << "F11:  ";
		std::cout << "Toggle Texture " << std::endl;
		std::cout << "F12:  ";
		std::cout << "Toggle DrawNormals " << std::endl;
		std::cout << std::endl;
		std::cout << "+:    ";
		std::cout << "Increase animation speed twice" << std::endl;
		std::cout << "-:    ";
		std::cout << "Reduce animation speed by half" << std::endl;
		std::cout << std::endl;
		std::cout << "Esc:  ";
		std::cout << "Quit" << std::endl;
		break;
	case '-':
		scalar *= 0.5f;
		break;
	case '+':
		scalar *= 2.0f;
		break;
	case ']':
		zoom += (float) zoomStep;
		break;
	case '[':
		zoom -= (float) zoomStep;
		break;
	case 'c':
		// print camera attrib values 
		printf("zoom= %f\n",zoom); 
		printf("rotx= %f\n",rotx);
		printf("roty= %f\n",roty);
		printf("tx= %f\n",tx);
		printf("ty= %f\n",ty);
		printf("lastx= %f\n",lastx);
		printf("lasty= %f\n",lasty);
		printf("zoomStep= %f\n",zoomStep);
		printf("panStep= %f\n",panStep);
		printf("ROTATION= %f\n",ROTATION); 
		break;
	case 27:
		glutIdleFunc(0);		
		exit(0);
		break;
	}
}

// options 
void SpecialKey(int key,int x,int y)
{
	switch(key) 
	{
	case GLUT_KEY_F1:
		options.ToggleAutoRotation();
		std::cout << "AutoRotation " << options.m_bAutoRotate << std::endl;
		break;
	case GLUT_KEY_F2:
		options.ToggleSampleAlphaToCoverage();
		std::cout << "EnableSampleAlphaToCoverage " << options.m_bSampleAlphaToCoverage << std::endl;
		break;
	case GLUT_KEY_F3:
		options.ToggleMeshes();
		std::cout << "DrawMeshes " << options.m_bDrawMeshes << std::endl;
		break;
	case GLUT_KEY_F4:
		options.ToggleJoints();
		std::cout << "DrawJoints " << options.m_bDrawJoints << std::endl;
		break;
	case GLUT_KEY_F5:
		options.ToggleAnimation();
		std::cout << "UpdateAnimation " << options.m_bAnimation << std::endl;
		break;
	case GLUT_KEY_F6:
		// cycle animations
		for ( size_t i = 0; i < modelList.size(); i++ )
		{
			XModel* model = modelList[i];
			model->nextAnimation();
		}
		break;
	case GLUT_KEY_F7:
		options.ToggleWireframe();
		std::cout << "IsWireframe " << options.m_bWireFrame << std::endl;
		break;
	case GLUT_KEY_F8:
		options.ToggleCulling();
		std::cout << "IsCulled " << options.m_bCulling << std::endl;
		break;
	case GLUT_KEY_F9:
		options.ToggleAxes();
		std::cout << "ViewAxes " << options.m_bAxes << std::endl;
		break;
	case GLUT_KEY_F10:
		options.ToggleGrid();
		std::cout << "ViewGrid " << options.m_bGrid << std::endl;
		break;
	case GLUT_KEY_F11:
		options.ToggleTextured();
		std::cout << "ToggleTextured " << options.m_bTextured << std::endl;
		break;
	case GLUT_KEY_F12:
		options.ToggleNormals();
		std::cout << "DrawNormals " << options.m_bNormals << std::endl;
		break;
	default:
		break;
	}
}

#define TYPE_MODEL	 0
#define TYPE_ANIM    1
#define TYPE_CAMERA  2

struct InputFile
{
	uint			type;
	std::string		inFileName;
};

int main(int argc,char** argv)
{
	// parse input
	std::string sceneFileName;

	if( argc == 2 )
	{
		sceneFileName = argv[1];
	}
	else
	{
		printf("Usage: Loader SCENE_FILE_NAME\n   ");
		assert(0);
		exit(0);
	}

	// check file extension
	std::string key (".scene");
	size_t found = sceneFileName.rfind(key);
	if ( found == std::string::npos )
	{
		std::cerr << "Error! Config file name has to have .scene as extension" << std::endl;
		assert(0); exit(1);
	}

	// open scene file
	std::ifstream ifs( sceneFileName.c_str() );
	if (!ifs)
	{
		std::cerr << "Error! Could not open scene file" << std::endl;
		assert(0); exit(1);
	}

	// read scene file
	char buffer[256];
	std::vector<InputFile> fileList;
	while( !ifs.eof() )
	{
		ifs >> buffer;
		if ( strlen( buffer ) > 0 )
		{
			//std::cout << buffer << std::endl;
			InputFile file;
			if ( strcmp( buffer, "model" ) == 0 )
				file.type = TYPE_MODEL;
			else if ( strcmp( buffer, "anim" ) == 0 )
				file.type = TYPE_ANIM;
			else if ( strcmp( buffer, "camera" ) == 0 )
				file.type = TYPE_CAMERA;
			else
			{
				std::cerr << "Error! Unrecognized file type " << buffer << " in config file." << std::endl;
				assert(0); exit(1);
			}

			ifs >> file.inFileName;
			fileList.push_back( file );
		}
		buffer[0] = '\0';
	}
	ifs.close();

	// init glut
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH | GLUT_MULTISAMPLE);
	
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(300, 80);

	glutCreateWindow( sceneFileName.c_str() );

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(idle);
	glutSpecialFunc(SpecialKey);
	glutKeyboardFunc(NormalKey);

	InitFrameTimer();
	InitGL();

	// Load the scene
	bool status = true;
	XModel* model = NULL;
	for ( size_t i = 0; i < fileList.size(); i++ )
	{
		InputFile file = fileList[i];
		if ( file.type == TYPE_MODEL )
		{
			model = new XModel;
			status &= model->LoadModel( file.inFileName.c_str() );
			modelList.push_back( model );
		}
		else if ( file.type == TYPE_ANIM )
		{
			status &= model->LoadAnim( file.inFileName.c_str() );
		}
		else // camera
		{
			InitCamera( file.inFileName );
		}
	}		

	// init models
	if ( status )
	{
		for ( size_t i = 0; i < modelList.size(); i++ )
		{
			XModel* model = modelList[i];
			model->init();
			offset += 1.0f;
		}
		//int zSize = glutGet(GLUT_WINDOW_DEPTH_SIZE);
		glutMainLoop();
	}

	return 1;
}

























