#ifndef _POINT_HPP_
#define _POINT_HPP_

struct Point
{

	double x, y, z;

	Point offset(double dx, double dy, double dz)
	{
		Point p = { x + dx, y + dy, z + dz };
		return p;
	}

};

#endif // _POINT_HPP_
