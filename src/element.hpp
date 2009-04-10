#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include "block.hpp"

class Element
{
public:

	Element( double x, double y, double z, double l, Block * b, const Vector3& color )
		: _pos(Vector3( x,  y,  z)), _length( l ), _block( b ), _color( color )
	{
	}

	void glDraw();

	const Vector3& pos() const throw() { return _pos; }
	double xoff() const throw() { return _pos.x; }
	double yoff() const throw() { return _pos.y; }
	double zoff() const throw() { return _pos.z; }
	double length() const throw() { return _length; }

	bool collide( const AABB& aabb );

private:

	Vector3 _pos;
	double _length;
	Block * _block;
	Vector3 _color;

};

#endif // _ELEMENT_HPP_
