#ifndef _POINT_HPP_
#define _POINT_HPP_

struct Point
{

	double x, y, z;

	Point offset(double dx, double dy, double dz) const throw()
	{
		Point p = { x + dx, y + dy, z + dz };
		return p;
	}

	Point offset(const Point& d) const throw()
	{
		Point p = { x + d.x, y + d.y, z + d.z };
		return p;
	}

	static Point make(double x, double y, double z)
	{
		Point p = { x, y, z };
		return p;
	}

};

#endif // _POINT_HPP_
