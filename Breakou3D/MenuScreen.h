#pragma once

#include <vector>

#include "Button.h"
#include "Texture.h"
#include "Quad.h"

class MenuScreen
{
public:
	MenuScreen(std::string title, Texture* background);
	~MenuScreen();

	void addButton(std::string text, void(*callback)());
	void addContent(std::string str);
	void clearContent();
	void resize(int w, int h);

	void draw();

	void up();
	void down();
	void enter();
private:
	int width;
	int height;
	std::string title;
	Texture* background_texture = NULL;
	std::vector<Button> buttons;
	std::vector<std::string> content;

	int currentButton = 0;
};

