#ifndef _SDL_INIT_HPP_
#define _SDL_INIT_HPP_

#include <stdexcept>
#include "error.hpp"

namespace SDL {

class Init
{
public:
	Init()
	{
		if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		{
			throw std::runtime_error( "Unable to init SDL: " + GetError() );
		}
	}
	~Init()
	{
		SDL_Quit();
	}
};

}

#endif // _SDL_INIT_HPP_
