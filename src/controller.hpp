#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "ship.hpp"

class Controller
{
public:
	Controller( Ship * ship, double acceleration, double strafespeed );

	void keydown( int key );
	void keyup( int key );
	void update( int difference );

private:
	Ship * _ship;
	int vx, az;
	double _zacc, _xspeed;
};

#endif // _CONTROLLER_HPP_
