#pragma once

#include "Point.h"
#include "Vector.h"

namespace MathLib {

	class Ray
	{
	public:
		Ray();
		~Ray();

	private:
		Point origin;
		Vector direction;
	};

}
