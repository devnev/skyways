#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include "sdl/sdl.hpp"
#include "ship.hpp"

class Controller
{
public:
	Controller( Ship * ship )
		: _ship( ship ), vx( 0 ), az( 0 )
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
		if (az < 0)
			_ship->decreaseSpeed(0.2);
		else if (az > 0)
			_ship->increaseSpeed(0.2);
	}

private:
	int vx, az;
	Ship * _ship;
};

#endif // _CONTROLLER_HPP_
