#include "Quad.h"

#include <gl\freeglut.h>
#include <algorithm>

#include "Ray.h"

Quad::Quad(Point& p1, Point& p2, Point& p3, Point& p4)
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
}


Quad::~Quad()
{
}

void Quad::draw()
{
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glVertex3d(points[i][0], points[i][1], points[i][2]);
		glNormal3d(normal[0], normal[1], normal[2]);
	}
	
	glEnd();
}

void Quad::trans(Matrix &m)
{
	for (int i = 0; i < 4; i++) {
		points[i] = m.mult(points[i]);
	}
}

void Quad::translate(Vector &v)
{
	for (int i = 0; i < 4; i++) {
		points[i] = points[i].add(v);
	}
}

double Quad::getDistance(const Ball *ball) const
{
	Ray ray(ball->getPosition(), ball->getVelocity());
	// get the intersection of ray and plane of the quad
	/*Vector l = Point::vector(ray.start, points[1]);
	Vector q1 = Point::vector(points[1], points[0]);
	Vector q2 = Point::vector(points[1], points[2]);

	Matrix m = Matrix::fromVectors(ray.direction, q1, q2);
	std::cout << "matrix:" << std::endl << m << std::endl;
	if (m.det() == 0.0) {
		return -1.0;
	}
	std::cout << "inv:" << std::endl << m.inverse() << std::endl;
	Vector t = m.inverse().mult(l);
	Point intersection(ray.start.add(ray.direction.multiply(t)));
	std::cout << "l: " << l << std::endl;
	std::cout << "intersection: " << intersection << std::endl;

	for (int i = 0; i < 4; i++) {
		Vector v = Point::vector(points[i], points[(i + 1) % 4]);
		Vector p = Point::vector(points[i], intersection);
		double angle = v.dot(p) / (v.length()*p.length());
		Ray r(points[i], v);
		double dist = r.distance(intersection);
		std::cout << "Angle " << acos(angle) * 180.0 / PI << ", dist: " << dist << std::endl;
	}*/

	// if the ray's direction is perpendicular to quad plane normal, it
	// is either not intersecting the quad plane or lies in it entirely
	if (normal.dot(ray.direction) == 0.0) {
		return -1.0;
	}

	Vector w = Point::vector(points[0], ray.start);
	double s = -normal.dot(w) / normal.dot(ray.direction);
	//std::cout << "s = " << s << std::endl;

	// "s" is distance from the start of the ray to the plane of the quad (ray.direction is normalized)
	// if it is negative, the ray does not hit it at all
	if (s < 0.0) {
		return -1.0;
	}

	// get the intersection point of the ray and the quad plane
	Point intersection(ray.start.add(ray.direction.multiply(s)));

	// see if the intersection lies inside the quad or not by looking at
	// the angle between each edge and the intersection point
	double minDist = -1.0;
	for (int i = 0; i < 4; i++) {
		// edge direction
		Vector v = Point::vector(points[i], points[(i + 1) % 4]);
		// direction for the edge's point and the intersection
		Vector p = Point::vector(points[i], intersection);
		// cosine of their angle
		double angle_cos = v.normalize().dot(p.normalize());
		if (angle_cos < 0.0) {
			Ray r(points[i], v);
			double dist = r.distance(intersection);
			if (minDist < 0) {
				minDist = dist;
			} else {
				minDist = std::min(minDist, dist);
			}
		}
	}
	
	// if the minDist was not changed from -1
	// the intersection lies inside the quad
	//std::cout << "d = " << minDist << std::endl;
	if (minDist == -1) {
		// the distance of the ball is just "s"
		return s;
	}
	else {
		// if the minDist is defined, the distance is the length of vector (s, minDist)
		return sqrt((s*s) + (minDist * minDist));
	}
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
	normal = Point::vector(points[0], points[2]).normalize().cross(Point::vector(points[1], points[3]).normalize());
}
