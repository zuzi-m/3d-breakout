#include "Ground.h"

#include <SOIL.h>
#include <cmath>

Ground::Ground(double radius) : color(1.0)
{
	this->radius = radius;
	this->tex_id = SOIL_load_OGL_texture("carpet.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS|SOIL_FLAG_MIPMAPS);
	if (tex_id == 0) {
		std::cout << "tex err: " << SOIL_last_result() << std::endl;
	} else {
		glBindTexture(GL_TEXTURE_2D, tex_id);
	}
}


Ground::~Ground()
{
}

void Ground::draw()
{
	color.bind();
	glBindTexture(GL_TEXTURE_2D, tex_id);

	double prevR = -1.0;
	for (int ring = 0; ring < RINGS_COUNT; ring++) {
		double r = (radius / RINGS_COUNT)*(ring + 1);
		if (ring == 0) {
			glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(0.0, 0.0);
			glNormal3d(0.0, 1.0, 0.0);
			glVertex3d(0.0, 0.0, 0.0);
		} else {
			glBegin(GL_TRIANGLE_STRIP);
		}
		for (int i = 0; i < SAMPLE_COUNT+1; i++) {
			double angle = (i*(2.0 * MathLib::PI)) / SAMPLE_COUNT;
			double x = cos(angle);
			double y = sin(angle);

			if (prevR > 0.0) {
				glTexCoord2d((prevR * -x) / (radius*0.222), (r * y) / (radius*0.222));
				glNormal3d(0.0, 1.0, 0.0);
				glVertex3d(prevR * -x, 0.0, prevR * y);
			}
			glTexCoord2d((r * -x) / (radius*0.222), (r * y) / (radius*0.222));
			glNormal3d(0.0, 1.0, 0.0);
			glVertex3d(r * -x, 0.0, r * y);
		}
		prevR = r;
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
