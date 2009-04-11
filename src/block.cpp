#include <GL/gl.h>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <string>
#include "objmodel.hpp"
#include "block.hpp"

void Block::draw()
{
	if (model.vertices.size() > 0)
	{
		glScalef( 1, 1, -1 );
		model.draw();
		glScalef( 1, 1, -1 );
	}
	else
	{
		glBegin( GL_QUADS );
			glNormal3f( 0, 0, 1 );
			glVertex3f( 0, 0, 0 );
			glVertex3f( 1, 0, 0 );
			glVertex3f( 1, 1, 0 );
			glVertex3f( 0, 1, 0 );

			glNormal3f( 1, 0, 0 );
			glVertex3f( 1, 0, 0 );
			glVertex3f( 1, 0, -1 );
			glVertex3f( 1, 1, -1 );
			glVertex3f( 1, 1, 0 );

			glNormal3f( 0, 0, -1 );
			glVertex3f( 1, 0, -1 );
			glVertex3f( 0, 0, -1 );
			glVertex3f( 0, 1, -1 );
			glVertex3f( 1, 1, -1 );

			glNormal3f( -1, 0, 0 );
			glVertex3f( 0, 0, -1 );
			glVertex3f( 0, 0, 0 );
			glVertex3f( 0, 1, 0 );
			glVertex3f( 0, 1, -1 );

			glNormal3f( 0, -1, 0 );
			glVertex3f( 0, 0, 0 );
			glVertex3f( 0, 0, -1 );
			glVertex3f( 1, 0, -1 );
			glVertex3f( 1, 0, 0 );

			glNormal3f( 0, 1, 0 );
			glVertex3f( 0, 1, 0 );
			glVertex3f( 1, 1, 0 );
			glVertex3f( 1, 1, -1 );
			glVertex3f( 0, 1, -1 );
		glEnd();
	}
}

void Block::drawDl()
{
	if ( _blockDl == 0 )
	{
		_blockDl = glGenLists( 1 );
		glNewList( _blockDl, GL_COMPILE_AND_EXECUTE );
			draw();
		glEndList();
	}
	else
	{
		glCallList( _blockDl );
	}
}

std::auto_ptr< Block > Block::fromFile( const std::string& filename )
{
	std::auto_ptr< Block > block( new Block() );
	ObjUnknownsList objunknowns;

	loadObjModel( filename.c_str(), block->model, false, &objunknowns );

	for ( ObjUnknownsList::iterator unknown = objunknowns.begin();
			unknown != objunknowns.end(); ++unknown )
	{
		if ( unknown->first == "b" )
		{
			std::istringstream iss( unknown->second );
			std::string cmd;
			iss >> cmd;

			AABB aabb;
			iss >> aabb.p1.x >> aabb.p1.y >> aabb.p1.z
				>> aabb.p2.x >> aabb.p2.y >> aabb.p2.z;
			block->bounds.push_back( aabb );
		}
	}
	return block;
}

bool Block::collide( const AABB& aabb ) const throw()
{
	if ( model.vertices.size() == 0 )
	{
		return aabb.collide( AABB( Vector3( 0, 0, 0 ), Vector3( 1, 1, 1 ) ) );
	}
	for ( AabbList::const_iterator iter = bounds.begin();
			iter != bounds.end(); ++iter )
	{
		if ( aabb.collide( *iter ) )
			return true;
	}
	return false;
}
