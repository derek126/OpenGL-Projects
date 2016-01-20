#include "Quad.h"

#include <SOIL.h>

Quad::Quad()
{
	GLfloat vertices[] = {
		// Positions            // Texture Coords   // Normals
		0.5f,  0.5f, 0.0f,		1.0f, 1.0f,		0.f, 0.f, 1.f,	// Top Right
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.f, 0.f, 1.f,	// Bottom Right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.f, 0.f, 1.f,	 // Bottom Left
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,		0.f, 0.f, 1.f	 // Top Left 
	};

	GLuint indices[] = {
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	// Create and store buffers
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	Buffers["VAO"] = VAO;
	Buffers["VBO"] = VBO;
	Buffers["EBO"] = EBO;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

	// Tex coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	// Normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


Quad::~Quad()
{
	// Properly clean up buffers
	glDeleteVertexArrays(1, &Buffers["VAO"]);
	glDeleteBuffers(1, &Buffers["VBO"]);
	glDeleteBuffers(1, &Buffers["EBO"]);
}

void Quad::Draw(const glm::mat4& View, const glm::mat4& Projection)
{
	// Set the MVP matrices for the shader
	RESOURCEMANAGER.GetShader(ShaderName).Use();
	RESOURCEMANAGER.GetShader(ShaderName).SetMatrix4("projection", Projection);
	RESOURCEMANAGER.GetShader(ShaderName).SetMatrix4("view", View);
	RESOURCEMANAGER.GetShader(ShaderName).SetMatrix4("model", GetModelMatrix());

	// Bind the texture
	RESOURCEMANAGER.GetTexture(TextureName).Bind();

	// Draw the object
	glBindVertexArray(Buffers["VAO"]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
