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
	bool result = collide(aabb, sections[ section1 ]);
	if ( !result && section2 != section1 )
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
		if (aabb.collide(AABB((*elemref)->pos(), (*elemref)->pos().offset(1, 1, (*elemref)->length()))))
			return true;
	}
	return false;
}

void World::generateWorld()
{
	sections.clear();
	elements.clear();
	for (size_t i = 0; i < 100; ++i)
	{
		elements.push_back(Element(
			rand() % 8 - 4,
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
