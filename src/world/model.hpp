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

#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <vector>
#include <GL/gl.h>
#include <vector.hpp>

typedef struct
{

	size_t ix[3];

} Triangle;

typedef struct
{

	size_t ix[4];

} Quad;

template<size_t N>
struct Face
{

	size_t vertices[N];
	size_t normals[N];

	bool valid()
	{
		for (size_t i = 0; i < N; ++i)
		{
			if ( vertices[i] == vertices[(i+1)%N] )
				return false;
		}
		return true;
	}

};

class Model
{

public:

	std::vector< Vector3 > vertices;
	std::vector< Vector3 > normals;
	std::vector< Face<3> > trifaces;
	std::vector< Face<4> > quadfaces;

	void draw() const
	{
		glBegin( GL_TRIANGLES );
		for ( size_t i = 0; i < trifaces.size(); ++i )
			drawFace( trifaces[i] );
		glEnd();
		glBegin( GL_QUADS );
		for ( size_t i = 0; i < quadfaces.size(); ++i )
			drawFace( quadfaces[i] );
		glEnd();
	}

private:

	template<size_t N>
	void drawFace( const Face<N>& face ) const
	{
		for ( size_t i = 0; i < N; ++i )
		{
			if ( normals.size() > 0 )
			{
				glNormal3d(
					normals[face.normals[i]].x,
					normals[face.normals[i]].y,
					normals[face.normals[i]].z
				);
			}
			glVertex3d(
				vertices[face.vertices[i]].x,
				vertices[face.vertices[i]].y,
				vertices[face.vertices[i]].z
			);
		}
	}

};

#endif // _MODEL_HPP_
