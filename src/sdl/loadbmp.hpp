#ifndef _SDL_LOADBMP_HPP_
#define _SDL_LOADBMP_HPP_

#include <stdexcept>
#include "surface.hpp"

namespace SDL {

inline
Surface::pointer
LoadBmp( const char * filename )
{
	SDL_Surface * image = SDL_LoadBMP( filename );
	if ( !image )
	{
		throw std::runtime_error( "Unable to load image: " + GetError() );
	}
	else
	{
		return Surface::pointer( new Surface( image ) );
	}
}

} // namespace SDL

#endif // _SDL_LOADBMP_HPP_
