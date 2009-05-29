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

#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include <world/ship.hpp>
#include <world/map.hpp>

class ShaderProgram;

class Game
{

public:

	Game(
	    double acceleration, double strafespeed, double speedlimit
	  , double gravity, double jumpstrength
	  , ShaderProgram * shader = 0
	);

	void addBlocks(boost::ptr_map< std::string, Block >& blocks) { _blocks.transfer( blocks ); }
	void setMap(std::auto_ptr<Map> map) throw() { _map = map; _map->optimize(); }
	void setPos(const Vector3& pos) throw() { _ship.pos() = pos; }
	void setShader(ShaderProgram& shader) throw() { _shader = &shader; }
	const boost::ptr_map< std::string, Block >& getBlocks() { return _blocks; }

	void setAcceleration( double accel ) { _currAcc = accel*_maxAcc; }
	void setStrafe( double strafe ) { _currStrafe = strafe*_maxStrafe; }
	void startJump();

	void draw( double zminClip );
	void update( int difference );

	double distanceTraveled() const throw() { return _ship.pos().z; }
	bool droppedOut() const throw() {
		return _ship.pos().y < _map->lowestPoint() - 1;
	}

	void kill( const std::string & cause );
	void suicide();
	void explode();
	bool dead() const throw() { return !_death.empty(); }
	const std::string& deathCause() const { return _death; }

private:

	boost::ptr_map< std::string, Block > _blocks;
	Ship _ship;
	std::auto_ptr< Map > _map;
	ShaderProgram * _shader;
	double _currAcc, _maxAcc;
	double _currStrafe, _maxStrafe;
	double _maxSpeed, _zspeed;
	double _yapex, _tapex, _gravity;
	double _jstrength;
	bool _grounded;
	std::string _death;
};

#endif // _WORLD_HPP_
