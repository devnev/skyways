#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include "block.hpp"

class Element
{
public:

	Element( double x, double y, double z, double l, Block * b )
		: _pos(Point::make( x,  y,  z)), _length( l ), _block( b )
	{
	}

	void glDraw()
	{
		glPushMatrix();
		glTranslated( _pos.x, _pos.y, -_pos.z );
		glScaled( 1, 1, _length );
		_block->glDraw();
		glPopMatrix();
	}

	const Point& pos() const throw() { return _pos; }
	double xoff() const throw() { return _pos.x; }
	double yoff() const throw() { return _pos.y; }
	double zoff() const throw() { return _pos.z; }
	double length() const throw() { return _length; }

private:

	Point _pos;
	double _length;
	Block * _block;

};

#endif // _ELEMENT_HPP_
