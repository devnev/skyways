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
#include <cctype>
#include "game.hpp"
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

const Element * Map::collide( const AABB& aabb )
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

	size_t width = 7;
	size_t base = 0;
	std::vector< size_t > running(width, 0);
	running[width/2] = 20;

	for (size_t i = 0; i < 100; ++i)
	{
		size_t distance = rand()%18+8;
		size_t length = rand()%20+2;
		size_t col;
		do {
			col = rand()%width;
		} while (running[col] > 0);
		elements.push_back(Element(
			(int)col - ((int)width)/2,
			((double)(rand() % 5))/4 - 0.5,
			base + distance,
			length,
			block( "" ),
			Vector3(
				( (double)rand() ) / RAND_MAX,
				( (double)rand() ) / RAND_MAX,
				( (double)rand() ) / RAND_MAX
			)
		));
		running[col] += distance + length;
		size_t mindist = distance + length;
		for (size_t j = 0; j < width; ++j)
			mindist = std::min(mindist, running[j]);
		for (size_t j = 0; j < width; ++j)
			running[j] -= mindist;
		base += mindist;
	}
	_startPoint = Vector3(0.5, 0, 0);
}

struct Column
{
	char key;
	double start, length;
};

struct BlockPos
{
	BlockPos() : block(0), ypos(0.0) { }
	const Block * block;
	double ypos;
	Element::TriggerFn tfn;
};

void Map::loadMap( std::istream& is )
{
	std::string line;
	std::vector< std::list< BlockPos > > aliases(256);
	std::vector< Element > newElements;
	size_t columns = 0;
	Vector3 startPoint = Vector3( 0.5, 0, 0 );
	std::vector< std::string > maplines;

	while ( std::getline( is, line ) )
	{
		std::istringstream iss( line );
		std::string cmd;
		if (!( iss >> cmd ))
			continue;
		else if ( cmd == "alias" )
		{
			char key;
			iss >> key;
			if ( key < 0 )
				throw std::runtime_error( std::string("Invalid alias name ") + key );
			BlockPos pos;
			std::string blockName;
			while ( iss >> blockName )
			{
				if ( blocks.find( blockName ) == blocks.end() )
					throw std::runtime_error( "Unknown block " + blockName );
				else
					pos.block = blocks.find( blockName )->second;

				std::string tmp;
				if (!( iss >> tmp ))
					throw std::runtime_error( "Unexpected end of line." );
				if (tmp.size() == 1 && std::isalpha(tmp[0]))
				{
					switch ( tmp[0] )
					{
					case 'd': pos.tfn = std::mem_fun_ref( &Game::explode ); break;
					default: pos.tfn = 0; break;
					}
					if (!( iss >> tmp ))
						throw std::runtime_error( "Unexpected end of line." );
				}
				else
					pos.tfn = 0;
				std::istringstream( tmp ) >> pos.ypos;

				aliases[ key ].push_back( pos );
			}
		}
		else if ( cmd == "start" )
		{
			if (!( iss >> startPoint.x >> startPoint.y ))
				throw std::runtime_error( "Unexpected end of line in start coordinates." );
		}
		else if ( cmd == "map" )
		{
			std::string delim;
			if (!( iss >> columns ))
				throw std::runtime_error( "Unexpected end of line before map width." );
			if (!( iss >> delim ))
				throw std::runtime_error( "Unexpected end of line before map delim." );
			while ( is && std::getline( is, line ) && line != delim )
				maplines.push_back(line);
			if ( line != delim )
				throw std::runtime_error( "Unexpected eof during map block." );
		}
	}

	if ( maplines.empty() )
		throw std::runtime_error( "Missing/empty map block." );

	Column emptyColumn = { ' ', 0, 0 };
	std::vector< Column > runningdata(columns, emptyColumn);
	double rowlength, position = 0.0;

	for ( std::vector< std::string >::iterator mapiter = maplines.begin();
			mapiter != maplines.end(); ++mapiter )
	{
		const std::string& line = *mapiter;
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
							posIter->block,
							Vector3(
								( (double)rand() ) / RAND_MAX,
								( (double)rand() ) / RAND_MAX,
								( (double)rand() ) / RAND_MAX
							),
							posIter->tfn
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
					posIter->block,
					Vector3(
						( (double)rand() ) / RAND_MAX,
						( (double)rand() ) / RAND_MAX,
						( (double)rand() ) / RAND_MAX
					),
					posIter->tfn
				));
			}
		}
	}

	_accelerator.clear();
	using std::swap;
	swap( elements, newElements );
	_startPoint = startPoint;
}
