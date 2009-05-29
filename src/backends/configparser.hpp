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

#ifndef _CONFIGPARSER_HPP_
#define _CONFIGPARSER_HPP_

#ifdef _QTCONFIGPARSER_HPP_
#error "Invalid build: cannot include qtconfigparser.hpp and configparser.hpp"
#endif // _QTCONFIGPARSER_HPP_

#include <boost/program_options.hpp>
#include <configuration.hpp>

class ConfigParser
{

public:

	ConfigParser( Configuration& config );

	bool args(int argc, char * argv[]);

private:

	Configuration& _config;

	boost::program_options::options_description options;
	boost::program_options::variables_map vm;

};

#endif // _CONFIGPARSER_HPP_
