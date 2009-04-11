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

typedef struct
{

	std::vector< Vector3 > vertices;
	std::vector< Vector3 > normals;
	std::vector< Face<3> > trifaces;
	std::vector< Face<4> > quadfaces;

	void draw()
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
	void drawFace( Face<N>& face )
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

} Model;

#endif // _MODEL_HPP_
