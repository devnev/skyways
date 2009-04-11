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
		loadObjModel("ship.obj", _model);
		std::cout << "loaded "
			<< _model.vertices.size() << " vertices and "
			<< _model.trifaces.size() + _model.quadfaces.size()
			<< " faces for ship." << std::endl;
	}
	catch (std::runtime_error& e)
	{
		std::cerr <<
			"Warning: failed to load ship model, "
			"falling back to box ship.\n"
			"Exception caught was: "
			<< e.what() << '\n';
	}
}

void Ship::draw()
{
	glTranslated( 0, _size.y / 2, -_size.z / 2 );
	if ( _model.vertices.size() && _model.trifaces.size() )
	{
		glScaled( _size.x, _size.y, -_size.z );
		_model.draw();
	}
	else
	{
		glScaled( _size.x, _size.y, _size.z );

		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 0.01f, 0.01f );
		drawSimple();
		glDisable( GL_POLYGON_OFFSET_FILL );

		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glColor3f( 0.75f, 0.75f, 0.75f );
		glLineWidth( 1.0f );
		drawSimple();
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
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
