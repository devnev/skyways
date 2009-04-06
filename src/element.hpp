#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include "block.hpp"

class Element
{
public:

	Element( double x, double y, double z, double l )
		: _xoff( x ), _yoff( y ), _zoff( z ), _length( l )
	{
	}

	void glDraw()
	{
		glPushMatrix();
		glTranslated( _xoff, _yoff, -_zoff );
		glScaled( 1, 1, _length );
		block.glDraw();
		glPopMatrix();
	}

	double xoff() const throw() { return _xoff; }
	double yoff() const throw() { return _yoff; }
	double zoff() const throw() { return _zoff; }
	double length() const throw() { return _length; }

private:

	Block block;
	double _xoff, _yoff, _zoff, _length;

};

#endif // _ELEMENT_HPP_
