#include <QCoreApplication>
#include "controller.hpp"

Controller::Controller( Ship * ship, double acceleration, double strafespeed )
	: _ship( ship ), vx( 0 ), az( 0 )
	, _zacc( acceleration ), _xspeed( strafespeed )
{
}

void Controller::keydown( int key )
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

void Controller::keyup( int key )
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
	case Qt::Key_Space:
		if (_ship->ypos() < 0.2)
			_ship->jump(1.5);
		break;
	}
}

void Controller::update(int difference)
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
