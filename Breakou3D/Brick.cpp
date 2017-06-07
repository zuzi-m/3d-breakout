#include "Brick.h"

#include <algorithm>

Brick::Brick()
{
	color = Color::randomColor(1.0);
}


Brick::~Brick()
{
}

Brick* Brick::unit(int num_segments, double radius, double thickness, double height)
{
	Point zero(0.0); // needed to compute things
	double step = (2 * MathLib::PI) / (2 * num_segments); // needed to compute things
	// first row of points
	Point mid(cos(0)*radius, 0, sin(0)*radius);
	Point left(cos(-step)*radius, 0, sin(-step)*radius);
	Point right(cos(step)*radius, 0, sin(step)*radius);
	// second row of points
	Point mid2 = mid.add(Point::vector(zero, mid).normalize().multiply(thickness));
	Point left2 = left.add(Point::vector(zero, left).normalize().multiply(thickness));
	Point right2 = right.add(Point::vector(zero, right).normalize().multiply(thickness));
	// vector to be added to points to create upper rows
	Vector up(0, height, 0);
	Brick* b = new Brick();

	// firont vertical wall
	b->quads.push_back(Quad(mid, right, right.add(up), mid.add(up)));
	b->quads.push_back(Quad(left, mid, mid.add(up), left.add(up)));
	// back vertical wall
	b->quads.push_back(Quad(right2, mid2, mid2.add(up), right2.add(up)));
	b->quads.push_back(Quad(mid2, left2, left2.add(up), mid2.add(up)));
	// top wall
	b->quads.push_back(Quad(mid.add(up), right.add(up), right2.add(up), mid2.add(up)));
	b->quads.push_back(Quad(left.add(up), mid.add(up), mid2.add(up), left2.add(up)));
	// sides
	b->quads.push_back(Quad(left, left.add(up), left2.add(up), left2));
	b->quads.push_back(Quad(right2, right2.add(up), right.add(up), right));
	return b;
}

void Brick::draw()
{
	color.bind();
	for (size_t i = 0; i < quads.size(); i++) {
		quads[i].draw();
	}
}

void Brick::trans(Matrix &m)
{
	for (size_t i = 0; i < quads.size(); i++) {
		quads[i].trans(m);
	}
}

void Brick::translate(Vector &v)
{
	for (size_t i = 0; i < quads.size(); i++) {
		quads[i].translate(v);
	}
}

double Brick::getTop()
{
	double top = 0.0;
	for (size_t i = 0; i < quads.size(); i++) {
		for (int j = 0; j < 4; j++) {
			top = std::max(top, quads[i][j][1]);
		}
	}
	return top;
}

double Brick::getBottom()
{
	double bot = quads[0][0][1];
	for (size_t i = 0; i < quads.size(); i++) {
		for (int j = 0; j < 4; j++) {
			bot = std::min(bot, quads[i][j][1]);
		}
	}
	return bot;
}

double Brick::getDistance(const Ball *b) const
{
	double dist = -1.0;
	for (size_t i = 0; i < quads.size(); i++) {
		double d = quads[i].getDistance(b);
		if (d == -1.0) {
			continue;
		}
		if (dist == -1) {
			dist = d;
		} else {
			dist = std::min(dist, d);
		}
		if (dist == 0.0) {
			return 0.0;
		}
	}
	return dist;
}
