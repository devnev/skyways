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

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glColor3f( 0.75f, 0.75f, 0.75f );
	glLineWidth( 1.0f );
	_block->drawDl();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glPopMatrix();
}

bool Element::collide( const AABB& aabb )
{
	AABB _aabb( aabb.offset( -_pos.x, -_pos.y, -_pos.z ) );
	_aabb.p1.z /= _length;
	_aabb.p2.z /= _length;
	return _block->collide( _aabb );
}
