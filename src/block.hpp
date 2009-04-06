#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <QtOpenGL>
#include <memory>
#include <vector>
#include <stdexcept>
#include <istream>
#include <sstream>
#include "point.hpp"

class Block
{

public:

	void glDraw()
	{
		if (faces.size() > 0)
		{
			glBegin( GL_QUADS );
			for ( FaceList::iterator face = faces.begin();
					face != faces.end(); ++face )
			{
				for ( size_t i = 0 ; i < 4 ; ++i )
					glVertex3d( (*face)[i].x, (*face)[i].y, (*face)[i].z );
			}
			glEnd();
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

	static std::auto_ptr< Block > fromStream( std::istream& is )
	{
		std::auto_ptr< Block > block( new Block() );
		std::string line;

		getline( is, line );

		size_t faces;
		sscanf( line.c_str(), "%zd", &faces );

		for ( size_t i = 0; i < faces; ++i )
		{
			Face face;
			for ( size_t f = 0; f < 4; ++f )
			{
				getline( is, line );
				if ( !is )
					throw std::runtime_error( "unexpected eof in block description" );
				std::istringstream iss( line );
				iss >> face[f].x >> face[f].y >> face[f].z;
			}
			block->faces.push_back( face );
		}

		return block;
	}

private:

	static void getline( std::istream& is, std::string& line )
	{
		while ( std::getline( is, line ) )
		{
			size_t ns = line.find_first_not_of( " \t\n" );
			if ( ns != std::string::npos && line[ ns ] != '#' )
				break;
		}
		if ( !is )
			line.clear();
	}

	typedef struct { Point points[4]; Point& operator[](size_t index) { return points[index]; } } Face;
	typedef std::vector< Face > FaceList;
	FaceList faces;

};

#endif // _BLOCK_HPP_
