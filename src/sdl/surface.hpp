#ifndef _SDL_SURFACE_HPP_
#define _SDL_SURFACE_HPP_

#include <memory>
#include "error.hpp"

namespace SDL {

class Surface
{
public:
	typedef std::auto_ptr< Surface > pointer;
	Surface( SDL_Surface* surface )
		: _surface( surface )
	{
	}
	~Surface()
	{
		SDL_FreeSurface( _surface );
	}
	SDL_Surface * p()
	{
		return _surface;
	}
private:
	SDL_Surface * _surface;
};

} // namespace SDL

#endif // _SDL_SURFACE_HPP_
