#include "Ball.h"

#include <gl\freeglut.h>

Ball::Ball(double radius) : position(0.0, radius, 0.0), velocity(0.0), color(0.0, 0.0, 1.0)
{
	this->radius = radius;
}


Ball::~Ball()
{
}

void Ball::draw()
{
	glPushMatrix();

	color.bind();

	glTranslated(position.X(), position.Y(), position.Z());

	glutSolidSphere(radius, 36, 36);

	MathLib::Point end(velocity.multiply(100.0));
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(end[0], end[1], end[2]);
	glEnd();

	glPopMatrix();
}

void Ball::move(long milis)
{
	position = position.add(velocity.multiply(milis / 1000.0));
}

MathLib::Point Ball::getPosition() const
{
	return MathLib::Point(position);
}

void Ball::setPosition(MathLib::Point &v)
{
	position = v;
}

MathLib::Vector Ball::getVelocity() const
{
	return MathLib::Vector(velocity);
}

void Ball::setVelocity(MathLib::Vector &v)
{
	this->velocity = v;
}
