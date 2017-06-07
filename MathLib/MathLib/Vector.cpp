#include "Vector.h"

namespace MathLib {

	Vector::Vector()
	{
		u = 0;
		v = 0;
		w = 0;
	}

	Vector::Vector(Vector & other)
	{
		u = other.u;
		v = other.v;
		w = other.w;
	}

	Vector::Vector(double x)
	{
		u = x;
		v = x;
		w = x;
	}

	Vector::Vector(double u, double v, double w)
	{
		this->u = u;
		this->v = v;
		this->w = w;
	}


	Vector::~Vector()
	{
	}

	double Vector::U() const
	{
		return u;
	}

	double Vector::V() const
	{
		return v;
	}

	double Vector::W() const
	{
		return w;
	}

	double Vector::length() const
	{
		return sqrt(this->lengthSquared());
	}

	double Vector::lengthSquared() const
	{
		return (u*u) + (v*v) + (w*w);
	}

	bool Vector::isZero() const
	{
		return abs(u + v + w) < EPS;
	}

	bool Vector::isUnit() const
	{
		return (abs(this->lengthSquared()) - 1) < EPS;
	}

	Vector Vector::normalize() const
	{
		double length = this->length();
		return Vector(this->u / length, this->v / length, this->w / length);
	}

	double Vector::dot(Vector &other) const
	{
		return u*other.u + v*other.v + w*other.w;
	}

	Vector Vector::cross(Vector &other) const
	{
		return Vector(v*other.w - w*other.v, w*other.u - u*other.w, u*other.v - v*other.u);
	}

	Vector Vector::add(Vector &other) const
	{
		return Vector(this->u + other.u, this->v + other.v, this->w + other.w);
	}

	Vector Vector::sub(Vector &other) const
	{
		return this->add(other.opposite());
	}

	Vector Vector::opposite() const
	{
		return this->multiply(-1);
	}

	Vector Vector::multiply(double s) const
	{
		return Vector(u*s, v*s, w*s);
	}

}
