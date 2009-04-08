#ifndef _POINT_HPP_
#define _POINT_HPP_

struct Vector3
{

	double x, y, z;

	Vector3 offset(double dx, double dy, double dz) const throw()
	{
		Vector3 p = { x + dx, y + dy, z + dz };
		return p;
	}

	Vector3 offset(const Vector3& d) const throw()
	{
		Vector3 p = { x + d.x, y + d.y, z + d.z };
		return p;
	}

	static Vector3 make(double x, double y, double z)
	{
		Vector3 p = { x, y, z };
		return p;
	}

};

#endif // _POINT_HPP_
