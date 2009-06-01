//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#ifndef _SHIP_HPP_
#define _SHIP_HPP_

#include <vector>
#include <memory>
#include <GL/gl.h>
#include <vector.hpp>

class Model;

class Ship
{
public:

	Ship( std::auto_ptr< Model > model );
	~Ship();

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

	std::auto_ptr< Model > _model;

	GLuint _shipDl;

};

#endif // _SHIP_HPP_
