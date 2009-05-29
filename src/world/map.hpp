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

#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <vector>
#include <istream>
#include <boost/ptr_container/ptr_map.hpp>
#include "element.hpp"
#include "aabb.hpp"
#include "collisionaccelerator.hpp"

class Map
{
public:

	Map( size_t sectionSize );
	Map( size_t sectionSize, std::vector< Element >& elements );

	void glDraw( double zmin = 0 );

	void optimize();

	// statistic functions

	size_t elementsDrawn() const throw() { return _elementsDrawn; }
	double lowestPoint() const throw() { return -1; /* TODO: calculate */ }

	// collide AABB with map.
	// assumes aabb.size().z<sectionSize
	const Element * collide( const AABB& aabb );

private:

	CollisionAccelerator _accelerator;

	typedef std::vector< Element > ElementList;
	ElementList _elements;

	size_t _elementsDrawn; // for statistics

};

#endif // _MAP_HPP_
