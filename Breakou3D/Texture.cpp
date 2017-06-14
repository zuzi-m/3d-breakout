#include "Texture.h"

#include <SOIL.h>

Texture::Texture(std::string filename)
{
	this->tex_id = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MIPMAPS);
	if (tex_id == 0) {
		throw SOIL_last_result();
	}
	else {
		this->unbind();
	}
}


Texture::~Texture()
{
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
