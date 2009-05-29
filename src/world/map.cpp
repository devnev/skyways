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

#include <algorithm>
#include <functional>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <cctype>
#include <game.hpp>
#include "map.hpp"

Map::Map( size_t sectionSize )
	: _accelerator( sectionSize )
{
}

Map::Map( size_t sectionSize, std::vector< Element >& elements )
	: _accelerator( sectionSize )
{
	using std::swap;
	swap ( elements, _elements );
}

void Map::glDraw( double )
{
	for ( ElementList::iterator elem = _elements.begin() ;
			elem != _elements.end(); ++elem )
	{
		elem->glDraw();
	}
	_elementsDrawn = _elements.size();
}

void Map::optimize()
{
	for ( ElementList::iterator elem = _elements.begin() ;
			elem != _elements.end(); ++elem )
	{
		_accelerator.addElement( *elem );
	}
}

const Element * Map::collide( const AABB& aabb )
{
	return _accelerator.collide( aabb );
}
