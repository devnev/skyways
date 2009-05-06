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

#ifndef _COLLISIONACCELERATOR_HPP_
#define _COLLISIONACCELERATOR_HPP_

#include <vector>
#include "element.hpp"
#include "aabb.hpp"

class CollisionAccelerator
{

public:

	CollisionAccelerator( double sectionSize );

	void addElement( const Element& e );
	void clear() { sections.clear(); }

	double sectionSize() const throw() { return _sectionSize; }

	const Element * collide( const AABB& aabb ) const;

private:

	double _sectionSize;
	typedef struct
	{
		std::vector< const Element* >
			beginning, running, ending, complete;
	} MapSection;
	typedef std::vector< MapSection > SectionList;
	SectionList sections;

	size_t _elementsDrawn; // for statistics

	const Element * collide( const AABB& aabb, const MapSection& section ) const;
	const Element * collide( const AABB& aabb, const std::vector< const Element* >& elemreflist ) const;

};

#endif // _COLLISIONACCELERATOR_HPP_
