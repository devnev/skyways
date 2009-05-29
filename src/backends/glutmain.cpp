//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

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

#include <controller.hpp>
#include "configparser.hpp"
#include <configuration.hpp>

std::auto_ptr< Controller > controller;

static void resize( int width, int height )
{
	controller->resize(width, height);

	glutPostRedisplay();
}

static void display()
{
	controller->draw();

	glutSwapBuffers();
}

static void keyDown( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case ' ':
		return controller->keydown( Controller::JUMP_KEY );
	case 27: // ESCAPE
		return controller->keydown( Controller::QUIT_KEY );
	}

	glutPostRedisplay();
}

static void specialKeyDown( int key, int x, int y )
{
	switch ( key )
	{
	case GLUT_KEY_LEFT:
		return controller->keydown( Controller::STRAFE_L_KEY );
	case GLUT_KEY_RIGHT:
		return controller->keydown( Controller::STRAFE_R_KEY );
	case GLUT_KEY_UP:
		return controller->keydown( Controller::ACCEL_KEY );
	case GLUT_KEY_DOWN:
		return controller->keydown( Controller::DECEL_KEY );
	}

	glutPostRedisplay();
}

static void keyUp( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case ' ':
		return controller->keyup( Controller::JUMP_KEY );
	}

	glutPostRedisplay();
}

static void specialKeyUp( int key, int x, int y )
{
	switch ( key )
	{
	case GLUT_KEY_LEFT:
		return controller->keyup( Controller::STRAFE_L_KEY );
	case GLUT_KEY_RIGHT:
		return controller->keyup( Controller::STRAFE_R_KEY );
	case GLUT_KEY_UP:
		return controller->keyup( Controller::ACCEL_KEY );
	case GLUT_KEY_DOWN:
		return controller->keyup( Controller::DECEL_KEY );
	}

	glutPostRedisplay();
}

static void idle()
{
	static int tick = glutGet( GLUT_ELAPSED_TIME );
	int newTick = glutGet( GLUT_ELAPSED_TIME );
	controller->update( newTick - tick );
	tick = newTick;

	glutPostRedisplay();
}

int main( int argc, char * argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "Skyways" );

	Configuration config;
	ConfigParser configParser( config );
	if ( !configParser.args( argc, argv ) )
		return 0;
	controller = config.buildController( &glutLeaveMainLoop );

	glutReshapeFunc( resize );
	glutDisplayFunc( display );
	glutKeyboardFunc( keyDown );
	glutKeyboardUpFunc( keyUp );
	glutSpecialFunc( specialKeyDown );
	glutSpecialUpFunc( specialKeyUp );
	glutIdleFunc( idle );

	controller->initialize();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();

	return 0;
}
