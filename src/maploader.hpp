
#ifndef _MAPLOADER_HPP_
#define _MAPLOADER_HPP_

#include <memory>
#include <istream>
#include <boost/ptr_container/ptr_map.hpp>
#include "vector.hpp"
#include "map.hpp"

struct MapInfo
{
	std::auto_ptr< Map > map;
	Vector3 startPos;
};

class MapLoader
{

public:

	MapLoader( const boost::ptr_map< std::string, Block > * blocks )
		: _blocks( blocks )
	{
	}

	void loadMap( std::istream& is, MapInfo& mapinfo );

private:

	const boost::ptr_map< std::string, Block > * _blocks;

};

#endif // _MAPLOADER_HPP_
