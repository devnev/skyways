#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include <vector>
#include "element.hpp"

class World
{
public:

	World( size_t sectionSize );

	void glDraw( double zmin = 0 );

	void add( const Element& e );

	void optimize();

	size_t elementsDrawn() const throw() { return _elementsDrawn; }

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

};

#endif // _WORLD_HPP_
