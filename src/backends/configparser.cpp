//
// This file is part of the game Skyways.
// Copyright (C) 2009 Mark Nevill
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include <iostream>
#include "configparser.hpp"

ConfigParser::ConfigParser( Configuration& config )
	: _config( config )
{
	namespace po = boost::program_options;
	options.add_options()
		("help,h", "print help message")
		("map,w",
		 po::value<std::string>()->default_value(std::string()),
		 "set map to load")
		("ship,s",
		 po::value<std::string>()->default_value(std::string()),
		 "set ship model to use")
	;
}

bool ConfigParser::args(int argc, char * argv[])
{
	namespace po = boost::program_options;
	po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
	po::notify(vm);
	if (vm.count("help"))
	{
		std::cout << options << '\n';
		return false;
	}
	_config.map = vm["map"].as<std::string>();
	_config.ship = vm["ship"].as<std::string>();
	return true;
}
