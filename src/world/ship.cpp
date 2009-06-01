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

#include <GL/gl.h>
#include <cmath>
#include "model.hpp"
#include "ship.hpp"

Ship::Ship( std::auto_ptr< Model > model )
	: _pos(Vector3(0, 0, 0))
	, _size(Vector3(0.8, 0.5, 1.0))
	, _model(model)
	, _shipDl(0)
{
}

Ship::~Ship()
{
}

void Ship::draw()
{
	glTranslated( 0, _size.y / 2, -_size.z / 2 );
	if ( _model.get() )
	{
		glScaled( _size.x, _size.y, -_size.z );
		_model->draw();
	}
	else
	{
		glScaled( _size.x, _size.y, _size.z );

		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 0.01f, 0.01f );
		drawSimple();
		glDisable( GL_POLYGON_OFFSET_FILL );
	}
}

void Ship::drawDl()
{
	if ( _shipDl == 0 )
	{
		_shipDl = glGenLists( 1 );
		glNewList( _shipDl, GL_COMPILE_AND_EXECUTE );
			draw();
		glEndList();
	}
	else
	{
		glCallList( _shipDl );
	}
}

void Ship::drawSimple()
{
	glBegin( GL_QUADS );
		glNormal3d( 0, 0, 1 );
		glVertex3d( -0.5,  0.5,  0.5 );
		glVertex3d( -0.5, -0.5,  0.5 );
		glVertex3d(  0.5, -0.5,  0.5 );
		glVertex3d(  0.5,  0.5,  0.5 );
	glEnd();
	glBegin( GL_TRIANGLES );
		glNormal3d( -1, 0, -0.5 );
		glVertex3d( -0.5, -0.5,  0.5 );
		glVertex3d( -0.5,  0.5,  0.5 );
		glVertex3d( 0, 0,  -1.0 );

		glNormal3d( 0, 1, -0.5 );
		glVertex3d( -0.5,  0.5,  0.5 );
		glVertex3d(  0.5,  0.5,  0.5 );
		glVertex3d( 0, 0,  -1.0 );

		glNormal3d( 1, 0, -0.5 );
		glVertex3d(  0.5,  0.5,  0.5 );
		glVertex3d(  0.5, -0.5,  0.5 );
		glVertex3d( 0, 0,  -1.0 );

		glNormal3d( 0, -1, -0.5 );
		glVertex3d(  0.5, -0.5,  0.5 );
		glVertex3d( -0.5, -0.5,  0.5 );
		glVertex3d( 0, 0,  -1.0 );
	glEnd();
}
