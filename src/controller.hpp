#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "ship.hpp"

class Controller
{
public:
	Controller( Ship * ship, double acceleration, double strafespeed );

	enum {
		JUMP_KEY = 1,
		STRAFE_L_KEY = 2,
		STRAFE_R_KEY = 3,
		ACCEL_KEY = 4,
		DECEL_KEY = 5,
	};

	void keydown( int key );
	void keyup( int key );
	void update( int difference );

private:
	Ship * _ship;
	int vx, az;
	double _zacc, _xspeed;
};

#endif // _CONTROLLER_HPP_
