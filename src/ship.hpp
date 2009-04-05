#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include <QtOpenGL>

class Ship
{
public:

	Ship(double maxSpeed)
		: _maxSpeed( maxSpeed ), _speed( 0 ), _xpos( 0 ), _zpos( 0 )
	{
	}

	void decreaseSpeed(double amount)
	{
		_speed -= amount;
		if (_speed < 0)
			_speed = 0;
	}
	void increaseSpeed(double amount)
	{
		_speed += amount;
		if (_speed > _maxSpeed)
			_speed = _maxSpeed;
	}
	double speed() const throw() { return _speed; }

	void moveLeft(double amount)
	{
		_xpos -= amount;
	}
	void moveRight(double amount)
	{
		_xpos += amount;
	}
	double xpos() const throw() { return _xpos; }
	double zpos() const throw() { return _zpos; }

	void update(double multiplier)
	{
		_zpos += multiplier * _speed;
	}

	void glDraw()
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

private:

	double _maxSpeed;
	double _speed;
	double _xpos, _zpos;

};

#endif // _SHIP_HPP_
