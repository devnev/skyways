#include <QtOpenGL>
#include <cmath>
#include "ship.hpp"

Ship::Ship()
{
	_pos.x = _pos.y = _pos.z = 0;
}

void Ship::glDraw()
{
	glScaled( 0.8, 0.5, 1.0 );
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
