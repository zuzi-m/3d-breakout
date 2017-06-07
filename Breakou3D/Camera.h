#pragma once

#include "Point.h"

class Camera
{
public:
	Camera(double);
	~Camera();

	void setCamera();

	void moveCamera(double, double);
	void zoom(double);
private:
	void updatePosition();

	MathLib::Point position;
	MathLib::Point target;

	double radius;
	double verticalDeg;
	double horizontalDeg;

	// static constants, defined at the start of CPP file
	static const double MIN_HORIZONTAL_DEG;
	static const double MAX_HORIZONTAL_DEG;
	static const double MIN_RADIUS;
	static const double MAX_RADIUS;
};

