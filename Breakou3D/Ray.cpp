#include "Ray.h"

namespace MathLib {

	Ray::Ray()
	{
	}

	Ray::Ray(const Point &p, const Vector &v)
	{
		start = p;
		direction = v.normalize();
	}


	Ray::~Ray()
	{
	}

	double Ray::distance(const Ray &other)
	{
		Vector n = direction.cross(other.direction);
		Vector P = Point::vector(start, other.start);

		if (n.isZero()) {
			return distance(other.start);
		}

		return abs(P.dot(n)/n.length());
	}

	double Ray::distance(const Point &p)
	{
		// get the vector from "start" to "p"
		Vector P = Point::vector(start, p);
		// project the vector P to "direction" of this ray
		Vector a1 = direction.project(P);
		// get the point "x" which is the closest point to p on this line
		Point x = start.add(a1);
		// get the length of vector "px"
		return Point::vector(x, p).length();
	}

}
