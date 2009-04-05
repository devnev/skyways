#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "ship.hpp"
#include <QCoreApplication>

class Controller
{
public:
	Controller( Ship * ship, double acceleration, double strafespeed )
		: _ship( ship ), vx( 0 ), az( 0 )
		, _zacc( acceleration ), _xspeed( strafespeed )
	{
	}

	void keydown( int key )
	{
		switch ( key )
		{
		case Qt::Key_Escape:
			QCoreApplication::instance()->quit();
			break;
		case Qt::Key_Left: vx = -1; break;
		case Qt::Key_Right: vx = 1; break;
		case Qt::Key_Up: az = 1; break;
		case Qt::Key_Down: az = -1; break;
		}
	}

	void keyup( int key )
	{
		switch ( key )
		{
		case Qt::Key_Left: 
			if (vx < 0)
				vx = 0;
			break;
		case Qt::Key_Right:
			if (vx > 0)
				vx = 0;
			break;
		case Qt::Key_Up:
			if (az > 0)
				az = 0;
			break;
		case Qt::Key_Down:
			if (az < 0)
				az = 0;
			break;
		}
	}

	void update(int difference)
	{
		double multiplier = ((double)( difference )) / 1000;

		if (az < 0)
			_ship->decreaseSpeed( _zacc*multiplier );
		else if (az > 0)
			_ship->increaseSpeed( _zacc*multiplier );
		if (vx < 0)
			_ship->moveLeft( _xspeed*multiplier );
		if (vx > 0)
			_ship->moveRight( _xspeed*multiplier );

		_ship->update(multiplier);
	}

private:
	Ship * _ship;
	int vx, az;
	double _zacc, _xspeed;
};

#endif // _CONTROLLER_HPP_
