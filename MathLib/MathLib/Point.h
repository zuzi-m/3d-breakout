#pragma once

namespace MathLib {

	class Point
	{
	public:
		Point();
		Point(Point&);
		Point(double);
		Point(double, double, double);
		~Point();



	private:
		double x;
		double y;
		double z;
	};

}
