#include "Button.h"

#include "Point.h"

Button::Button(double x, double y, double w, double h, std::string text, void(*callback)()) : quad(MathLib::Point(x, y, 0), MathLib::Point(x+w, y, 0), MathLib::Point(x+w, y+h, 0), MathLib::Point(x, y+h, 0))
{
	this->text = text;
	this->color_back = Color(0.95);
	this->color_selected = Color(0.75);
	this->color_text = Color(0.0);
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->isSelected = false;
	this->callback = callback;
}


Button::Button(const Button &other) : quad(other.quad)
{
	text = other.text;
	color_back = other.color_back;
	color_selected = other.color_selected;
	color_text = other.color_text;
	x = other.x;
	y = other.y;
	w = other.w;
	h = other.h;
	isSelected = other.isSelected;
	callback = other.callback;
}

Button::~Button()
{
}

void Button::draw()
{
	if (isSelected) {
		quad.color = Color(color_selected);
	}
	else {
		quad.color = Color(color_back);
	}

	quad.draw();

	// draw the text
	glColor3d(color_text[0], color_text[1], color_text[2]);
	glRasterPos2d(quad[3].X()+5, quad[3].Y()-5);
	for (unsigned int i = 0; i<text.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

void Button::enter()
{
	callback();
}
