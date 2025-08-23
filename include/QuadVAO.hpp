#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>


class QuadVAO
{
public:
    QuadVAO()
    {
        indicesCount = sizeof(indices) / sizeof(indices[0]);

        // VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // IBO
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind our settings
	    glBindVertexArray(0);
    }

public:
    unsigned int VAO, VBO, IBO;
    int indicesCount;

private:
    // Vertex positions in NDC range [-1.0, 1.0]
	float vertices[16] = {
		// positions 	// texcoords
		-0.5f, -0.5f,	0.0f, 0.0f,	// bottom left	
		-0.5f,  0.5f,	0.0f, 1.0f,	// top left
		 0.5f,  0.5f,	1.0f, 1.0f,	// top right
		 0.5f, -0.5f,	1.0f, 0.0f,	// bottom right
	};

	unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3,
	};
};
