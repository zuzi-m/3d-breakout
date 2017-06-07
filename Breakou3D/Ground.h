#pragma once

#include <gl\freeglut.h>

#include "MathCommon.h"
#include "Color.h"

class Ground
{
public:
	Ground(double radius);
	~Ground();

	void draw();
private:
	double radius;
	Color color;
	GLuint tex_id;

	int const SAMPLE_COUNT = 36;
	int const RINGS_COUNT = 100;
};

