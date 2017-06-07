#include "Vector.h"

#include <cmath>

namespace MathLib {

	Vector::Vector()
	{
		u = 0;
		v = 0;
		w = 0;
	}

	Vector::Vector(const Vector & other)
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

	void Vector::toArray(double dest[])
	{
		dest[0] = u;
		dest[1] = v;
		dest[2] = w;
	}

	double Vector::length() const
	{
		return sqrt(this->lengthSquared());
	}

	double Vector::lengthSquared() const
	{
		return (u*u) + (v*v) + (w*w);
	}

	double Vector::angle(const Vector &v1, const Vector &v2)
	{
		return acos(v1.dot(v2) / (v1.length()*v2.length()));
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

	double Vector::dot(const Vector &other) const
	{
		return u*other.u + v*other.v + w*other.w;
	}

	Vector Vector::cross(const Vector &other) const
	{
		return Vector(v*other.w - w*other.v, w*other.u - u*other.w, u*other.v - v*other.u);
	}

	Vector Vector::add(const Vector &other) const
	{
		return Vector(this->u + other.u, this->v + other.v, this->w + other.w);
	}

	Vector Vector::sub(const Vector &other) const
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

	Vector Vector::multiply(const Vector &other) const
	{
		return Vector(u*other.u, v*other.v, w*other.w);
	}

	Vector Vector::project(const Vector &projected) const
	{
		Vector this_unit = normalize();
		return this_unit.multiply(projected.dot(this_unit));
	}

	std::ostream & operator<<(std::ostream &os, const Vector &v)
	{
		os << v.u << " " << v.v << " " << v.w;
		return os;
	}

	double Vector::operator[](const int index) const
	{
		switch (index % 3)
		{
		case 0:
			return u;
		case 1:
			return v;
		case 2:
			return w;
		}
		return u;
	}

	void Vector::toArray(double *arr) const
	{
		*arr = u;
		arr++;
		*arr = v;
		arr++;
		*arr = w;
	}

	void Vector::toArray(float *arr) const
	{
		*arr = static_cast<float>(u);
		arr++;
		*arr = static_cast<float>(v);
		arr++;
		*arr = static_cast<float>(w);
	}

}
