#pragma once

#include "Point.h"
#include "Matrix.h"
#include "Ball.h"

using namespace MathLib;

class Quad
{
public:
	Quad(Point&, Point&, Point&, Point&);
	Quad(const Quad&);
	~Quad();

	void draw();

	void trans(Matrix&);
	void translate(Vector&);

	double getDistance(const Ball*) const;

	Vector getNormal() const;
	const Point& operator[](const int) const;
private:
	Point points[4];
	Vector normal;

	void updateNormal();
};

