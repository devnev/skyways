#ifndef _AABB_HPP_
#define _AABB_HPP_

struct AABB
{
	AABB()
		: p1(Vector3::make(0, 0, 0))
		, p2(Vector3::make(0, 0, 0))
	{
	}

	AABB(const Vector3& p1_, const Vector3& p2_)
		: p1(p1_), p2(p2_)
	{
	}

	Vector3 size() const throw()
	{
		return Vector3::make(p2.x-p1.x, p2.y-p1.y, p2.z-p1.z);
	}

	bool collide(const AABB& other) const throw()
	{
		int c = 0;
		c |= (p1.x > other.p1.x && p1.x < other.p2.x) ? 0x01 : 0;
		c |= (p1.y > other.p1.y && p1.y < other.p2.y) ? 0x02 : 0;
		c |= (p1.z > other.p1.z && p1.z < other.p2.z) ? 0x04 : 0;
		c |= (p2.x > other.p1.x && p2.x < other.p2.x) ? 0x01 : 0;
		c |= (p2.y > other.p1.y && p2.y < other.p2.y) ? 0x02 : 0;
		c |= (p2.z > other.p1.z && p2.z < other.p2.z) ? 0x04 : 0;
		return c == 0x07;
	}

	Vector3 p1, p2;
};

#endif // _AABB_HPP_
