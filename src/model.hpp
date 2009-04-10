#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <vector>
#include <GL/gl.h>
#include "vector.hpp"

typedef struct
{

	size_t ix[3];

} Triangle;

typedef struct
{

	size_t ix[4];

} Quad;

typedef struct
{

	Triangle vertices;
	Triangle normals;

	bool valid()
	{
		return
			vertices.ix[0] != vertices.ix[1] &&
			vertices.ix[1] != vertices.ix[2] &&
			vertices.ix[2] != vertices.ix[0];
	}

} Face;

typedef struct
{

	std::vector< Vector3 > vertices;
	std::vector< Vector3 > normals;
	std::vector< Face > faces;

	void draw()
	{
		glBegin( GL_TRIANGLES );
		for ( size_t i = 0; i < faces.size(); ++i )
		{
			for ( size_t j = 0; j < 3; ++j )
			{
				if ( normals.size() > 0 )
				{
					glNormal3d(
						normals[faces[i].normals.ix[j]].x,
						normals[faces[i].normals.ix[j]].y,
						normals[faces[i].normals.ix[j]].z
					);
				}
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
