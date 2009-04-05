#include "ship.hpp"

#include <QtOpenGL>

Ship::Ship(double maxSpeed)
	: _maxSpeed( maxSpeed ), _speed( 0 ), _xpos( 0 ), _zpos( 0 )
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

void Ship::update(double multiplier)
{
	_zpos += multiplier * _speed;
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
