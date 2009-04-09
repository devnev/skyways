#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#if defined(FREEGLUT)
#  include <GL/freeglut_ext.h>
#elif defined(OPENGLUT)
#  include <GL/openglut_ext.h>
#else
#  error "Unknown GLUT implementation. Need glutLeaveMainLoop extension."
#endif

#include "controller.hpp"

Controller controller;

static void resize( int width, int height )
{
	controller.resize(width, height);

	glutPostRedisplay();
}

static void display()
{
	controller.draw();

	glutSwapBuffers();
}

static void keyDown( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case ' ':
		return controller.keydown( Controller::JUMP_KEY );
	}

	glutPostRedisplay();
}

static void specialKeyDown( int key, int x, int y )
{
	switch ( key )
	{
	case GLUT_KEY_LEFT:
		return controller.keydown( Controller::STRAFE_L_KEY );
	case GLUT_KEY_RIGHT:
		return controller.keydown( Controller::STRAFE_R_KEY );
	case GLUT_KEY_UP:
		return controller.keydown( Controller::ACCEL_KEY );
	case GLUT_KEY_DOWN:
		return controller.keydown( Controller::DECEL_KEY );
	case 27:
		return glutLeaveMainLoop();
	}

	glutPostRedisplay();
}

static void keyUp( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case ' ':
		return controller.keyup( Controller::JUMP_KEY );
	}

	glutPostRedisplay();
}

static void specialKeyUp( int key, int x, int y )
{
	switch ( key )
	{
	case GLUT_KEY_LEFT:
		return controller.keyup( Controller::STRAFE_L_KEY );
	case GLUT_KEY_RIGHT:
		return controller.keyup( Controller::STRAFE_R_KEY );
	case GLUT_KEY_UP:
		return controller.keyup( Controller::ACCEL_KEY );
	case GLUT_KEY_DOWN:
		return controller.keyup( Controller::DECEL_KEY );
	}

	glutPostRedisplay();
}

static void idle()
{
	static int tick = glutGet( GLUT_ELAPSED_TIME );
	int newTick = glutGet( GLUT_ELAPSED_TIME );
	controller.update( newTick - tick );
	tick = newTick;

	glutPostRedisplay();
}

int main( int argc, char * argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "skyways" );

	glutReshapeFunc( resize );
	glutDisplayFunc( display );
	glutKeyboardFunc( keyDown );
	glutKeyboardUpFunc( keyUp );
	glutSpecialFunc( specialKeyDown );
	glutSpecialUpFunc( specialKeyUp );
	glutIdleFunc( idle );

	controller.initialize();

	glutMainLoop();

	return 0;
}
