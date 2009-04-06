#include <GL/gl.h>
#include <iostream>
#include "controller.hpp"

Controller::Controller()
	: _ship( 100 ), _world( 10 )
	, vx( 0 ), az( 0 )
	, _zacc( 8 ), _xspeed( 3 )
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
		if ( _ship.ypos() < 0.2 )
			_ship.jump( 1.5 );
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
			_world.block( "" )
		));
	}
	_world.optimize();

	std::cout << "Loaded " << _world.blocksLoaded() << " blocks.\n";
}

void Controller::resize( int width, int height )
{
     glViewport( 0, 0, width, height );

     glMatrixMode( GL_PROJECTION );
     glLoadIdentity();
	 double fH = tan( 30.0 / 180.0 * 3.14159265358979323846 );
	 double fW = ( (double)width ) / ( (double)height ) * fH;
     glFrustum( -fW, fW, -fH, fH, 1.0, 100.0 );
     glMatrixMode( GL_MODELVIEW );
}

void Controller::draw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslated( _ship.xpos(), _ship.ypos() - 2, -4 );
	glColor3f( 1, 0, 0 );
	_ship.glDraw();
	glLoadIdentity();
	glColor3f( 0.8f, 1, 1 );
	glTranslatef( 0, -2, _ship.zpos() - 4 );
	_world.glDraw( _ship.zpos() - 4 );
	std::cout << _ship.xpos() << ", " << _ship.zpos() << ", " << _ship.speed() << ", " << _world.elementsDrawn() << std::endl;
}

void Controller::update( int difference )
{
	double multiplier = ( (double)difference ) / 1000;

	if ( az < 0 )
		_ship.decreaseSpeed( _zacc*multiplier );
	else if ( az > 0 )
		_ship.increaseSpeed( _zacc*multiplier );
	if ( vx < 0 )
		_ship.moveLeft( _xspeed*multiplier );
	if ( vx > 0 )
		_ship.moveRight( _xspeed*multiplier );

	_ship.update( multiplier );
}
