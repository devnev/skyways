#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include <vector>
#include "element.hpp"

class World
{
public:

	World( size_t sectionSize )
		: sectionSize( sectionSize )
	{
	}

	void glDraw()
	{
		for ( ElementList::iterator elem = elements.begin();
				elem != elements.end(); ++elem )
		{
			elem->glDraw();
		}
	}

	void add( const Element& e )
	{
		if ( e.zoff() < 0 )
			return;

		elements.push_back( e );
		Element * pe = &elements.back();

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

private:

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

};

#endif // _WORLD_HPP_
