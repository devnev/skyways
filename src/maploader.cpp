#include <cctype>
#include <functional>
#include <istream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "block.hpp"
#include "element.hpp"
#include "game.hpp"
#include "maploader.hpp"

struct Row
{
	std::string data;
	double start, length;
};

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

void parseAlias(
	std::istream& is
  , const boost::ptr_map< std::string, Block >& blocks
  , std::vector< std::list< BlockPos > >& aliases
  )
{
	char key;
	is >> key;
	if ( key < 0 )
		throw std::runtime_error( std::string("Invalid alias name ") + key );
	BlockPos pos;
	std::string blockName;
	while ( is >> blockName )
	{
		if ( blocks.find( blockName ) == blocks.end() )
			throw std::runtime_error( "Unknown block " + blockName );
		else
			pos.block = blocks.find( blockName )->second;

		std::string tmp;
		if (!( is >> tmp ))
			throw std::runtime_error( "Unexpected end of line." );
		if (tmp.size() == 1 && std::isalpha(tmp[0]))
		{
			switch ( tmp[0] )
			{
			case 'd': pos.tfn = std::mem_fun_ref( &Game::explode ); break;
			default: pos.tfn = 0; break;
			}
			if (!( is >> tmp ))
				throw std::runtime_error( "Unexpected end of line." );
		}
		else
			pos.tfn = 0;
		std::istringstream( tmp ) >> pos.ypos;

		aliases[ key ].push_back( pos );
	}
}

void parseRow( size_t columns, const Row& row
  , std::vector< Column >& runningdata
  , const std::vector< std::list< BlockPos > >& aliases
  , std::vector< Element >& elements
  )
{
	for (size_t i = 0; i < columns; ++i)
	{
		char key = ' ';
		if ( !row.data.empty() )
		{
			key = row.data[i];
			if ( key < 0 || ( key != ' ' && aliases[key].empty() ) )
				throw std::runtime_error( std::string("Invalid alias name ") + key );
			if ( key == runningdata[i].key )
			{
				runningdata[i].length += row.length;
				continue;
			}
		}

		if ( runningdata[i].key != ' ' )
		{
			for ( std::list< BlockPos >::const_iterator posIter = aliases[runningdata[i].key].begin();
					posIter != aliases[runningdata[i].key].end(); ++posIter)
			{
				elements.push_back(Element(
					( (double)i ) - ( (double)columns ) / 2,
					posIter->ypos, runningdata[i].start, runningdata[i].length,
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

		if ( !row.data.empty() )
		{
			runningdata[i].key = key;
			runningdata[i].start = row.start;
			runningdata[i].length = row.length;
		}
	}
}

void parseMap( size_t columns
  , const std::vector< std::string >& maplines
  , const std::vector< std::list< BlockPos > >& aliases
  , std::vector< Element >& elements
  )
{
	Column emptyColumn = { ' ', 0, 0 };
	Row row;
	std::vector< Column > runningdata(columns, emptyColumn);
	row.start = 0;

	for ( std::vector< std::string >::const_iterator mapiter = maplines.begin();
			mapiter != maplines.end(); ++mapiter )
	{
		const std::string& line = *mapiter;
		size_t seperator = line.find(':');
		if ( seperator == std::string::npos )
			throw std::runtime_error( "Missing seperator in map row." );
		if ( sscanf( line.substr(0, seperator).c_str(), " %lf ", &row.length ) != 1 )
			throw std::runtime_error( "Invalid row length in map row." );
		row.data = line.substr( seperator+1 );
		if ( row.data.length() > columns )
			throw std::runtime_error( "Bad row size." );
		else if ( row.data.length() < columns )
			row.data.resize( columns,  ' ' );
		parseRow( columns, row, runningdata, aliases, elements );
		row.start += row.length;
	}
	row.data.clear();
	parseRow( columns, row, runningdata, aliases, elements );
}

void MapLoader::loadMap( std::istream& is, MapInfo& mapinfo )
{
	std::string line;
	std::vector< std::list< BlockPos > > aliases(256);
	std::vector< Element > elements;
	size_t columns = 0;
	std::vector< std::string > maplines;
	Vector3 startPos = Vector3( 0.5, 0, 0 );

	while ( std::getline( is, line ) )
	{
		std::istringstream iss( line );
		std::string cmd;
		if (!( iss >> cmd ))
			continue;
		else if ( cmd == "alias" )
		{
			parseAlias( iss, *_blocks, aliases );
		}
		else if ( cmd == "start" )
		{
			if (!( iss >> startPos.x >> startPos.y ))
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

	parseMap( columns, maplines, aliases, elements );

	mapinfo.map.reset( new Map( 10, elements ) );
	mapinfo.startPos = startPos;
}
