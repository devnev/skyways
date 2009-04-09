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
