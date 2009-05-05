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

#include "collisionaccelerator.hpp"
#include <cmath>

CollisionAccelerator::CollisionAccelerator( double sectionSize )
	: _sectionSize( sectionSize )
{
}

void CollisionAccelerator::addElement( const Element& e )
{
	const Element * const pe = &e;

	const double beginDistance = pe->zoff();
	size_t beginSection = (size_t)(beginDistance / _sectionSize);
	size_t sectionCount = (size_t)((pe->length() + fmod(beginDistance, _sectionSize)) / _sectionSize);

	// make sure section array has sufficient space
	if ( sections.size() <= ( beginSection + sectionCount ) )
		sections.resize( beginSection + sectionCount + 1 );

	if ( sectionCount == 0 ) // fully contained in one section
	{
		sections[ beginSection ].complete.push_back( pe );
		return;
	}

	sections[ beginSection ].beginning.push_back( pe );
	++beginSection; --sectionCount;

	for ( ; sectionCount > 0 ; --sectionCount, ++beginSection )
	{
		sections[ beginSection ].running.push_back( pe );
	}

	sections[ beginSection ].ending.push_back( pe );
}

bool CollisionAccelerator::collide(const AABB& aabb) const
{
	size_t section1 = ( (size_t)aabb.p1.z ) / _sectionSize,
		   section2 = ( (size_t)aabb.p2.z ) / _sectionSize;
	if ( section1 >= sections.size() )
		return false;
	bool result = collide(aabb, sections[ section1 ]);
	if ( !result && section2 != section1 && section2 < sections.size() )
		result = collide(aabb, sections[ section2 ]);
	return result;
}

bool CollisionAccelerator::collide(const AABB& aabb, const MapSection& section) const
{
	return (
		collide(aabb, section.beginning) ||
		collide(aabb, section.running) ||
		collide(aabb, section.ending) ||
		collide(aabb, section.complete)
	);
}

bool CollisionAccelerator::collide(const AABB& aabb, const std::vector< const Element* >& elemreflist) const
{
	typedef std::vector< const Element* > ElemRefList;
	typedef ElemRefList::const_iterator ElemRefIter;

	for ( ElemRefIter elemref = elemreflist.begin() ;
			elemref != elemreflist.end(); ++elemref )
	{
		if ( (*elemref)->collide( aabb ) )
			return true;
	}
	return false;
}

