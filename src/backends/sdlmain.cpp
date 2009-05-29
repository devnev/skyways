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

#include <stdexcept>
#include <boost/format.hpp>
#include <SDL.h>
#include <controller.hpp>
#include "configparser.hpp"
#include <configuration.hpp>

void SdlThrowError( const char * message )
{
	throw new std::runtime_error( (
		boost::format("%1%: %2%") % message % SDL_GetError()
	).str() );
}

class SdlInit
{
public:
	SdlInit()
	{
		if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
			SdlThrowError( "Unable to init SDL" );
	}
	~SdlInit() { SDL_Quit(); }
};

class SdlScreen
{
public:
	SdlScreen( SDL_Surface * screen )
	{
		_screen = screen;
		if ( !_screen )
			SdlThrowError( "Unable to set video mode" );
	}
private:
	SDL_Surface * _screen;
};

int mapSdlKey( SDLKey key )
{
	switch ( key )
	{
	case SDLK_ESCAPE: return Controller::QUIT_KEY;
	case SDLK_SPACE: return Controller::JUMP_KEY;
	case SDLK_LEFT: return Controller::STRAFE_L_KEY;
	case SDLK_RIGHT: return Controller::STRAFE_R_KEY;
	case SDLK_UP: return Controller::ACCEL_KEY;
	case SDLK_DOWN: return Controller::DECEL_KEY;
	default: return -1;
	}
}

std::auto_ptr< Controller > controller;

bool g_running;

void endMainLoop() { g_running = false; }

int main(int argc, char* argv[])
{
	SdlInit init;
	SdlScreen screen( SDL_SetVideoMode( 800, 600, 32, SDL_OPENGL ) );

	Configuration config;
	ConfigParser configParser( config );
	if ( !configParser.args( argc, argv ) )
		return 0;
	controller = config.buildController( &endMainLoop );

	controller->initialize();
	controller->resize( 800, 600 );
	g_running = true;

	int time = SDL_GetTicks();
	while ( g_running )
	{
		int newTime = SDL_GetTicks();
		controller->update( newTime - time );
		time = newTime;

		controller->draw();
		SDL_GL_SwapBuffers();

		SDL_Event event;
		int key;
		while ( SDL_PollEvent(&event) )
		{
			if ( event.type == SDL_QUIT )
				endMainLoop();
			else if ( event.type == SDL_KEYDOWN && ( key = mapSdlKey( event.key.keysym.sym ) ) != -1 )
				controller->keydown( key );
			else if ( event.type == SDL_KEYUP && ( key = mapSdlKey( event.key.keysym.sym ) ) != -1 )
				controller->keyup( key );
		}
	}
}
