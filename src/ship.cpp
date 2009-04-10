#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "ship.hpp"
#include "objmodel.hpp"

Ship::Ship()
	: _pos(Vector3(0, 0, 0))
	, _size(Vector3(0.8, 0.5, 1.0))
	, _shipDl(0)
{
}

Ship::~Ship()
{
}

void Ship::initialize()
{
	try
	{
		loadObjModel("ship.obj", _vertices, _faces);
		std::cout << "loaded " << _vertices.size() << " vertices and "
			<< _faces.size() << " faces for ship." << std::endl;
	}
	catch (std::runtime_error& e)
	{
		std::cerr <<
			"Warning: failed to load ship model, "
			"falling back to box ship.\n";
	}
}

void Ship::draw()
{
	glTranslated( 0, _size.y / 2, -_size.z / 2 );
	if ( _vertices.size() && _faces.size() )
	{
		glScaled( _size.x, _size.y, _size.z );
		glBegin( GL_TRIANGLES );
		for ( size_t i = 0; i < _faces.size(); ++i )
		{
			for ( size_t j = 0; j < 3; ++j )
			{
				glVertex3d(
					_vertices[_faces[i].indices[j]].x,
					_vertices[_faces[i].indices[j]].y,
					_vertices[_faces[i].indices[j]].z
				);
			}
		}
		glEnd();
	}
	else
	{
		glScaled( _size.x, _size.y, _size.z );
		glBegin( GL_QUAD_STRIP );
			glVertex3d( -0.5,  0.5,  0.5 );
			glVertex3d( -0.5, -0.5,  0.5 );
			glVertex3d(  0.5,  0.5,  0.5 );
			glVertex3d(  0.5, -0.5,  0.5 );
			glVertex3d(  0.5,  0.5, -0.5 );
			glVertex3d(  0.5, -0.5, -0.5 );
			glVertex3d( -0.5,  0.5, -0.5 );
			glVertex3d( -0.5, -0.5, -0.5 );
			glVertex3d( -0.5,  0.5,  0.5 );
			glVertex3d( -0.5, -0.5,  0.5 );
		glEnd();
		glBegin( GL_QUADS );
			glVertex3d( -0.5, -0.5,  0.5 );
			glVertex3d(  0.5, -0.5,  0.5 );
			glVertex3d(  0.5, -0.5, -0.5 );
			glVertex3d( -0.5, -0.5, -0.5 );

			glVertex3d( -0.5,  0.5,  0.5 );
			glVertex3d(  0.5,  0.5,  0.5 );
			glVertex3d(  0.5,  0.5, -0.5 );
			glVertex3d( -0.5,  0.5, -0.5 );
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
