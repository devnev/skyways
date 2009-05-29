
#include "mapgenerator.hpp"

void MapGenerator::generate( MapInfo& mapinfo )
{
	std::vector< Element > elements;
	elements.clear();
	elements.push_back(Element(
		0, -1, 0, 20, &_blocks->at( "" ), Vector3( 1, 1, 1 )
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
			&_blocks->at( "" ),
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
	mapinfo.map.reset( new Map( 10, elements ) );
	mapinfo.startPos = Vector3(0.5, 0, 0);
}
