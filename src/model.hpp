#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <vector>
#include <GL/gl.h>
#include "vector.hpp"

typedef struct
{
	size_t ix[3];

	bool valid()
	{
		return
			ix[0] != ix[1] &&
			ix[1] != ix[2] &&
			ix[2] != ix[0];
	}

} Triangle;

typedef struct
{
	size_t ix[4];
} Quad;

typedef struct
{
	Triangle vertices;
} Face;

typedef struct
{
	std::vector< Vector3 > vertices;
	std::vector< Face > faces;

	void draw()
	{
		glBegin( GL_TRIANGLES );
		for ( size_t i = 0; i < faces.size(); ++i )
		{
			for ( size_t j = 0; j < 3; ++j )
			{
				glVertex3d(
					vertices[faces[i].vertices.ix[j]].x,
					vertices[faces[i].vertices.ix[j]].y,
					vertices[faces[i].vertices.ix[j]].z
				);
			}
		}
		glEnd();
	}

} Model;

#endif // _MODEL_HPP_
