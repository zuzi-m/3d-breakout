#pragma once

#include "Point.h"
#include "Vector.h"

namespace MathLib {

	class Ray
	{
	public:
		Ray();
		Ray(const Point&, const Vector&);
		~Ray();

		double distance(const Ray&);
		double distance(const Point&);

		Point start;
		Vector direction;
	};

}

