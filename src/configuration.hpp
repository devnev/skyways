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

#ifndef _CONFIGURATION_HPP_
#define _CONFIGURATION_HPP_

#include <memory>
#include "controller.hpp"

class Configuration
{

public:

	Configuration();

	void setWorld( const std::string& world ) { _world = world; }

	std::auto_ptr< Controller > buildController( Controller::QuitCallback cbquit );

private:

	std::string _world;

};

#endif // _CONFIGURATION_HPP_
