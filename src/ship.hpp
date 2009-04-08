#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include "vector.hpp"

class Ship
{
public:

	Ship();

	double xpos() const throw() { return _pos.x; }
	double ypos() const throw() { return _pos.y; }
	double zpos() const throw() { return _pos.z; }
	const Vector3& pos() const throw() { return _pos; }
	Vector3& pos() throw() { return _pos; }
	const Vector3& size() const throw() { return _size; }

	void glDraw();

private:

	Vector3 _pos;
	Vector3 _size;

};

#endif // _SHIP_HPP_
