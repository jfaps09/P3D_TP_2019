#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint loadFileTGA(const char * imagepath) {
	GLuint texture;
	glGenTextures(1, &texture);

	//Bind the newly created texture
	glBindTexture(GL_TEXTURE_2D, texture);

	//Flip texture image on load, since texture coordinates are loaded inverted vertically
	stbi_set_flip_vertically_on_load(true);

	//Read the file using stb_image header, call glTexImage2D with the right parameters
	int width, height, channels;
	unsigned char* imageData = stbi_load(imagepath, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	//Pretty decent trilinear filtering, change anything(???)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}