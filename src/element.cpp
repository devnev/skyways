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
#include "element.hpp"

void Element::glDraw()
{
	glPushMatrix();

	glColor4d( _color.x, _color.y, _color.z, 0.5 );
	glTranslated( _pos.x, _pos.y, -_pos.z );
	glScaled( 1, 1, _length );

	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 0.01f, 0.01f );
	_block->drawDl();
	glDisable( GL_POLYGON_OFFSET_FILL );

	glPopMatrix();
}

bool Element::collide( const AABB& aabb ) const
{
	AABB _aabb( aabb.offset( -_pos.x, -_pos.y, -_pos.z ) );
	_aabb.p1.z /= _length;
	_aabb.p2.z /= _length;
	return _block->collide( _aabb );
}
