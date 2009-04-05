#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "sdl/sdl.hpp"
#include "ship.hpp"

class Controller
{
public:
	Controller( Ship * ship, double acceleration, double strafespeed )
		: _ship( ship ), vx( 0 ), az( 0 )
		, _zacc( acceleration ), _xspeed( strafespeed )
	{
	}

	void keydown( SDL_KeyboardEvent * event )
	{
		switch (event->keysym.sym)
		{
		case SDLK_ESCAPE:
			{
				SDL_Event qev = { SDL_QUIT };
				SDL_PushEvent(&qev);
				break;
			}
		case SDLK_LEFT: vx = -1; break;
		case SDLK_RIGHT: vx = 1; break;
		case SDLK_UP: az = 1; break;
		case SDLK_DOWN: az = -1; break;
		}
	}

	void keyup( SDL_KeyboardEvent * event )
	{
		switch (event->keysym.sym)
		{
		case SDLK_LEFT: 
			if (vx < 0)
				vx = 0;
			break;
		case SDLK_RIGHT:
			if (vx > 0)
				vx = 0;
			break;
		case SDLK_UP:
			if (az > 0)
				az = 0;
			break;
		case SDLK_DOWN:
			if (az < 0)
				az = 0;
			break;
		}
	}

	void update()
	{
		Uint32 newTick = SDL_GetTicks();
		double multiplier = ((double)( newTick - updateTick )) / 1000;
		updateTick = newTick;

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
	int vx, az;
	Ship * _ship;
	Uint32 updateTick;
	double _zacc, _xspeed;
};

#endif // _CONTROLLER_HPP_
