#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <boost/ptr_container/ptr_map.hpp>
#include "block.hpp"

class Config
{
public:

	void readBlocks( /*out*/ boost::ptr_map< std::string, Block >& blockMap );

};

#endif // _CONFIG_HPP_
