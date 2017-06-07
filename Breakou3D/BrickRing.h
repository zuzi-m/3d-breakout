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
	void tick(long milis, Ball* ball);
private:
	int brick_count;
	int brick_levels;
	double radius;
	double brick_width;
	double brick_height;

	Brick*** bricks;
};

