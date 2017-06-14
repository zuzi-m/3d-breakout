#pragma once

#include "Point.h"
#include "Color.h"

class Ball
{
public:
	Ball(double radius);
	~Ball();

	void draw();

	MathLib::Point getPosition() const;
	void setPosition(MathLib::Point&);
	MathLib::Vector getVelocity() const;
	void setVelocity(MathLib::Vector&);
	double getRadius() const;
private:
	MathLib::Point position;
	MathLib::Vector velocity;
	double radius;
	Color color;
};

