#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "ship.hpp"
#include "objmodel.hpp"

Ship::Ship()
	: _pos(Vector3(0, 0, 0))
	, _size(Vector3(0.8, 0.5, 1.0))
	, _vertices(0), _faces(0)
	, _shipDl(0)
{
}

Ship::~Ship()
{
	delete [] _vertices;
	delete [] _faces;
}

void Ship::initialize()
{
	try
	{
		std::vector< Vector3 > vertices;
		std::vector< Triangle > faces;
		loadObjModel("ship.obj", vertices, faces);
		_vertices = new Vector3[vertices.size()];
		_faces = new Triangle[faces.size()];
		_vertexCount = vertices.size();
		_faceCount = faces.size();
		std::copy(vertices.begin(), vertices.end(), _vertices);
		std::copy(faces.begin(), faces.end(), _faces);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE,
				sizeof(Vector3) - 3*sizeof(double),
				_vertices);
		std::cout << "loaded " << _vertexCount << " vertices and "
			<< _faceCount << " faces for ship." << std::endl;
	}
	catch (std::runtime_error& e)
	{
		delete [] _vertices;
		delete [] _faces;
		std::cerr <<
			"Warning: failed to load ship model, "
			"falling back to box ship.\n";
	}
}

void Ship::draw()
{
	if ( _vertices && _faces )
	{
		glScaled( 0.01, 0.01, 0.01 );
		glDrawElements(GL_TRIANGLES, _faceCount, GL_UNSIGNED_INT, _faces);
	}
	else
	{
		glScaled( _size.x, _size.y, _size.z );
		glBegin( GL_QUAD_STRIP );
			glVertex3d( 0, 1, 0 );
			glVertex3d( 0, 0, 0 );
			glVertex3d( 1, 1, 0 );
			glVertex3d( 1, 0, 0 );
			glVertex3d( 1, 1, -1 );
			glVertex3d( 1, 0, -1 );
			glVertex3d( 0, 1, -1 );
			glVertex3d( 0, 0, -1 );
			glVertex3d( 0, 1, 0 );
			glVertex3d( 0, 0, 0 );
		glEnd();
		glBegin( GL_QUADS );
			glVertex3d( 0, 0, 0 );
			glVertex3d( 1, 0, 0 );
			glVertex3d( 1, 0, -1 );
			glVertex3d( 0, 0, -1 );

			glVertex3d( 0, 1, 0 );
			glVertex3d( 1, 1, 0 );
			glVertex3d( 1, 1, -1 );
			glVertex3d( 0, 1, -1 );
		glEnd();
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
