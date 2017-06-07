#include "Light.h"

#include <iostream>

Light::Light(GLenum id) : color(1.0), specular(1.0)
{
	light_id = id;
	light_type = LIGHT_POINT;
}


Light::~Light()
{
}

void Light::bind()
{
	float vals[4];
	float w = 1.0;
	position.toArray(vals);
	if (light_type == LIGHT_DIRECTIONAL) {
		w = 0.0;
		direction.toArray(vals);
	}
	// set the position of the light
	vals[3] = w;
	glLightfv(light_id, GL_POSITION, vals);

	// set color of the light
	color.toArray(vals, false);
	glLightfv(light_id, GL_AMBIENT, vals);
	glLightfv(light_id, GL_DIFFUSE, vals);
	specular.toArray(vals, false);
	glLightfv(light_id, GL_SPECULAR, vals);

	// set special parameters
	switch (light_type) {
	case LIGHT_POINT:
		break;
	case LIGHT_SPOT:
		direction.toArray(vals);
		glLightfv(light_id, GL_SPOT_DIRECTION, vals);
		glLightf(light_id, GL_SPOT_CUTOFF, static_cast<float>(spot_cutoff));
		glLightf(light_id, GL_SPOT_EXPONENT, static_cast<float>(spot_exponent));
		break;
	case LIGHT_DIRECTIONAL:
		break;
	}
	
	glEnable(light_id);
}
