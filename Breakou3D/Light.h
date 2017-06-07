#pragma once

#include <gl/freeglut.h>

#include "Color.h"
#include "Point.h"
#include "Vector.h"

enum LightType { LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_SPOT };

class Light
{
public:
	Light(GLenum id);
	~Light();

	void bind();

	LightType light_type;
	Color color;
	Color specular;
	MathLib::Point position;
	MathLib::Vector direction;

	double spot_cutoff;
	double spot_exponent;
private:
	GLenum light_id;
};

