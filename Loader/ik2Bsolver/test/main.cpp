#include <ik2Bsolver.h>

#include <iostream>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>

static const int KEY_SPACE = ' ';
static const int KEY_ESCAPE = '\033';

AwPoint startJointPos, midJointPos, effectorPos, handlePos;
AwVector poleVector;
double twistValue;
AwQuaternion qStart;
AwQuaternion qMid;
AwMatrix mStart, mMid;

int currentButton = 0;

template <class T>
inline T
Min( const T a, const T b )
{
	return a <= b ? a : b; // use <= to maintain order
}

void
Draw( void )
{
	static bool firstTime = true;
	if ( firstTime )
	{
		// init bones
		startJointPos.set(.0, .0);
		midJointPos.set(.2, .4);
		effectorPos.set(.5, .5);
		handlePos.set(.5, .5);
		twistValue = .0;
		poleVector = AwVector(-1,1).normalize();
		firstTime = false;
	}
			
	static GLfloat polygonColor1[] = { .25f, .8f, .8f };
	static GLfloat polygonColor2[] = { .8f, .25f, .8f };

	// clear out buffer
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	glClear( GL_COLOR_BUFFER_BIT );

	// draw primitives
	glPushMatrix();
		glTranslated( startJointPos.x, startJointPos.y, 0 );
		glMultMatrixd( &mStart.matrix[0][0] );
		glBegin( GL_LINES );
		glColor3fv( polygonColor1 );
		glVertex2f(0,0);
		glVertex2f(midJointPos.x-startJointPos.x, midJointPos.y-startJointPos.y);
		glEnd();

		glTranslated( midJointPos.x, midJointPos.y, 0 );
		glMultMatrixd( &mMid.matrix[0][0] );
		glBegin( GL_LINES );
		glColor3fv( polygonColor2 );
		glVertex2f(0,0);
		glVertex2f(effectorPos.x-midJointPos.x, effectorPos.y-midJointPos.y);
		glEnd();
	glPopMatrix();

	// show result
	glutSwapBuffers();
}

void
Reshape( int width, int height )
{
	// the user just changed the window size
	// Adjust projection parameters
	glViewport( 0, 0, width, height );

	// correct projection to avoid distortion
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// make it normalized
	if( width > height ){
		GLdouble aspect = GLdouble( width ) / height;
		gluOrtho2D( -aspect, aspect, -1., 1. );
	}else{
		GLdouble aspect = GLdouble( height ) / width;
		gluOrtho2D( -1., 1., -aspect, aspect );
	}

	// and back to ModelView
	glMatrixMode( GL_MODELVIEW );
}

void
Keyboard( unsigned char c, int x, int y )
{
	switch( c ){
	case KEY_SPACE:
		break;
	case KEY_ESCAPE:
		exit( 0 );
		break;
	default:
		;
	}
}

void 
Motion( int x, int y )
{
	if ( currentButton == GLUT_LEFT_BUTTON )
	{
		handlePos.set( (x - 500)/ 500.0, -(y - 500)/ 500.0 );
	}
	else if ( currentButton == GLUT_RIGHT_BUTTON )
	{
		startJointPos.set( (x - 500)/ 500.0, -(y - 500)/ 500.0 );
	}

	solveIK( startJointPos, midJointPos, effectorPos, handlePos, poleVector, twistValue, qStart, qMid );

	qStart.convertToMatrix( mStart );
	qMid.convertToMatrix( mMid );

	glutPostRedisplay();
}

// button click callback
void
Mouse( int button, int state, int x, int y )
{
	switch( button )
	{

	case GLUT_LEFT_BUTTON:
		// animation control
		if( state == GLUT_DOWN )
		{
			currentButton = button;
		}
		else if( state == GLUT_UP )
		{
			currentButton = 0;
		}
		else
		{
			assert( 0 );
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		break;

	case GLUT_RIGHT_BUTTON:
		if( state == GLUT_DOWN )
		{
			currentButton = button;
		}
		else if( state == GLUT_UP )
		{
			currentButton = 0;
		}
		else
		{
			assert( 0 );
		}
		break;

	default:
		assert( 0 );

	}
}


void
Init( void )
{
	glutDisplayFunc( Draw );
	glutReshapeFunc( Reshape );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( Mouse );
	glutMotionFunc( Motion );
	glutPassiveMotionFunc( 0 );
	glutMenuStatusFunc( 0 );
	glutEntryFunc( 0 );

	// setup orthographic projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	// make it normalized
	gluOrtho2D( -1., 1., -1., 1. );

	// back to the model matrix
	glMatrixMode( GL_MODELVIEW );
}

int
main( int argc, char *argv[] )
{
	// let glut eat any command line args that it owns
	glutInit( &argc, argv );

	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(300, 80);

	// full color, double buffered
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutCreateWindow( "IK Test" );

	// setup remaining state
	Init();

	// start processing events
	glutMainLoop();

	return 0;
}
