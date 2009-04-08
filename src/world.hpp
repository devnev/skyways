#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include <vector>
#include <istream>
#include <boost/ptr_container/ptr_map.hpp>
#include "element.hpp"
#include "aabb.hpp"

class World
{
public:

	World( size_t sectionSize );

	void glDraw( double zmin = 0 );

	void loadBlocks();

	void generateWorld();
	void loadWorld( std::istream& is );
	void optimize();

	Block * block( const char * name ) { return &blocks.at( name ); }

	// statistic functions

	size_t elementsDrawn() const throw() { return _elementsDrawn; }
	size_t blocksLoaded() const throw() { return blocks.size(); }

	// collide AABB with world.
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
	} WorldSection;
	typedef std::vector< WorldSection > SectionList;
	SectionList sections;

	size_t _elementsDrawn; // for statistics

	boost::ptr_map< std::string, Block > blocks;

	bool collide( const AABB& aabb, WorldSection& section );
	bool collide( const AABB& aabb, std::vector< Element* >& elemreflist );

};

#endif // _WORLD_HPP_
