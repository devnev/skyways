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

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include "world.hpp"

World::World( size_t sectionSize )
	: sectionSize( sectionSize )
{
	std::string empty;
	blocks.insert( empty, new Block() );
}

void World::glDraw( double )
{
	for ( ElementList::iterator elem = elements.begin() ;
			elem != elements.end(); ++elem )
	{
		elem->glDraw();
	}
	_elementsDrawn = elements.size();
}

void World::loadBlocks()
{
	using namespace boost::filesystem;
	path block_dir( "blocks" );
	if ( !exists( block_dir ) )
		return;
	for ( recursive_directory_iterator block_fs_entry( block_dir ) ;
			block_fs_entry != recursive_directory_iterator() ;
			++block_fs_entry )
	{
		if ( is_directory( block_fs_entry->status() ) )
			continue;
		std::string block_path;
		path::iterator iter = block_fs_entry->path().begin();
		++iter;
		if ( iter != block_fs_entry->path().end() )
			block_path = *iter;
		for ( ++iter ; iter != block_fs_entry->path().end() ; ++iter )
			block_path = block_path + "/" + *iter;
		blocks.insert( block_path, Block::fromFile( block_fs_entry->path().string() ) );
	}
}

void World::optimize()
{
	for ( ElementList::iterator elem = elements.begin() ;
			elem != elements.end(); ++elem )
	{
		processElement( *elem );
	}
}

void World::processElement( Element& e )
{
	Element * pe = &e;

	size_t beginIndex = (size_t)pe->zoff();
	size_t beginSection = beginIndex / sectionSize;
	size_t sectionCount = ( (size_t)pe->length() + ( beginIndex % sectionSize ) ) / sectionSize;

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

bool World::collide(const AABB& aabb)
{
	size_t section1 = ( (size_t)aabb.p1.z ) / sectionSize,
		   section2 = ( (size_t)aabb.p2.z ) / sectionSize;
	if ( section1 >= sections.size() )
		return false;
	bool result = collide(aabb, sections[ section1 ]);
	if ( !result && section2 != section1 && section2 < sections.size() )
		result = collide(aabb, sections[ section2 ]);
	return result;
}

bool World::collide(const AABB& aabb, WorldSection& section)
{
	return (
		collide(aabb, section.beginning) ||
		collide(aabb, section.running) ||
		collide(aabb, section.ending) ||
		collide(aabb, section.complete)
	);
}

bool World::collide(const AABB& aabb, std::vector< Element* >& elemreflist)
{
	typedef std::vector< Element* > ElemRefList;
	typedef ElemRefList::iterator ElemRefIter;

	for ( ElemRefIter elemref = elemreflist.begin() ;
			elemref != elemreflist.end(); ++elemref )
	{
		if ( (*elemref)->collide( aabb ) )
			return true;
	}
	return false;
}

void World::generateWorld()
{
	sections.clear();
	elements.clear();
	elements.push_back(Element(
		0, -1, 0, 20, block( "" ), Vector3( 1, 1, 1 )
	));
	for (size_t i = 0; i < 100; ++i)
	{
		elements.push_back(Element(
			rand() % 7 - 3,
			((double)(rand() % 5))/4 - 0.5,
			rand() % 400,
			rand() % 20 + 2,
			block( "" ),
			Vector3(
				( (double)rand() ) / RAND_MAX,
				( (double)rand() ) / RAND_MAX,
				( (double)rand() ) / RAND_MAX
			)
		));
	}
}

struct Column
{
	char key;
	double start, length;
};

struct BlockPos
{
	BlockPos() : block(), ypos(0.0) { }
	std::string block;
	double ypos;
};

void World::loadWorld( std::istream& is )
{
	std::string line;
	std::vector< std::list< BlockPos > > aliases(256);
	std::vector< Element > newElements;

	while ( std::getline( is, line ) )
	{
		if ( line == "%%" )
			break;
		std::istringstream iss( line );
		char key;
		iss >> key;
		if ( key < 0 )
			throw std::runtime_error( std::string("Invalid alias name ") + key );
		BlockPos pos;
		while ( iss >> pos.block >> pos.ypos )
		{
			if ( blocks.find( pos.block ) == blocks.end() )
				throw std::runtime_error( "Unknown block " + pos.block );
			aliases[ key ].push_back( pos );
		}
	}
	if ( !is || !std::getline( is, line ) )
		throw std::runtime_error( "Unexpected eof before map." );

	size_t columns;
	{
		std::istringstream iss( line );
		iss >> columns;
	}

	Column emptyColumn = { ' ', 0, 0 };

	std::vector< Column > runningdata(columns, emptyColumn);

	double rowlength, position = 0.0;

	while ( std::getline( is, line ) )
	{
		size_t seperator = line.find(':');
		if ( seperator == std::string::npos )
			throw std::runtime_error( "Missing seperator in map row." );
		if ( sscanf( line.substr(0, seperator).c_str(), " %lf ", &rowlength ) != 1 )
			throw std::runtime_error( "Invalid row length in map row." );
		std::string row = line.substr( seperator+1 );
		if ( row.length() > columns )
			throw std::runtime_error( "Bad row size." );
		else if ( row.length() < columns )
			row.resize( columns,  ' ' );
		for (size_t i = 0; i < columns; ++i)
		{
			char key = row[i];
			if ( key < 0 || ( key != ' ' && aliases[key].empty() ) )
				throw std::runtime_error( std::string("Invalid alias name ") + key );
			if ( key == runningdata[i].key )
			{
				runningdata[i].length += rowlength;
			}
			else
			{
				if ( runningdata[i].key != ' ' )
				{
					for ( std::list< BlockPos >::iterator posIter =
							aliases[runningdata[i].key].begin();
							posIter != aliases[runningdata[i].key].end();
							++posIter)
					{
						newElements.push_back(Element(
							( (double)i ) - ( (double)columns ) / 2,
							posIter->ypos,
							runningdata[i].start,
							runningdata[i].length,
							blocks.find( posIter->block )->second,
							Vector3(
								( (double)rand() ) / RAND_MAX,
								( (double)rand() ) / RAND_MAX,
								( (double)rand() ) / RAND_MAX
							)
						));
					}
				}
				runningdata[i].key = key;
				runningdata[i].start = position;
				runningdata[i].length = rowlength;
			}
		}
		position += rowlength;
	}
	for (size_t i = 0; i < columns; ++i)
	{
		if ( runningdata[i].key != ' ' )
		{
			for ( std::list< BlockPos >::iterator posIter =
					aliases[runningdata[i].key].begin();
					posIter != aliases[runningdata[i].key].end();
					++posIter)
			{
				newElements.push_back(Element(
					( (double)i ) - ( (double)columns ) / 2,
					posIter->ypos,
					runningdata[i].start,
					runningdata[i].length,
					blocks.find( posIter->block )->second,
					Vector3(
						( (double)rand() ) / RAND_MAX,
						( (double)rand() ) / RAND_MAX,
						( (double)rand() ) / RAND_MAX
					)
				));
			}
		}
	}

	sections.clear();
	using std::swap;
	swap( elements, newElements );
}
