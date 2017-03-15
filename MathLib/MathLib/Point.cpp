#include "Point.h"

namespace MathLib {

	Point::Point()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Point::Point(Point &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
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

}
