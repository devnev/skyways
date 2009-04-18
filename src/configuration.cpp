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

#include "textprinter.hpp"
#include "controller.hpp"
#include "configuration.hpp"

Configuration::Configuration()
{
}

std::auto_ptr< Controller > Configuration::buildController( Controller::QuitCallback cbquit )
{
	std::auto_ptr< TextPrinter > printer( new TextPrinter( "DejaVuSans.ttf" ) );
	std::auto_ptr< Controller > controller( new Controller (
		10, 5, 100, 20, 1.5, 3, 6,
		cbquit, printer
	) );
	if ( _world.length() )
		controller->loadWorld( _world );
	else
		controller->generateWorld();
	return controller;
}
