#pragma once

#include <iostream>
#include "Vector.h"

namespace MathLib {

	class Point
	{
	public:
		Point();
		Point(const Point&);
		Point(const Vector&);
		Point(double);
		Point(double, double, double);
		~Point();

		static Vector vector(const Point&, const Point&);
		Vector vector() const;

		double X() const;
		double Y() const;
		double Z() const;

		Point add(const Vector&) const;

		friend std::ostream& operator<<(std::ostream&, const Point&);
		double operator[](const int) const;
		void toArray(double*) const;
		void toArray(float*) const;
	private:
		double x;
		double y;
		double z;
	};

}
