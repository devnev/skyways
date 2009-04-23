//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

#include <memory>
#include "ship.hpp"
#include "world.hpp"

class TextPrinter;

class Controller
{

public:

	typedef void (*QuitCallback)();

	Controller(
	    double acceleration, double strafespeed, double speedlimit
	  , double gravity, double jumpstrengt
	  , double cameraheight, double cameradistance, double camerarotation
	  , QuitCallback cbQuit
	  , std::auto_ptr< TextPrinter > printer
	);

	~Controller();

	enum {
		JUMP_KEY = 1,
		STRAFE_L_KEY = 2,
		STRAFE_R_KEY = 3,
		ACCEL_KEY = 4,
		DECEL_KEY = 5,
		QUIT_KEY = 6,
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
	double _camy, _camz, _camrot;
	bool _dead;
	QuitCallback _quitcb;
	std::auto_ptr< TextPrinter > _printer;
	size_t _windowwidth, _windowheight;

};

#endif // _CONTROLLER_HPP_
