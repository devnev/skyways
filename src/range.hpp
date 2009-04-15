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

#ifndef _RANGE_HPP_
#define _RANGE_HPP_

#include <limits>
#include <algorithm>

/// Represents a range of numbers for some numeric type T.
template<class T>
struct Range
{
	T min, max;
	/// Is the range valid, i.e. min <= max?
	bool valid() const throw() { return min <= max; }
	/// Is a value contained in the range?
	bool contains(const T& value) const throw() { return (min <= value) && (value <= max); }
	/// Do two ranges overlap?
	bool overlaps(const Range& range) const throw() { return max >= range.min && min <= range.max; }
	/// Extend range to include another range.
	void merge(const Range& other)
	{
		using std::min;
		using std::max;
		this->min = min(this->min, other.min);
		this->max = max(this->max, other.max);
	}
	/// Extend range to include a point.
	void include(const T& value)
	{
		using std::min;
		using std::max;
		this->min = min(this->min, value);
		this->max = max(this->max, value);
	}
	/// Distance between min and max
	T difference()
	{
		return max - min;
	}
	/// Center between min and max
	T center()
	{
		return (max + min) / 2;
	}
};

/// Basic inline range constructor.
/// @relatesalso Range
template<class T>
inline Range<T> makeRange(const T& min, const T& max)
{
	Range<T> r = { min, max };
	return r;
}

/// Construct range containing all points.
/// @relatesalso Range
template<class T>
inline Range<T> makeMaxRange()
{
	using std::numeric_limits;
	Range<T> r = { numeric_limits<T>::min(), numeric_limits<T>::max() };
	return r;
}

/// Construct reverse of maximum range, i.e. maximize range.min and minimize range.max.
/// @relatesalso Range
template<class T>
inline Range<T> makeInvalidRange()
{
	using std::numeric_limits;
	Range<T> r = { numeric_limits<T>::max(), numeric_limits<T>::min() };
	return r;
}

/// @relatesalso Range
template<class OStream, class T>
OStream& operator<<(OStream& os, const Range<T>& range)
{
	return os << "(range: " << range.min << ' ' << range.max << ')';
}

#endif
