#pragma once

#include "Point.h"
#include "Matrix.h"
#include "Ball.h"
#include "Texture.h"

using namespace MathLib;

class Quad
{
public:
	Quad(Point&, Point&, Point&, Point&);
	Quad(const Quad&);
	~Quad();

	void draw() const;

	void trans(Matrix&);
	void translate(Vector&);

	Vector getNormal() const;
	const Point& operator[](const int) const;

	Color color;
	Texture* texture = NULL;
private:
	Point points[4];
	Vector normal;

	void updateNormal();
};

