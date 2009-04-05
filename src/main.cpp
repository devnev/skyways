#include "sdl/init.hpp"
#include "sdl/surface.hpp"
#include "sdl/setvideomode.hpp"
#include "sdl/loadbmp.hpp"
#include "sdl/rect.hpp"
#include <iostream>
#include "controller.hpp"

int main ( int argc, char** argv )
{
	try
	{
		SDL::Init init;

		SDL::Surface::pointer screen(
			SDL::SetVideoMode( 640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF )
		);

		// load c::b image
		SDL::Surface::pointer bmp( SDL::LoadBmp( "cb.bmp" ) );

		// centre the bitmap on screen
		SDL::Rect dstrect(
			( screen->p()->w - bmp->p()->w ) / 2,
			( screen->p()->h - bmp->p()->h ) / 2,
			0, 0
		);

		Ship ship( 100 );
		Controller controller( &ship, 1.2, 1.2 );

		// program main loop
		bool done = false;
		while ( !done )
		{
			// message processing loop
			SDL_Event event;
			while ( SDL_PollEvent( &event ) )
			{
				// check for messages
				switch ( event.type )
				{
					// exit if the window is closed
					case SDL_QUIT:
						done = true;
						break;

					// check for keypresses
					case SDL_KEYDOWN:
						controller.keydown( &event.key );
						break;

					case SDL_KEYUP:
						controller.keyup( &event.key );
						break;
				}
			}

			controller.update();

			// DRAWING STARTS HERE

			std::cout << ship.xpos() << ", " << ship.speed() << std::endl;

			// clear screen
			SDL_FillRect(screen->p(), 0, SDL_MapRGB(screen->p()->format, 0, 0, 0));

			// draw bitmap
			SDL_BlitSurface(bmp->p(), 0, screen->p(), &dstrect);

			// DRAWING ENDS HERE

			// finally, update the screen :)
			SDL_Flip(screen->p());

		}
	}
	catch ( std::exception& e )
	{
		std::cerr << "Fatal error " << e.what() << "\n";
		return 1;
	}
	return 0;
}
