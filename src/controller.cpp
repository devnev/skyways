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

#include <GL/gl.h>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "textprinter.hpp"
#include "controller.hpp"

Controller::Controller(
	  double acceleration, double strafespeed, double speedlimit
	, double gravity, double jumpstrength
	, double cameraheight, double cameradistance, double camerarotation
	, Controller::QuitCallback cbQuit
	, std::auto_ptr< TextPrinter > printer
)
	: _ship(), _world( 10 )
	, vx( 0 ), az( 0 )
	, _zacc( acceleration ), _xspeed( strafespeed )
	, _maxSpeed( speedlimit ), _zspeed( 0 )
	, _yapex( 0 ), _tapex( 0 ), _gravity( gravity )
	, _jstrength( jumpstrength ), _grounded( true )
	, _camy( cameraheight ), _camz( cameradistance ), _camrot( camerarotation )
	, _dead( false ), _quitcb( cbQuit )
	, _printer( printer )
	, _windowwidth( 1 ), _windowheight( 1 )
{
	_ship.pos().x = 0.5;
}

Controller::~Controller()
{
}

void Controller::keydown( int key )
{
	switch ( key )
	{
	case STRAFE_L_KEY: vx = -1; break;
	case STRAFE_R_KEY: vx = 1; break;
	case ACCEL_KEY: az = 1; break;
	case DECEL_KEY: az = -1; break;
	case JUMP_KEY:
		if ( _grounded ||
			_world.collide( AABB(
				_ship.pos().offset(0, -0.2, 0),
				_ship.pos().offset(0, -0.2, 0).offset(_ship.size())
			) ) )
		{
			_yapex = _ship.ypos() + _jstrength;
			_tapex = sqrt( _jstrength / _gravity );
		}
		break;
	case QUIT_KEY:
		if ( _dead )
		{
			_quitcb();
		}
		else
		{
			std::cout <<
				"You committed suicide!\n"
				"Distance traveled: " << _ship.pos().z << std::endl;
			_dead = true;
		}
		break;
	}
}

void Controller::keyup( int key )
{
	switch ( key )
	{
	case STRAFE_L_KEY:
		if ( vx < 0 )
			vx = 0;
		break;
	case STRAFE_R_KEY:
		if ( vx > 0 )
			vx = 0;
		break;
	case ACCEL_KEY:
		if ( az > 0 )
			az = 0;
		break;
	case DECEL_KEY:
		if ( az < 0 )
			az = 0;
		break;
	}
}

void Controller::loadWorld( std::string filename )
{
	_world.loadBlocks();
	std::ifstream worldFile( filename.c_str() );
	_world.loadWorld( worldFile );
}

void Controller::generateWorld()
{
	_world.loadBlocks();
	srand(time(0));
	_world.generateWorld();
}

void Controller::initialize()
{
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

	_world.optimize();
	_ship.initialize();
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
	glTranslated( _ship.xpos() - 0.5, _ship.ypos() - _camy, -_camz );
	glColor4f( 1, 0, 0, 0.25 );
	_ship.drawDl();
	glLoadIdentity();
	glColor3f( 0.8f, 1, 1 );
	glRotatef( _camrot, 1, 0, 0 );
	glTranslatef( -0.5, -_camy, _ship.zpos() - _camz );
	_world.glDraw( _ship.zpos() -_camz );
	if ( _dead )
	{
		_printer->print(
			( boost::format( "Distance Traveled: %1%" ) % _ship.pos().z ).str(),
			_windowwidth / 2, _windowheight / 4 * 3,
			TextPrinter::ALIGN_CENTER
		);
	}
}

void Controller::update( int difference )
{
	if ( _dead )
		return;

	double multiplier = ( (double)difference ) / 1000;

	Vector3 newPos = _ship.pos();

	AABB shipAabb(
		Vector3( -_ship.size().x/2, 0, 0 ),
		Vector3( _ship.size().x/2, _ship.size().y, _ship.size().z )
	);

	if ( az < 0 )
	{
		_zspeed -= _zacc*multiplier;
		if ( _zspeed < 0 )
			_zspeed = 0;
	}
	else if ( az > 0 )
	{
		_zspeed += _zacc*multiplier;
		if ( _zspeed > _maxSpeed )
			_zspeed = _maxSpeed;
	}

	newPos.z += multiplier * _zspeed;
	if ( _world.collide( shipAabb.offset( newPos ) ) )
	{
		newPos.z = _ship.pos().z;
		_zspeed = 0;
	}
	else
		_ship.pos().z = newPos.z;

	if ( vx < 0 )
		newPos.x -= _xspeed*multiplier;
	if ( vx > 0 )
		newPos.x += _xspeed*multiplier;
	if ( vx != 0 )
	{
		if ( _world.collide( shipAabb.offset( newPos ) ) )
			newPos.x = _ship.pos().x;
		else
			_ship.pos().x = newPos.x;
	}

	if ( _tapex <= 0 && _grounded )
	{
		_tapex = 0;
		_yapex = _ship.pos().y;
	}

	_tapex -= multiplier;
	newPos.y = _yapex - _gravity * ( _tapex*_tapex );
	if ( _world.collide( shipAabb.offset( newPos ) ) )
	{
		newPos.y = _ship.pos().y;
		if (_tapex < 0)
			_grounded = true;
		_tapex = 0;
		_yapex = _ship.pos().y;
	}
	else
	{
		_ship.pos().y = newPos.y;
		_grounded = false;
	}

	if ( _ship.pos().y < _world.lowestPoint() - 1 )
	{
		std::cout <<
			"You dropped into the void!\n"
			"Distance traveled: " << _ship.pos().z << std::endl;
		_dead = true;
	}

}
