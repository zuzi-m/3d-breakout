#pragma once

#include "Point.h"
#include "Color.h"

class Ball
{
public:
	Ball(double);
	~Ball();

	void draw();
	void move(long);

	MathLib::Point getPosition() const;
	void setPosition(MathLib::Point&);
	MathLib::Vector getVelocity() const;
	void setVelocity(MathLib::Vector&);
private:
	MathLib::Point position;
	MathLib::Vector velocity;
	double radius;
	Color color;
};

