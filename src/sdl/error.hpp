#ifndef _SDLERROR_HPP_
#define _SDLERROR_HPP_

#include <string>
#include "sdl.hpp"

namespace SDL {

std::string GetError() {
	return std::string( SDL_GetError() );
}

} // namespace SDL

#endif // _SDLERROR_HPP_
