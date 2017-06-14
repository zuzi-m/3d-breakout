#pragma once

#include "Brick.h"
#include "Ball.h"

class BrickRing
{
public:
	BrickRing(int count, int levels, double radius, double width, double height, int hue);
	~BrickRing();

	void draw() const;

	void removeRandom();

	double tick(double dt, Ball* ball);

	int bricksLeft() const;
private:
	bool removeBrick(int level, int position);

	int activeBricks;
	int brick_count;
	int brick_levels;
	double radius;
	double brick_width;
	double brick_height;

	Brick*** bricks;
};

