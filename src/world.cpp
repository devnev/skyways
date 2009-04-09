#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <algorithm>
#include <functional>
#include <fstream>
#include <string>
#include <sstream>
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
		ifstream block_file( block_fs_entry->path() );
		blocks.insert( block_path, Block::fromStream( block_file ) );
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

void World::loadWorld( std::istream& is )
{
	std::string line;
	std::vector< std::pair< std::string, double > >
		aliases(256, std::make_pair(std::string(), 0.0));
	std::vector< Element > newElements;

	while ( std::getline( is, line ) )
	{
		if ( line == "%%" )
			break;
		std::istringstream iss( line );
		std::string block;
		char key;
		double ypos;
		iss >> key >> block >> ypos;
		if ( key < 0 )
			throw std::runtime_error( std::string("Invalid alias name ") + key );
		if ( blocks.find( block ) == blocks.end() )
			throw std::runtime_error( "Unknown block " + block );
		aliases[ key ] = std::make_pair( block, ypos );
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
			if ( key < 0 || ( key != ' ' && aliases[key].first.empty() ) )
				throw std::runtime_error( std::string("Invalid alias name ") + key );
			if ( key == runningdata[i].key )
			{
				runningdata[i].length += rowlength;
			}
			else
			{
				if ( runningdata[i].key != ' ' )
				{
					newElements.push_back(Element(
						( (double)i ) - ( (double)columns ) / 2,
						aliases[runningdata[i].key].second,
						runningdata[i].start,
						runningdata[i].length,
						blocks.find( aliases[runningdata[i].key].first )->second,
						Vector3(
							( (double)rand() ) / RAND_MAX,
							( (double)rand() ) / RAND_MAX,
							( (double)rand() ) / RAND_MAX
						)
					));
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
			newElements.push_back(Element(
				( (double)i ) - ( (double)columns ) / 2,
				aliases[runningdata[i].key].second,
				runningdata[i].start,
				runningdata[i].length,
				blocks.find( aliases[runningdata[i].key].first )->second,
				Vector3(
					( (double)rand() ) / RAND_MAX,
					( (double)rand() ) / RAND_MAX,
					( (double)rand() ) / RAND_MAX
				)
			));
		}
	}

	sections.clear();
	using std::swap;
	swap( elements, newElements );
}
