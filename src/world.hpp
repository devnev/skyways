#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include <vector>
#include <boost/ptr_container/ptr_map.hpp>
#include "element.hpp"

class World
{
public:

	World( size_t sectionSize );

	void glDraw( double zmin = 0 );

	void loadBlocks();

	void add( const Element& e );

	void optimize();

	Block * block( const char * name ) { return &blocks.at( name ); }

	// statistic functions

	size_t elementsDrawn() const throw() { return _elementsDrawn; }
	size_t blocksLoaded() const throw() { return blocks.size(); }

private:

	std::auto_ptr< Element > readElement( std::istream& is )
	{
		double x, y, z, l;
		std::string block;
		is >> x >> y >> z >> l >> block;
		return std::auto_ptr< Element >( new Element( x, y, z, l, &blocks.at( block ) ) );
	}

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

};

#endif // _WORLD_HPP_
