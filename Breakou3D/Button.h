#pragma once

#include <string>

#include "Color.h"
#include "Quad.h"

class Button
{
public:
	Button(double x, double y, double w, double h, std::string text, void (*callback)());
	Button(const Button&);
	~Button();

	void draw();
	void enter();

	bool isSelected;
private:
	double x, y, w, h;
	std::string text;

	Quad quad;
	Color color_text;
	Color color_selected;
	Color color_back;

	void(*callback)();
};

