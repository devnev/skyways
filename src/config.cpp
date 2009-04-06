#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fstream>
#include <string>
#include "config.hpp"

using namespace boost::filesystem;

void Config::readBlocks( boost::ptr_map< std::string, Block >& blockMap )
{
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
		blockMap.insert( block_path, Block::fromStream( block_file ) );
	}
}
