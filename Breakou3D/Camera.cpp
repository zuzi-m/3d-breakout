#include "Camera.h"
#include "Matrix.h"
#include "MathCommon.h"

#include <gl\freeglut.h>
#include <iostream>
#include <algorithm>

const double Camera::MIN_HORIZONTAL_DEG = 0.5;
const double Camera::MAX_HORIZONTAL_DEG = 90.0;
const double Camera::MIN_RADIUS = 5.0;
const double Camera::MAX_RADIUS = 250.0;

Camera::Camera(double radius) : position(0.0), target(0)
{
	this->radius = radius;
	verticalDeg = 45.0;
	horizontalDeg = 90.0;
	updatePosition();
}


Camera::~Camera()
{
}

void Camera::setCamera()
{
	gluLookAt(position[0], position[1], position[2], target[0], target[1], target[2], 0.0, 1.0, 0.0);
}

void Camera::moveCamera(double horizontal, double vertical)
{
	verticalDeg += vertical;
	horizontalDeg += horizontal;
	updatePosition();
}

void Camera::zoom(double zoomAmount)
{
	radius += zoomAmount;
	updatePosition();
}

void Camera::updatePosition()
{
	// constrain rotations in some reasonable bounds
	verticalDeg = std::min(std::max(verticalDeg, MIN_HORIZONTAL_DEG), MAX_HORIZONTAL_DEG);
	if (horizontalDeg > 360) {
		horizontalDeg -= 360.0;
	}
	if (horizontalDeg < 0) {
		horizontalDeg += 360.0;
	}
	// constrain zoom as well
	radius = std::min(std::max(radius, MIN_RADIUS), MAX_RADIUS);

	// start with some initial camera position
	MathLib::Vector v = MathLib::Vector(radius, 0, 0);

	// rotate around Z axis to set camera in vertical direction
	MathLib::Matrix rot = MathLib::Matrix::rotation(2, verticalDeg * MathLib::PI / 180.0);
	v = rot.mult(v);

	// then, rotate around Y axis to set camera in horizontal direction
	rot = MathLib::Matrix::rotation(1, horizontalDeg * MathLib::PI / 180.0);
	v = rot.mult(v);

	// update the resulting position of camera
	position = MathLib::Point(v[0], v[1], v[2]);
}
