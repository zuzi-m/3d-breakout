#include "CollisionDetector.h"

#include <algorithm>

#include "MathCommon.h"
#include "Ray.h"

using namespace MathLib;

Point getClosestPoint(const Point& p, const Quad& q) {
	// get the direction vectors of the plane
	Vector u = Point::vector(q[1], q[0]);
	Vector v = Point::vector(q[1], q[2]);
	Vector s = Point::vector(q[1], p);
	// get the closest point of the plane to the center of ball
	return q[1].add(u.project(s)).add(v.project(s));
}

bool isFacingQuad(const Ray& r, const Quad& q) {
	Vector normal = q.getNormal();
	Vector p = Point::vector(q[0], r.start);
	double angle_cos = normal.dot(p.normalize());
	if (angle_cos > 0.0) {
		double s = -normal.dot(p) / normal.dot(r.direction);
		
		// "s" is distance from the start of the ray to the plane of the quad (ray.direction is normalized)
		// if it is negative, the ray does not hit it at all
		return s > 0.0;
	}
	else {
		return false;
	}
}

double getDistanceFromPolygon(const Point& point, const Quad& q) {
	// NOTE the p should already lie in the plane of the quad q
	double minDist = -1.0;

	Vector n = q.getNormal();
	for (int i = 0; i < 4; i++) {
		// edge direction
		Vector v = Point::vector(q[i], q[(i + 1) % 4]);
		// direction for the edge's point and the intersection
		Vector p = Point::vector(q[i], point);

		// cosine of their angle
		Vector perpendicular = n.cross(v.normalize());
		double angle_cos = perpendicular.normalize().dot(p.normalize());
		if (angle_cos < 0.0) {
			Ray r(q[i], v);
			double dist = r.distance(point);
			if (minDist <= 0.0) {
				minDist = dist;
			}
			else {
				minDist = std::min(minDist, dist);
			}
		}
	}
	if (minDist <= 0.0) {
		return 0.0;
	} else {
		return minDist;
	}
}

double CollisionDetector::getCollision(Ball *ball, Brick *brick, double dt)
{
	// prepare variables used for each quad:
	// velocity of the ball
	Vector velocity = ball->getVelocity();
	// radius of the ball
	double radius = ball->getRadius();
	// original position of the ball
	Point oldPos = ball->getPosition();
	// Ray of the ball
	Ray ballRay(oldPos, velocity);

	// check for collision with each wall of the brick
	for (int i = 0; i < brick->numQuads(); i++) {
		Quad currentQuad = brick->operator[](i);

		// reject the quad if the ball is not going towards it
		if (!isFacingQuad(ballRay, currentQuad)) {
			continue;
		}

		// just a sanity check - if the ball already collides with quad, let it pass
		if (Point::vector(getClosestPoint(oldPos, currentQuad), oldPos).length() < radius) {
			continue;
		}

		// start checking for collision after moving the ball
		double distAmount = dt;
		Point newPos = oldPos.add(velocity.multiply(distAmount));
		Point closestPoint = getClosestPoint(newPos, currentQuad);
		double dist = Point::vector(closestPoint, newPos).length();

		// reject the quad if the ball on new position does not collide with quad's plane
		if (dist > radius) {
			continue;
		}

		// if the ball collides with quad plane, check if it also collides with the polygon
		double polyDist = getDistanceFromPolygon(closestPoint, currentQuad);
		if (polyDist > radius) {
			continue;
		}

		// if the ball on new position collides with quad polygon, find the collision time
		double currentStep = distAmount / 2.0;
		distAmount = distAmount - currentStep;
		while (abs(dist - radius) > 0.1) {
			newPos = oldPos.add(velocity.multiply(distAmount));
			closestPoint = getClosestPoint(newPos, currentQuad);
			dist = Point::vector(newPos, closestPoint).length();
			currentStep = currentStep / 2.0;
			if (dist < radius) {
				distAmount -= currentStep;
			}
			else {
				distAmount += currentStep;
			}
		}

		// now the "distAmount" shows the time of collision
		// now find the new velocity of the ball and carry on
		Vector V = velocity.normalize();
		Vector N = currentQuad.getNormal();
		Vector reflected = V.sub(N.multiply(2.0 * V.dot(N))).normalize();
		Vector newVelocity = reflected.multiply(velocity.length());
		ball->setVelocity(newVelocity);
		ball->setPosition(newPos);
		return dt - distAmount;
	}
	return 0.0;
}
