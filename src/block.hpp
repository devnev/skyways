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
	static std::auto_ptr< Block > fromFile( const std::string& filename );

	bool collide( const AABB& aabb ) const throw();

private:

	Model model;
	typedef std::vector< AABB > AabbList;
	AabbList bounds;

	GLuint _blockDl;

};

#endif // _BLOCK_HPP_
