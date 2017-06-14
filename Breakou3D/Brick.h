#pragma once

#include <vector>

#include "Ball.h"
#include "Color.h"
#include "Texture.h"
#include "Quad.h"

class Brick
{
public:
	~Brick();

	/**
	 * @fn	static Brick* Brick::unit(int num_segments, double radius, double thickness, double height);
	 *
	 * @brief	Create a "unit" brick with given parameters, sitting on the XZ plane
	 * 			with its middle at the zero of the X axis. The created brick can be
	 * 			transformed then to arbitrary position.
	 *
	 * @param	num_segments	Number of bricks that would make up the whole ring. Controlls the size of the brick.
	 * @param	radius			The radius of the brick - its distance from the center.
	 * @param	thickness   	The thickness of the brick.
	 * @param	height			The height of the brick.
	 *
	 * @return	A pointer to a dynamically allocated Brick object.
	 */
	static Brick* unit(int num_segments, double radius, double thickness, double height);

	void draw();

	void trans(Matrix&);
	void translate(Vector&);

	void setColor(Color&);
	void setTexture(Texture*);

	double getTop();
	double getBottom();

	Vector velocity;

	int numQuads() const;
	const Quad& operator[](const int) const;
private:
	Brick();

	std::vector<Quad> quads;
};

