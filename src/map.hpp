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

class Map
{
public:

	Map( size_t sectionSize );

	void glDraw( double zmin = 0 );

	void loadBlocks();

	void generateMap();
	void loadMap( std::istream& is );
	void optimize();

	Block * block( const char * name ) { return &blocks.at( name ); }

	// statistic functions

	size_t elementsDrawn() const throw() { return _elementsDrawn; }
	size_t blocksLoaded() const throw() { return blocks.size(); }
	double lowestPoint() const throw() { return -1; /* TODO: calculate */ }

	// collide AABB with map.
	// assumes aabb.size().z<sectionSize
	bool collide( const AABB& aabb );

private:

	void processElement( Element& e );

	typedef std::vector< Element > ElementList;
	ElementList elements;

	size_t sectionSize;
	typedef struct
	{
		std::vector< Element* >
			beginning, running, ending, complete;
	} MapSection;
	typedef std::vector< MapSection > SectionList;
	SectionList sections;

	size_t _elementsDrawn; // for statistics

	boost::ptr_map< std::string, Block > blocks;

	bool collide( const AABB& aabb, MapSection& section );
	bool collide( const AABB& aabb, std::vector< Element* >& elemreflist );

};

#endif // _MAP_HPP_
