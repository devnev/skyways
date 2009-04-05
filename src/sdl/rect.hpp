#ifndef _SDL_RECT_HPP_
#define _SDL_RECT_HPP_

namespace SDL {

struct Rect : public SDL_Rect
{
	Rect(Sint16 x, Sint16 y, Uint16 w, Uint16 h)
	{
		this->x = x; this->y = y;
		this->w = w; this->h = h;
	}
};

} // namespace SDL

#endif // _SDL_RECT_HPP_
