#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include <vector>
#include <GL/gl.h>
#include "vector.hpp"
#include "model.hpp"

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

	void drawSimple();

	Vector3 _pos;
	Vector3 _size;

	Model _model;

	GLuint _shipDl;

};

#endif // _SHIP_HPP_
