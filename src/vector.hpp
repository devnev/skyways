#ifndef _POINT_HPP_
#define _POINT_HPP_

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
