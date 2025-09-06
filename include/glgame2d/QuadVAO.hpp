#pragma once


namespace glgame2d {


class QuadVAO
{
public:
    QuadVAO();

public:
    unsigned int VAO, VBO, IBO;
    int indicesCount;

private:
    // Vertex positions in NDC range [-1.0, 1.0]
	float vertices[8] = {
		 0.0f, -1.0f,	// bottom left	
		 0.0f,  0.0f,	// top left
		 1.0f,  0.0f,	// top right
		 1.0f, -1.0f,	// bottom right
	};

	unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3,
	};
};


} // namespace glgame2d
