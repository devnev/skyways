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

#include <display/textprinter.hpp>
#include <loading/objmodel.hpp>
#include <iostream>
#include <stdexcept>
#include "controller.hpp"
#include "configuration.hpp"

std::auto_ptr< Controller > Configuration::buildController( Controller::QuitCallback cbquit )
{
	std::auto_ptr< TextPrinter > printer( new TextPrinter( "DejaVuSans.ttf" ) );
	std::auto_ptr< Model > shipModel;
	if ( ship.length() > 0 )
	{
		shipModel.reset( new Model() );
		loadObjModel( ship.c_str(), *shipModel, true, 0, "mtl\0" );
	}
	std::auto_ptr< Ship > ship( new Ship( shipModel ) );
	std::auto_ptr< Game > game( new Game(
		10, 5, 100, 20, 1.5, ship
	) );
	std::auto_ptr< Controller > controller( new Controller (
		game, 3.5, 6, 10, cbquit, printer
	) );
	if ( map.length() )
		controller->loadMap( map );
	else
		controller->generateMap();
	return controller;
}
