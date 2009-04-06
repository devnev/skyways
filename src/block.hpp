#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

class Block
{
public:

	void glDraw()
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

};

#endif // _BLOCK_HPP_
