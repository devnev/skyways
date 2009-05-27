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

#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include "block.hpp"
#include <boost/function.hpp>

class Game;

class Element
{
public:

	typedef boost::function1<void, Game&> TriggerFn;

	Element( double x, double y, double z, double l, Block * b, const Vector3& color, TriggerFn trigger = 0 )
		: _pos(Vector3( x,  y,  z)), _length( l ), _block( b ), _color( color ), _trigger( trigger )
	{
	}

	void glDraw();
	void trigger(Game& game) const { if (_trigger) _trigger(game); }

	const Vector3& pos() const throw() { return _pos; }
	double xoff() const throw() { return _pos.x; }
	double yoff() const throw() { return _pos.y; }
	double zoff() const throw() { return _pos.z; }
	double length() const throw() { return _length; }

	bool collide( const AABB& aabb ) const;

private:

	Vector3 _pos;
	double _length;
	Block * _block;
	Vector3 _color;
	TriggerFn _trigger;

};

#endif // _ELEMENT_HPP_
