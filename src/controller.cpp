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

#include <GL/glew.h>
#include <GL/gl.h>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "textprinter.hpp"
#include "shader.hpp"
#include "controller.hpp"

Controller::Controller(
	  std::auto_ptr< Game > game
	, double cameraheight, double cameradistance, double camerarotation
	, Controller::QuitCallback cbQuit
	, std::auto_ptr< TextPrinter > printer
)
	: _game( game ), _map( 10 )
	, _camy( cameraheight ), _camz( cameradistance ), _camrot( camerarotation )
	, _quitcb( cbQuit ), _printer( printer )
	, _windowwidth( 1 ), _windowheight( 1 )
{
	_game->setMap( _map );
}

Controller::~Controller()
{
}

void Controller::keydown( int key )
{
	switch ( key )
	{
	case STRAFE_L_KEY: _game->setStrafe( -1 ); break;
	case STRAFE_R_KEY: _game->setStrafe( 1 ); break;
	case ACCEL_KEY: _game->setAcceleration( 1 ); break;
	case DECEL_KEY: _game->setAcceleration( -1 ); break;
	case JUMP_KEY: _game->startJump(); break;
	case QUIT_KEY:
		if ( _game->dead() )
		{
			_quitcb();
		}
		else
		{
			_game->kill( "You committed suicide!" );
		}
		break;
	}
}

void Controller::keyup( int key )
{
	switch ( key )
	{
	case STRAFE_L_KEY:
		_game->setStrafe( 0 );
		break;
	case STRAFE_R_KEY:
		_game->setStrafe( 0 );
		break;
	case ACCEL_KEY:
		_game->setAcceleration( 0 );
		break;
	case DECEL_KEY:
		_game->setAcceleration( 0 );
		break;
	}
}

void Controller::loadMap( std::string filename )
{
	_map.loadBlocks();
	std::ifstream mapFile( filename.c_str() );
	_map.loadMap( mapFile );
}

void Controller::generateMap()
{
	_map.loadBlocks();
	srand(time(0));
	_map.generateMap();
}

void Controller::initialize()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::runtime_error((const char*)glewGetErrorString(err));
	if (!GLEW_VERSION_2_0)
		throw std::runtime_error("Nead OpenGL >= 2.0 for shaders. Update your graphics drivers!");

	_shaderProgram = createShaderProgram("shaders/shader.glslv", "shaders/shader.glslf");
	_game->setShader( *_shaderProgram );

	glClearColor( 0.2, 0.2, 0.2, 0 );
	glClearDepth( 1.0 );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_CULL_FACE );

	glEnable( GL_LIGHTING );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_NORMALIZE );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_LINE_SMOOTH );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float position[] = { 0.0f, 4.0f, -2.0f, 1.0f };
	glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT1, GL_POSITION, position );
	glEnable( GL_LIGHT1 );

	_map.optimize();
}

void Controller::resize( int width, int height )
{
	glViewport( 0, 0, width, height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	double fH = tan( 30.0 / 180.0 * 3.14159265358979323846 );
	double fW = ( (double)width ) / ( (double)height ) * fH;
	glFrustum( -fW, fW, -fH, fH, 1.0, 1000.0 );
	glMatrixMode( GL_MODELVIEW );

	_windowwidth = width;
	_windowheight = height;
}

void Controller::draw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glRotatef( _camrot, 1, 0, 0 );
	glTranslated( 0.0, -_camy, -_camz );
	_game->draw( _camz );
	if ( _game->dead() )
	{
		glUseProgram(0);
		_printer->print(
			( boost::format( "%1% Distance Traveled: %2%" )
			  % _game->deathCause()
			  % _game->distanceTraveled() ).str(),
			_windowwidth / 2, _windowheight / 4 * 3,
			TextPrinter::ALIGN_CENTER
		);
	}
}

void Controller::update( int difference )
{
	if ( _game->dead() )
		return;

	_game->update( difference );
}
