#pragma once

#include <string>
#include <gl\freeglut.h>

class Texture
{
public:
	Texture(std::string filename);
	~Texture();

	void bind();
	void unbind();
private:
	GLuint tex_id;
};

