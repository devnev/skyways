#include <GL/gl.h>
#include <stdexcept>
#include <sstream>
#include "block.hpp"

void Block::glDraw()
{
	if (faces.size() > 0)
	{
		glScalef( 1, 1, -1 );
		glBegin( GL_QUADS );
		for ( QuadList::iterator face = faces.begin();
				face != faces.end(); ++face )
		{
			for ( size_t i = 0 ; i < 4 ; ++i )
				glVertex3d(
					vertices[face->indices[i]].x,
					vertices[face->indices[i]].y,
					vertices[face->indices[i]].z
				);
		}
		glEnd();
		glScalef( 1, 1, -1 );
	}
	else
	{
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

std::auto_ptr< Block > Block::fromStream( std::istream& is )
{
	std::auto_ptr< Block > block( new Block() );
	std::string line;

	while ( getline( is, line ) )
	{
		size_t commentStart = line.find( '#' );
		if ( commentStart != std::string::npos )
			line.resize( commentStart );
		size_t start = line.find_first_not_of( " \t" );
		if ( start == std::string::npos )
			continue;
		std::istringstream iss( line );
		std::string type;
		iss >> type;
		if ( type == "v" )
		{
			Vector3 v;
			iss >> v.x >> v.y >> v.z;
			block->vertices.push_back( v );
		}
		else if ( type == "f" )
		{
			Quad f;
			iss >> f.indices[0] >> f.indices[1] >> f.indices[2] >> f.indices[3];
			--f.indices[0]; --f.indices[1]; --f.indices[2]; --f.indices[3];
			block->faces.push_back( f );
		}
		else if ( type == "b" )
		{
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
	if ( faces.size() == 0 )
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
