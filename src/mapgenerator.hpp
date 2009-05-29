
#include "maploader.hpp"

class MapGenerator
{

public:

	MapGenerator( const boost::ptr_map< std::string, Block > * blocks )
		: _blocks( blocks )
	{
	}

	void generate( MapInfo& mapinfo );

private:

	const boost::ptr_map< std::string, Block > * _blocks;

};
