#include <algorithm>
#include <functional>
#include "config.hpp"
#include "world.hpp"

World::World( size_t sectionSize )
	: sectionSize( sectionSize )
{
}

void World::glDraw( double zmin )
{
	_elementsDrawn = 0;
	if (sections.size() == 0)
	{
		for ( ElementList::iterator elem = elements.begin() ;
				elem != elements.end(); ++elem )
		{
			elem->glDraw();
		}
		_elementsDrawn += elements.size();
	}
	else
	{
		size_t z;
		if ( zmin < 0 )
			z = 0;
		else
			z = ( (size_t)zmin ) / sectionSize;
		if ( z >= sections.size() )
			return;
		std::for_each(
			sections[ z ].running.begin(),
			sections[ z ].running.end(),
			std::mem_fun( &Element::glDraw )
		);
		_elementsDrawn += sections[ z ].running.size();
		std::for_each(
			sections[ z ].ending.begin(),
			sections[ z ].ending.end(),
			std::mem_fun( &Element::glDraw )
		);
		_elementsDrawn += sections[ z ].ending.size();
		for ( ; z < sections.size() ; ++z )
		{
			std::for_each(
				sections[ z ].beginning.begin(),
				sections[ z ].beginning.end(),
				std::mem_fun( &Element::glDraw )
			);
			_elementsDrawn += sections[ z ].beginning.size();
		}
	}
}

void World::loadBlocks()
{
	Config config;
	config.readBlocks( blocks );
}

void World::add( const Element& e )
{
	if ( e.zoff() < 0 )
		return;

	sections.clear();
	elements.push_back( e );
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
