#pragma once

#include <gl\freeglut.h>

#include "MathCommon.h"
#include "Color.h"
#include "Texture.h"

/**
 * @class	Ground
 *
 * @brief	A ground object used to draw a circular ground in the XZ plane.
 */
class Ground
{
public:
	Ground(double radius);
	~Ground();

	/**
	 * @fn	void Ground::draw();
	 *
	 * @brief	Draws the ground as a circle in the XZ plane with the
	 * 			center at (0, 0, 0) and a radius specified at creation.
	 */
	void draw() const;

	// the simple color of the floor
	// if a texture is set, it should be black to prevent interfering with it
	Color color;

	// optionally, there may be a texture assigned to render with
	Texture* texture = NULL;
private:

	/**
	 * @fn	void Ground::drawVertex(double x, double y, double r);
	 *
	 * @brief	Draws single vertex as a part of the grid created to approximate circle.
	 *
	 * @param	x	The x coordinate on unit circle.
	 * @param	y	The y coordinate on unit circle.
	 * @param	r	Radius of the vertex.
	 */
	void drawVertex(double x, double y, double r) const;

	// radius of the circular floor
	double radius;

	// the resolution of the circle approximation
	int const SAMPLE_COUNT = 36;
	int const RINGS_COUNT = 36;
	// the sampling ratio of the texture - means how many times
	// to repeat the texture in each quadrant of the ground's circle
	// the value is inverted (e.g. 1.0/2.0 means it will be repeated 2x2 times)
	double const TEXTURE_SAMPLING = 1.0 / 2.0;
};

