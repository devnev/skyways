//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

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

	Block()
		: _model( 0 )
		, _bounds()
		, _blockDl( 0 )
	{
	}

	Block( std::auto_ptr<Model> model, std::vector< AABB > bounds)
		: _model( model )
		, _bounds( bounds )
		, _blockDl( 0 )
	{
	}

	void draw() const;
	void drawDl() const;

	bool collide( const AABB& aabb ) const throw();

private:

	std::auto_ptr< Model > _model;
	typedef std::vector< AABB > AabbList;
	AabbList _bounds;

	mutable GLuint _blockDl;

};

#endif // _BLOCK_HPP_
