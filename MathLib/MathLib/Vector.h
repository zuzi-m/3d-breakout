#pragma once

#include <math.h>
#include "Common.h"

namespace MathLib {

	class Vector
	{
	public:
		Vector();
		Vector(Vector&);
		Vector(double);
		Vector(double, double, double);
		~Vector();

		double U() const;
		double V() const;
		double W() const;

		double length() const;
		double lengthSquared() const;
		bool isZero() const;
		bool isUnit() const;
		Vector normalize() const;

		double dot(Vector&) const;
		Vector cross(Vector&) const;

		Vector add(Vector&) const;
		Vector sub(Vector&) const;
		Vector opposite() const;
		Vector multiply(double) const;
	private:
		double u;
		double v;
		double w;
	};

}
