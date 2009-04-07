#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include "block.hpp"

class Element
{
public:

	Element( double x, double y, double z, double l, Block * b, const Point& color )
		: _pos(Point::make( x,  y,  z)), _length( l ), _block( b ), _color( color )
	{
	}

	void glDraw()
	{
		glPushMatrix();
		glColor3d( _color.x, _color.y, _color.z );
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
	Point _color;

};

#endif // _ELEMENT_HPP_
