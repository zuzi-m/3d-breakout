#pragma once

#include <math.h>
#include <iostream>
#include "MathCommon.h"

namespace MathLib {

	class Vector
	{
	public:
		Vector();
		Vector(const Vector&);
		Vector(double);
		Vector(double, double, double);
		~Vector();

		double U() const;
		double V() const;
		double W() const;
		void toArray(double[]);

		double length() const;
		double lengthSquared() const;
		static double angle(const Vector&, const Vector&);
		bool isZero() const;
		bool isUnit() const;
		Vector normalize() const;

		double dot(const Vector&) const;
		Vector cross(const Vector&) const;

		Vector add(const Vector&) const;
		Vector sub(const Vector&) const;
		Vector opposite() const;
		Vector multiply(double) const;
		Vector multiply(const Vector&) const;
		Vector project(const Vector&) const;

		double operator[](const int) const;
		friend std::ostream& operator<<(std::ostream&, const Vector&);
		void toArray(double*) const;
		void toArray(float*) const;
	private:
		double u;
		double v;
		double w;
	};

}
