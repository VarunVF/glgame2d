#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Window.hpp"
#include "QuadVAO.hpp"
#include "Shader.hpp"


int main(void)
{
	Window window(640, 480, "title");

	// texture stuff
	int width, height, nrChannels;
	unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0); 
	if (!data)
	{
		std::cerr << "Failed to load texture\n";
		return -1;
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	QuadVAO vao;
	Shader shader;

	while (!window.shouldClose())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shader.shaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao.VAO);
		glDrawElements(GL_TRIANGLES, vao.indicesCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		window.swapBuffers();
		window.pollEvents();
	}

	return 0;
}

