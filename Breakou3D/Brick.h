#pragma once

#include <vector>

#include "Quad.h"
#include "Color.h"
#include "Ball.h"

class Brick
{
public:
	Brick();
	~Brick();

	static Brick* unit(int num_segments, double radius, double thickness, double height);

	void draw();

	void trans(Matrix&);
	void translate(Vector&);

	double getTop();
	double getBottom();
	double getDistance(const Ball*) const;

	Color color;
	Vector velocity;
private:
	std::vector<Quad> quads;
};

