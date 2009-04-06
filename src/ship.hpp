#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "point.hpp"

class Ship
{
public:

	Ship();

	double xpos() const throw() { return _pos.x; }
	double ypos() const throw() { return _pos.y; }
	double zpos() const throw() { return _pos.z; }
	const Point& pos() const throw() { return _pos; }
	Point& pos() throw() { return _pos; }

	void glDraw();

private:

	Point _pos;

};

#endif // _SHIP_HPP_
