#ifndef _SDL_SETVIDEOMODE_HPP_
#define _SDL_SETVIDEOMODE_HPP_

#include "surface.hpp"

namespace SDL {

inline
Surface::pointer
SetVideoMode( int w, int h, int bpp, Uint32 flags )
{
	SDL_Surface * screen = SDL_SetVideoMode( w, h, bpp, flags );
	if ( !screen )
	{
		throw std::runtime_error( "Unable to set video mode: " + GetError() );
	}
	else
	{
		return Surface::pointer( new Surface( screen ) );
	}
}

} // namespace SDL

#endif // _SDL_SETVIDEOMODE_HPP_
