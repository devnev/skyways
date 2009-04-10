#include <GL/gl.h>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "controller.hpp"

Controller::Controller( Controller::QuitCallback cbQuit )
	: _ship(), _world( 10 )
	, vx( 0 ), az( 0 )
	, _zacc( 10 ), _xspeed( 5 )
	, _maxSpeed( 100 ), _zspeed( 0 )
	, _yapex( 0 ), _tapex( 0 ), _gravity( 20 )
	, _jstrength( 1.5 ), _grounded( true )
	, _camy( 3 ), _camz( 6 )
	, _dead( false ), _quitcb( cbQuit )
	, _printer( "DejaVuSans.ttf" )
	, _windowwidth( 1 ), _windowheight( 1 )
{
	_ship.pos().x = 0.5;
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
	glClearColor( 0, 0, 0, 0 );
	glClearDepth( 1.0 );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_CULL_FACE );

	glEnable( GL_LIGHTING );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_COLOR_MATERIAL );

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
	glTranslated( _ship.xpos() - 0.5, _ship.ypos() - _camy, -_camz );
	glColor3f( 1, 0, 0 );
	_ship.drawDl();
	glLoadIdentity();
	glColor3f( 0.8f, 1, 1 );
	glTranslatef( -0.5, -_camy, _ship.zpos() - _camz );
	_world.glDraw( _ship.zpos() -_camz );
	if ( _dead )
	{
		_printer.print(
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
