#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include <GL/gl.h>
#include "block.hpp"

class Element
{
public:

	Element( double x, double y, double z, double l, Block * b, const Vector3& color )
		: _pos(Vector3( x,  y,  z)), _length( l ), _block( b ), _color( color )
	{
	}

	void glDraw()
	{
		glPushMatrix();

		glColor4d( _color.x, _color.y, _color.z, 0.75 );
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

	const Vector3& pos() const throw() { return _pos; }
	double xoff() const throw() { return _pos.x; }
	double yoff() const throw() { return _pos.y; }
	double zoff() const throw() { return _pos.z; }
	double length() const throw() { return _length; }

	bool collide( const AABB& aabb )
	{
		AABB _aabb( aabb.offset( -_pos.x, -_pos.y, -_pos.z ) );
		_aabb.p1.z /= _length;
		_aabb.p2.z /= _length;
		return _block->collide( _aabb );
	}

private:

	Vector3 _pos;
	double _length;
	Block * _block;
	Vector3 _color;

};

#endif // _ELEMENT_HPP_
