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

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <display/shader.hpp>
#include "game.hpp"

Game::Game(
    double acceleration, double strafespeed, double speedlimit
  , double gravity, double jumpstrength
  , ShaderProgram * shader
)
	: _ship(), _map( 0 ), _shader( shader )
	, _currAcc( 0 ), _maxAcc( acceleration )
	, _currStrafe( 0 ), _maxStrafe( strafespeed )
	, _maxSpeed( speedlimit ), _zspeed( 0 )
	, _yapex( 0 ), _tapex( 0 ), _gravity( gravity )
	, _jstrength( jumpstrength ), _grounded( true )
{
	_ship.initialize();
	_ship.pos().x = 0.5;
	std::string empty;
	_blocks.insert( empty, new Block() );
}

void Game::startJump()
{
	if ( _grounded ||
		_map->collide( AABB(
			_ship.pos().offset(0, -0.2, 0),
			_ship.pos().offset(0, -0.2, 0).offset(_ship.size())
		) ) )
	{
		_yapex = _ship.ypos() + _jstrength;
		_tapex = sqrt( _jstrength / _gravity );
	}
}

void Game::draw( double zminClip )
{
	_shader->use();
	glPushMatrix();
	glTranslated( _ship.xpos() - 0.5, _ship.ypos(), 0.0 );
	glColor4f( 1, 0, 0, 0.25 );
	_ship.drawDl();
	glPopMatrix();
	glColor3f( 0.8f, 1, 1 );
	glTranslatef( -0.5, 0.0, _ship.zpos() );
	_map->glDraw( _ship.zpos() - zminClip );
}

void Game::update( int difference )
{
	double multiplier = ( (double)difference ) / 1000;

	Vector3 newPos = _ship.pos();

	AABB shipAabb(
		Vector3( -_ship.size().x/2, 0, 0 ),
		Vector3( _ship.size().x/2, _ship.size().y, _ship.size().z )
	);

	if ( _currAcc < -_maxAcc ) _currAcc = -_maxAcc;
	else if ( _currAcc > _maxAcc ) _currAcc = _maxAcc;
	_zspeed += _currAcc*multiplier;
	if ( _zspeed < 0 ) _zspeed = 0;
	if ( _zspeed > _maxSpeed ) _zspeed = _maxSpeed;

	newPos.z += multiplier * _zspeed;
	if ( _map->collide( shipAabb.offset( newPos ) ) )
	{
		newPos.z = _ship.pos().z;
		_zspeed = 0;
	}
	else
		_ship.pos().z = newPos.z;

	if ( _currStrafe < -_maxStrafe ) _currStrafe = -_maxStrafe;
	if ( _currStrafe > _maxStrafe ) _currStrafe = _maxStrafe;
	if ( _currStrafe != 0 )
	{
		newPos.x += _currStrafe*multiplier;
		if ( _map->collide( shipAabb.offset( newPos ) ) )
			newPos.x = _ship.pos().x;
		else
			_ship.pos().x = newPos.x;
	}

	if ( _tapex <= 0 && _grounded )
	{
		_tapex = 0;
		_yapex = _ship.pos().y;
	}

	_tapex -= multiplier;
	newPos.y = _yapex - _gravity * ( _tapex*_tapex );
	const Element * collision;
	if ( ( collision = _map->collide( shipAabb.offset( newPos ) ) ) )
	{
		newPos.y = _ship.pos().y;
		if (_tapex < 0) {
			_grounded = true;
			collision->trigger( *this );
		}
		_tapex = 0;
		_yapex = _ship.pos().y;
	}
	else
	{
		_ship.pos().y = newPos.y;
		_grounded = false;
	}

	if ( droppedOut() )
	{
		kill( "You dropped into the void!" );
	}

}

void Game::suicide()
{
	kill( "You committed suicide!" );
}

void Game::explode()
{
	kill( "You exploded!" );
}

void Game::kill( const std::string & cause )
{
	_death = cause;
}
