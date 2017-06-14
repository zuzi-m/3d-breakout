#include "Quad.h"

#include <gl\freeglut.h>
#include <algorithm>

#include "Ray.h"

Quad::Quad(Point& p1, Point& p2, Point& p3, Point& p4) : color(1.0)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;

	updateNormal();
	for (int i = 0; i < 4; i++) {
		double d = Point::vector(points[i], points[(i+1) %4]).dot(normal);
		if (abs(d) > MathLib::EPS) {
			throw "The quad is not a plane";
		}
	}
}

Quad::Quad(const Quad &q)
{
	for (int i = 0; i < 4; i++) {
		points[i] = q.points[i];
	}
	normal = q.normal;
	color = q.color;
	texture = q.texture;
}


Quad::~Quad()
{
}

void bindTextureCoords(int index) {
	switch (index % 4)
	{
	case 0:
		glTexCoord2d(0.0, 0.0);
		break;
	case 1:
		glTexCoord2d(1.0, 0.0);
		break;
	case 2:
		glTexCoord2d(1.0, 1.0);
		break;
	case 3:
		glTexCoord2d(0.0, 1.0);
		break;
	}
}

void Quad::draw() const
{
	color.bind();
	if (texture != NULL) {
		texture->bind();
	}

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		if (texture != NULL) {
			bindTextureCoords(i);
		}
		glNormal3d(normal[0], normal[1], normal[2]);
		glVertex3d(points[i][0], points[i][1], points[i][2]);
	}
	glEnd();

	if (texture != NULL) {
		texture->unbind();
	}
}

void Quad::trans(Matrix &m)
{
	for (int i = 0; i < 4; i++) {
		points[i] = m.mult(points[i]);
	}
	updateNormal();
}

void Quad::translate(Vector &v)
{
	for (int i = 0; i < 4; i++) {
		points[i] = points[i].add(v);
	}
	updateNormal();
}

Vector Quad::getNormal() const
{
	return Vector(normal);
}

const Point& Quad::operator[](const int i) const
{
	return points[i % 4];
}

void Quad::updateNormal()
{
	normal = Point::vector(points[0], points[2]).normalize().cross(Point::vector(points[1], points[3]).normalize()).normalize();
}
