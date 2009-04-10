#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <memory>
#include <vector>
#include <istream>
#include <GL/gl.h>
#include "vector.hpp"
#include "model.hpp"
#include "aabb.hpp"

class Block
{

public:

	Block() : _blockDl( 0 ) { }

	void draw();
	void drawDl();

	static std::auto_ptr< Block > fromStream( std::istream& is );

	bool collide( const AABB& aabb ) const throw();

private:

	std::vector< Vector3 > vertices;
	typedef std::vector< Quad > QuadList;
	QuadList faces;
	typedef std::vector< AABB > AabbList;
	AabbList bounds;

	GLuint _blockDl;

};

#endif // _BLOCK_HPP_
