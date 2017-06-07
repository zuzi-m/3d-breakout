#include "Point.h"

namespace MathLib {

	Point::Point()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Point::Point(const Point &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	Point::Point(const Vector &v)
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	Point::Point(double val)
	{
		x = val;
		y = val;
		z = val;
	}

	Point::Point(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	Point::~Point()
	{
	}

	Vector Point::vector(const Point &p1, const Point &p2)
	{
		Vector v1(p1[0], p1[1], p1[2]);
		Vector v2(p2[0], p2[1], p2[2]);

		return v2.sub(v1);
	}

	Vector Point::vector() const
	{
		return Vector(x, y, z);
	}

	double Point::X() const
	{
		return x;
	}

	double Point::Y() const
	{
		return y;
	}

	double Point::Z() const
	{
		return z;
	}

	Point Point::add(const Vector &v) const
	{
		return Point(x + v[0], y + v[1], z + v[2]);
	}

	double Point::operator[](const int i) const
	{
		switch (i % 3)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
		return 0.0;
	}

	void Point::toArray(double *arr) const
	{
		*arr = x;
		arr++;
		*arr = y;
		arr++;
		*arr = z;
	}

	void Point::toArray(float *arr) const
	{
		*arr = static_cast<float>(x);
		arr++;
		*arr = static_cast<float>(y);
		arr++;
		*arr = static_cast<float>(z);
	}

	std::ostream & operator<<(std::ostream &os, const Point &p)
	{
		os << p.x << " " << p.y << " " << p.z;
		return os;
	}

}
