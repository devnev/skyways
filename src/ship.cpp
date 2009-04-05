#include "ship.hpp"

#include <QtOpenGL>
#include <cmath>

Ship::Ship(double maxSpeed)
	: _maxSpeed( maxSpeed ), _speed( 0 ), _xpos( 0 ), _ypos( 0 )
	, _zpos( 0 ), _yapex( 0 ), _tapex( 0 ), _fallrate( 1.5 )
{
}

void Ship::decreaseSpeed(double amount)
{
	_speed -= amount;
	if (_speed < 0)
		_speed = 0;
}

void Ship::increaseSpeed(double amount)
{
	_speed += amount;
	if (_speed > _maxSpeed)
		_speed = _maxSpeed;
}

void Ship::moveLeft(double amount)
{
	_xpos -= amount;
}

void Ship::moveRight(double amount)
{
	_xpos += amount;
}

void Ship::jump(double strength)
{
	_yapex = _ypos + strength;
	_tapex = 1.0;
}

void Ship::update(double multiplier)
{
	_zpos += multiplier * _speed;
	if ( _yapex > 0 )
	{
		_tapex -= multiplier;
		_ypos = _yapex - _fallrate * ( _tapex*_tapex );
	}
	if ( _ypos < 0 )
	{
		_ypos = 0;
		_yapex = 0;
	}
}

void Ship::glDraw()
{
	glBegin( GL_QUAD_STRIP );
		glVertex3d( 0, 1, 0 );
		glVertex3d( 0, 0, 0 );
		glVertex3d( 1, 1, 0 );
		glVertex3d( 1, 0, 0 );
		glVertex3d( 1, 1, -1 );
		glVertex3d( 1, 0, -1 );
		glVertex3d( 0, 1, -1 );
		glVertex3d( 0, 0, -1 );
		glVertex3d( 0, 1, 0 );
		glVertex3d( 0, 0, 0 );
	glEnd();
	glBegin( GL_QUADS );
		glVertex3d( 0, 0, 0 );
		glVertex3d( 1, 0, 0 );
		glVertex3d( 1, 0, -1 );
		glVertex3d( 0, 0, -1 );

		glVertex3d( 0, 1, 0 );
		glVertex3d( 1, 1, 0 );
		glVertex3d( 1, 1, -1 );
		glVertex3d( 0, 1, -1 );
	glEnd();
}
