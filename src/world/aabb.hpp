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

#ifndef _AABB_HPP_
#define _AABB_HPP_

struct AABB
{
	AABB()
		: p1(Vector3(0, 0, 0))
		, p2(Vector3(0, 0, 0))
	{
	}

	AABB(const Vector3& p1_, const Vector3& p2_)
		: p1(p1_), p2(p2_)
	{
	}

	AABB offset( const Vector3& d ) const throw()
	{
		return AABB(p1.offset(d), p2.offset(d));
	}

	AABB offset( double x, double y, double z ) const throw()
	{
		return AABB(p1.offset( x, y, z ), p2.offset( x, y, z ));
	}

	Vector3 size() const throw()
	{
		return Vector3(p2.x-p1.x, p2.y-p1.y, p2.z-p1.z);
	}

	bool collide(const AABB& other) const throw()
	{
		int c = 0;
		c |= ((p1.x > other.p1.x && p1.x < other.p2.x) ? 0x01 : 0);
		c |= ((p1.y > other.p1.y && p1.y < other.p2.y) ? 0x02 : 0);
		c |= ((p1.z > other.p1.z && p1.z < other.p2.z) ? 0x04 : 0);
		c |= ((p2.x > other.p1.x && p2.x < other.p2.x) ? 0x01 : 0);
		c |= ((p2.y > other.p1.y && p2.y < other.p2.y) ? 0x02 : 0);
		c |= ((p2.z > other.p1.z && p2.z < other.p2.z) ? 0x04 : 0);
		return c == 0x07;
	}

	Vector3 p1, p2;
};

#endif // _AABB_HPP_
