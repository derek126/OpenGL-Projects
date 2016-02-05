#include "Cube.h"

Cube::Cube()
{
	GLfloat vertices[] = {
		// Vertices			  // Tex coords // Normals

		// -Z
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,

		// +Z
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

		// -X
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

		// +X
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,

		// -Y
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,   1.0f, 1.0f,  0.0f,  -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  0.0f,  -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f,  0.0f,  -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  -1.0f,  0.0f,

		// +Y  
		0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 0.0f,  0.0f,  1.0f,  0.0f

	};

	// Create and store buffers
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	Buffers["VAO"] = VAO;
	Buffers["VBO"] = VBO;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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


Cube::~Cube()
{
	// Properly clean up buffers
	glDeleteVertexArrays(1, &Buffers["VAO"]);
	glDeleteBuffers(1, &Buffers["VBO"]);
	glDeleteBuffers(1, &Buffers["EBO"]);
}

void Cube::Draw()
{
	// Set the MVP matrices for the shader
	RESOURCEMANAGER.GetShader(ShaderName).Use();
	RESOURCEMANAGER.GetShader(ShaderName).SetMatrix4("Model", GetModelMatrix());

	// Bind the texture
	RESOURCEMANAGER.GetTexture(TextureName).Bind();

	// Draw the object
	glBindVertexArray(Buffers["VAO"]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
