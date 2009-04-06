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
	case STRAFE_L_KEY: vx = -1; break;
	case STRAFE_R_KEY: vx = 1; break;
	case ACCEL_KEY: az = 1; break;
	case DECEL_KEY: az = -1; break;
	case JUMP_KEY:
		if ( _ship->ypos() < 0.2 )
			_ship->jump( 1.5 );
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

void Controller::update( int difference )
{
	double multiplier = ( (double)difference ) / 1000;

	if ( az < 0 )
		_ship->decreaseSpeed( _zacc*multiplier );
	else if ( az > 0 )
		_ship->increaseSpeed( _zacc*multiplier );
	if ( vx < 0 )
		_ship->moveLeft( _xspeed*multiplier );
	if ( vx > 0 )
		_ship->moveRight( _xspeed*multiplier );

	_ship->update( multiplier );
}
