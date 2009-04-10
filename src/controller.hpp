#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "ship.hpp"
#include "world.hpp"

class Controller
{

public:

	typedef void (*QuitCallback)();

	Controller( QuitCallback cbQuit );

	enum {
		JUMP_KEY = 1,
		STRAFE_L_KEY = 2,
		STRAFE_R_KEY = 3,
		ACCEL_KEY = 4,
		DECEL_KEY = 5,
	};

	void keydown( int key );
	void keyup( int key );
	void loadWorld( std::string filename );
	void generateWorld();
	void initialize();
	void resize( int width, int height );
	void draw();
	void update( int difference );

private:

	Ship _ship;
	World _world;
	int vx, az;
	double _zacc, _xspeed;
	double _maxSpeed, _zspeed;
	double _yapex, _tapex, _gravity;
	double _jstrength;
	bool _grounded;
	double _camy, _camz;
	size_t _deadTime;
	QuitCallback _quitcb;

};

#endif // _CONTROLLER_HPP_
