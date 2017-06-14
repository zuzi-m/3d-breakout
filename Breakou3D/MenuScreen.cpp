#include "MenuScreen.h"

#include "Point.h"
#include <iostream>

MenuScreen::MenuScreen(std::string title, Texture* background)
{
	this->title = title;
	this->background_texture = background;
}


MenuScreen::~MenuScreen()
{
}

void MenuScreen::addButton(std::string text, void(*callback)())
{
	double B_W = 220;
	double B_H = 35;
	double B_TOP = 75;
	double B_LEFT = 60;
	double y = B_TOP + buttons.size() * B_H;
	buttons.push_back(Button(B_LEFT, y + B_H, B_W, B_H - 5, text, callback));
	if (buttons.size() == 1) {
		currentButton = 0;
		buttons[currentButton].isSelected = true;
	}
}

void MenuScreen::addContent(std::string str)
{
	content.push_back(str);
}

void MenuScreen::clearContent()
{
	content.clear();
}

void MenuScreen::resize(int w, int h)
{
	width = w;
	height = h;
}

void MenuScreen::draw()
{
	glColor3d(1.0, 1.0, 1.0);
	background_texture->bind();
	// draw the background
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex3d(0.0, 0.0, 0.0);

	glTexCoord2d(1.0, 0.0);
	glVertex3d(width, 0.0, 0.0);

	glTexCoord2d(1.0, 1.0);
	glVertex3d(width, height, 0.0);

	glTexCoord2d(0.0, 1.0);
	glVertex3d(0.0, height, 0.0);
	glEnd();
	background_texture->unbind();

	// draw the title
	glRasterPos2d(35, 55);
	for (size_t i = 0; i<title.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title[i]);
	}

	// draw buttons
	for (size_t i = 0; i < buttons.size(); i++) {
		buttons[i].draw();
	}

	// draw the content
	glColor3d(0.0, 0.0, 0.0);
	double currentX;
	double currentY = 140;
	for (size_t i = 0; i < content.size(); i++) {
		std::string currentContent = content[i];
		currentX = 320;

		glRasterPos2d(currentX, currentY);
		for (size_t j = 0; j < currentContent.size(); j++) {
			currentX += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, currentContent[j]);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, currentContent[j]);
			if (currentX > (width - 20)) {
				currentX = 320;
				currentY += glutBitmapHeight(GLUT_BITMAP_TIMES_ROMAN_24) + 5;
				glRasterPos2d(currentX, currentY);
			}
		}
		currentY += glutBitmapHeight(GLUT_BITMAP_TIMES_ROMAN_24) + 5;
	}
}

void MenuScreen::up()
{
	if (currentButton > 0) {
		buttons[currentButton].isSelected = false;
		currentButton--;
		buttons[currentButton].isSelected = true;
	}
}

void MenuScreen::down()
{
	if (currentButton < buttons.size() - 1) {
		buttons[currentButton].isSelected = false;
		currentButton++;
		buttons[currentButton].isSelected = true;
	}
}

void MenuScreen::enter()
{
	buttons[currentButton].enter();
}
