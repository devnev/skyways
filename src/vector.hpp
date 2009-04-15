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

#ifndef _POINT_HPP_
#define _POINT_HPP_

#include <cstddef>

struct Vector3
{

	Vector3()
		: x( 0 ), y( 0 ), z( 0 )
	{
	}

	Vector3(double x_, double y_, double z_)
		: x( x_ ), y( y_ ), z( z_ )
	{
	}

	Vector3 offset(double dx, double dy, double dz) const throw()
	{
		return Vector3( x + dx, y + dy, z + dz );
	}

	Vector3 offset(const Vector3& d) const throw()
	{
		return offset( d.x, d.y, d.z );
	}

	double x, y, z;

};

#endif // _POINT_HPP_
