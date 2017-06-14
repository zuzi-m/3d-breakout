#include "Ground.h"

#include <SOIL.h>
#include <cmath>

Ground::Ground(double radius) : color(1.0)
{
	// set the radius
	this->radius = radius;
}


Ground::~Ground()
{
}

void Ground::drawVertex(double x, double y, double r) const
{
	if (texture != NULL) {
		// set the texture coordinates for the vertex - similar to vertex positions, just
		// stretched by TEXTURE_SAMPLING
		glTexCoord2d((r * -x) / (radius*TEXTURE_SAMPLING), (r * y) / (radius*TEXTURE_SAMPLING));
	}

	// the same normal for the whole floor - floor is horizontal
	glNormal3d(0.0, 1.0, 0.0);

	// the actual position of the vertex
	glVertex3d(r * -x, 0.0, r * y);
}

void Ground::draw() const
{
	// set the color and texture to opengl
	color.bind();
	if (texture != NULL) {
		texture->bind();
	}

	// compute variables needed to draw rings
	double angleStep = (2.0 * MathLib::PI) / SAMPLE_COUNT;
	double prevR = -1.0;
	// draw all rings one by one
	for (int ring = 0; ring < RINGS_COUNT; ring++) {
		// get the radius of the current ring
		double r = (radius / RINGS_COUNT)*(ring + 1);
		
		// the first ring is special, because it does not have hole in it
		if (ring == 0) {
			glBegin(GL_POLYGON);
		} else {
			glBegin(GL_QUADS);
		}

		// draw the ring by approximating circle with segments
		for (int i = 0; i < SAMPLE_COUNT; i++) {
			// compute positions of the segment vertices using polar coordinates
			double angle1 = i*angleStep;
			double angle2 = (i+1)*angleStep;
			double x1 = cos(angle1);
			double y1 = sin(angle1);
			double x2 = cos(angle2);
			double y2 = sin(angle2);

			// draw the vertices on appropriate positions
			if (ring == 0) {
				drawVertex(x1, y1, r);
			}
			else {
				// for rings with hole draw full quad per segment
				drawVertex(x2, y2, prevR);
				drawVertex(x1, y1, prevR);
				drawVertex(x1, y1, r);
				drawVertex(x2, y2, r);
			}
		}

		prevR = r;
		glEnd();
	}

	// reset the texture setting
	if (texture != NULL) {
		texture->unbind();
	}
}
