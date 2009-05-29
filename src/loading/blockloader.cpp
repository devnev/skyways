#include <sstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "objmodel.hpp"
#include "blockloader.hpp"

void BlockLoader::loadDirectory( const char * directory, boost::ptr_map< std::string, Block >& blocks )
{
	boost::ptr_map< std::string, Block > newBlocks;

	using namespace boost::filesystem;
	path block_dir( directory );
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
		++iter;
		if ( iter != block_fs_entry->path().end() )
			block_path = *iter;
		for ( ++iter ; iter != block_fs_entry->path().end() ; ++iter )
			block_path = block_path + "/" + *iter;
		newBlocks.insert( block_path, load( block_fs_entry->path().string() ) );
	}

	blocks.transfer( newBlocks );
}

std::auto_ptr< Block > BlockLoader::load( const std::string& filename )
{
	ObjUnknownsList objunknowns;
	std::auto_ptr< Model > model( new Model() );
	loadObjModel( filename.c_str(), *model, false, &objunknowns, "b\0" );
	std::vector< AABB > bounds;

	for ( ObjUnknownsList::iterator unknown = objunknowns.begin();
			unknown != objunknowns.end(); ++unknown )
	{
		if ( unknown->first == "b" )
		{
			std::istringstream iss( unknown->second );
			std::string cmd;
			iss >> cmd;

			AABB aabb;
			iss >> aabb.p1.x >> aabb.p1.y >> aabb.p1.z
				>> aabb.p2.x >> aabb.p2.y >> aabb.p2.z;
			bounds.push_back( aabb );
		}
	}
	return std::auto_ptr< Block >( new Block( model, bounds ) );
}

