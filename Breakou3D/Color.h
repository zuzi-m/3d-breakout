#pragma once

#include "Vector.h"

class Color
{
public:
	Color();
	Color(double grayscale);
	Color(double red, double green, double blue);
	Color(double red, double green, double blue, double alpha);
	Color(MathLib::Vector& v);
	~Color();

	static Color randomColor(double alpha);
	static Color hsv(int h, double s, double v, double alpha);

	void bind();

	double operator[](const int) const;
	friend std::ostream& operator<<(std::ostream&, const Color&);
	void toArray(double*, bool) const;
	void toArray(float*, bool) const;
private:
	void clamp();

	double r;
	double g;
	double b;
	double a;
};

