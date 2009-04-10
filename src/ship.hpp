#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include <vector>
#include <GL/gl.h>
#include "vector.hpp"

class Ship
{
public:

	Ship();
	~Ship();

	void initialize();

	double xpos() const throw() { return _pos.x; }
	double ypos() const throw() { return _pos.y; }
	double zpos() const throw() { return _pos.z; }
	const Vector3& pos() const throw() { return _pos; }
	Vector3& pos() throw() { return _pos; }
	const Vector3& size() const throw() { return _size; }

	void draw();
	void drawDl();

private:

	Vector3 _pos;
	Vector3 _size;

	std::vector< Vector3 > _vertices;
	std::vector< Triangle > _faces;

	GLuint _shipDl;

};

#endif // _SHIP_HPP_
