#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Window.hpp"


const char* vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec2 aTexCoord;\n"
	"\n"
	"out vec2 TexCoord;\n"
	"\n"
	"void main()\n"
	"{\n"
	"    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
	"    TexCoord = aTexCoord;\n"
	"}\n";

const char* fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"\n"
	"in vec2 TexCoord;\n"
	"\n"
	"uniform sampler2D ourTexture;\n"
	"\n"
	"void main()\n"
	"{\n"
	"    FragColor = texture(ourTexture, TexCoord);\n"
	"}\n";


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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);


	// Vertex positions in NDC range [-1.0, 1.0]
	float vertices[] = {
		// positions 	// texcoords
		-0.5f, -0.5f,	0.0f, 0.0f,	// bottom left	
		-0.5f,  0.5f,	0.0f, 1.0f,	// top left
		 0.5f,  0.5f,	1.0f, 1.0f,	// top right
		 0.5f, -0.5f,	1.0f, 0.0f,	// bottom right
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	const int indicesCount = sizeof(indices) / sizeof(indices[0]);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind our settings
	glBindVertexArray(0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!window.shouldClose())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		window.swapBuffers();
		window.pollEvents();
	}

	return 0;
}

