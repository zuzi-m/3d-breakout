#include "Color.h"

#include <gl\freeglut.h>
#include <algorithm>
#include <cstdlib>

Color::Color()
{
	r = 0.0;
	g = 0.0;
	b = 0.0;
	a = 1.0;
}

Color::Color(double grayscale)
{
	r = grayscale;
	g = grayscale;
	b = grayscale;
	a = 1.0;
}

Color::Color(double red, double green, double blue)
{
	r = red;
	g = green;
	b = blue;
	a = 1.0;
	clamp();
}

Color::Color(double red, double green, double blue, double alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
	clamp();
}

Color::Color(MathLib::Vector &v)
{
	r = v[0];
	g = v[1];
	b = v[2];
	a = 1.0;
	clamp();
}


Color::~Color()
{
}

Color Color::randomColor(double alpha)
{
	return Color((rand() % 101) / 100.0, (rand() % 101) / 100.0, (rand() % 101) / 100.0, alpha);
}

Color Color::hsv(int h, double s, double v, double alpha)
{
	h = h % 360;
	double hh = h / 60.0;
	int x = static_cast<int>(floor(hh));
	double f = hh - x;
	double p = v * (1 - s);
	double q = v * (1 - s * f);
	double t = v * (1 - s * (1 - f));
	double r, g, b;
	if (h >= 0 && h < 60) {
		r = v; g = t; b = p;
	}
	else if (h >= 60 && h < 120) {
		r = q; g = v; b = p;
	}
	else if (h >= 120 && h < 180) {
		r = p; g = v; b = t;
	}
	else if (h >= 180 && h < 240) {
		r = p; g = q; b = v;
	}
	else if (h >= 240 && h < 300) {
		r = t; g = p; b = v;
	}
	else if (h >= 300 && h < 360) {
		r = v; g = p; b = q;
	}

	return Color(r, g, b, alpha);
}

void Color::bind()
{
	glColor4d(r, g, b, a);
	float vals[3]{ static_cast<float>(r), static_cast<float>(g), static_cast<float>(b) };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, vals);
}

double Color::operator[](const int i) const
{
	switch (i % 4)
	{
	case 0:
		return r;
	case 1:
		return g;
	case 2:
		return b;
	case 3:
		return a;
	}
	return 0.0;
}

void Color::toArray(double *arr, bool includeAlpha) const
{
	*arr = r;
	arr++;
	*arr = g;
	arr++;
	*arr = b;
	if (includeAlpha) {
		arr++;
		*arr = a;
	}
}

void Color::toArray(float *arr, bool includeAlpha) const
{
	*arr = static_cast<float>(r);
	arr++;
	*arr = static_cast<float>(g);
	arr++;
	*arr = static_cast<float>(b);
	if (includeAlpha) {
		arr++;
		*arr = static_cast<float>(a);
	}
}

void Color::clamp()
{
	r = std::min(std::max(r, 0.0), 1.0);
	g = std::min(std::max(g, 0.0), 1.0);
	b = std::min(std::max(b, 0.0), 1.0);
	a = std::min(std::max(a, 0.0), 1.0);
}

std::ostream & operator<<(std::ostream &os, const Color &c)
{
	os << c.r << " " << c.g << " " << c.b << " " << c.a;
	return os;
}
