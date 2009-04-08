#include <GL/gl.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "controller.hpp"

Controller::Controller()
	: _ship(), _world( 10 )
	, vx( 0 ), az( 0 )
	, _zacc( 8 ), _xspeed( 3 )
	, _maxSpeed( 100 ), _zspeed( 0 )
	, _yapex( 0 ), _tapex( 0 ), _gravity( 10 )
	, _grounded( true )
	, _camy( 3 ), _camz( 6 )
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
			_yapex = _ship.ypos() + 1.5;
			_tapex = sqrt( 1.5 / _gravity );
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

void Controller::initialize()
{
	srand(time(0));
	glClearColor( 0, 0, 0, 0 );
	glClearDepth( 1.0 );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glEnable( GL_LIGHTING );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_COLOR_MATERIAL );

	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float position[] = { 0.0f, 4.0f, -2.0f, 1.0f };
	glLightfv( GL_LIGHT1, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT1, GL_POSITION, position );
	glEnable( GL_LIGHT1 );

	_world.loadBlocks();

	for (size_t i = 0; i < 100; ++i)
	{
		_world.add(Element(
			rand() % 8 - 4,
			((double)(rand() % 5))/4 - 0.5,
			rand() % 400,
			rand() % 20 + 2,
			_world.block( "" ),
			Point::make(
				( (double)rand() ) / RAND_MAX,
				( (double)rand() ) / RAND_MAX,
				( (double)rand() ) / RAND_MAX
			)
		));
	}
	_world.optimize();
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
}

void Controller::draw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslated( _ship.xpos(), _ship.ypos() - _camy, -_camz );
	glColor3f( 1, 0, 0 );
	_ship.glDraw();
	glLoadIdentity();
	glColor3f( 0.8f, 1, 1 );
	glTranslatef( 0, -_camy, _ship.zpos() - _camz );
	_world.glDraw( _ship.zpos() -_camz );
}

void Controller::update( int difference )
{
	double multiplier = ( (double)difference ) / 1000;

	Point newPos = _ship.pos();

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
	if ( _world.collide( AABB( newPos, newPos.offset(_ship.size()) ) ) )
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
		if ( _world.collide( AABB( newPos, newPos.offset(_ship.size()) ) ) )
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
	if ( _world.collide( AABB( newPos, newPos.offset(_ship.size()) ) ) )
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

	if ( _ship.pos().y < 0 )
	{
		_ship.pos().y = 0;
		_yapex = 0;
		_tapex = 0;
		_grounded = true;
	}

}
