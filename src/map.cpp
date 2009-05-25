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
#include "map.hpp"

Map::Map( size_t sectionSize )
	: _accelerator( sectionSize )
{
	std::string empty;
	blocks.insert( empty, new Block() );
}

void Map::glDraw( double )
{
	for ( ElementList::iterator elem = elements.begin() ;
			elem != elements.end(); ++elem )
	{
		elem->glDraw();
	}
	_elementsDrawn = elements.size();
}

void Map::loadBlocks()
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

void Map::optimize()
{
	for ( ElementList::iterator elem = elements.begin() ;
			elem != elements.end(); ++elem )
	{
		_accelerator.addElement( *elem );
	}
}

bool Map::collide( const AABB& aabb )
{
	return _accelerator.collide( aabb );
}

void Map::generateMap()
{
	_accelerator.clear();
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

void Map::loadMap( std::istream& is )
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

	_accelerator.clear();
	using std::swap;
	swap( elements, newElements );
}