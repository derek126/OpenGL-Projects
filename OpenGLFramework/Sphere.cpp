#include "Sphere.h"
#include <vector>

Sphere::Sphere(const GLfloat& R,const GLuint& DivX, const GLuint& DivY) :
	Radius(R),
	SDivX(DivX),
	SDivY(DivY)
{
	CreateSphere();
}

Sphere::~Sphere()
{
	// Properly clean up buffers
	glDeleteVertexArrays(1, &Buffers["VAO"]);
	glDeleteBuffers(1, &Buffers["Verts"]);
	glDeleteBuffers(1, &Buffers["Norms"]);
	glDeleteBuffers(1, &Buffers["EBO"]);
}

void Sphere::Draw()
{	
	// Set the MVP matrices for the shader
	RESOURCEMANAGER.GetShader(ShaderName).Use();
	RESOURCEMANAGER.GetShader(ShaderName).SetMatrix4("Model", GetModelMatrix());

	// Bind the texture
	RESOURCEMANAGER.GetTexture2D(TextureName).Bind();

	// Draw the object
	glBindVertexArray(Buffers["VAO"]);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::CreateSphere()
{
	glm::vec3 P;

	GLfloat Theta = 0.0f, Phi = 0.0f;
	GLfloat PhiDelta = glm::pi<GLfloat>() / (SDivY + 1);
	GLfloat ThetaDelta = glm::two_pi<GLfloat>() / SDivX;

	P = glm::vec3(0.f, 0.f, Radius);
	Vertices.push_back(P);
	Normals.push_back(glm::normalize(P));

	for (GLuint y = 0; y < SDivY; ++y)
	{
		Phi += PhiDelta;
		Theta = 0.0f;

		for (GLuint x = 0; x < SDivX; ++x)
		{
			Theta = x * ThetaDelta;

			P.x = Radius * glm::cos(Theta) * glm::sin(Phi);
			P.y = Radius * glm::sin(Theta) * glm::sin(Phi);
			P.z = Radius * glm::cos(Phi);
			Vertices.push_back(P);
			Normals.push_back(glm::normalize(P));
		}
	}

	P = glm::vec3(0.f, 0.f, -Radius);
	Vertices.push_back(P);
	Normals.push_back(glm::normalize(P));

	///////////////////////////////////////////////

	std::vector<GLuint> UpperLoop(SDivX, 0);
	std::vector<GLuint> LowerLoop(SDivY, 0);

	for (GLuint i = 0; i < SDivX; ++i)
	{
		LowerLoop[i] = i + 1;
	}

	for (GLuint i = 0; i < SDivX; ++i)
	{
		Indices.push_back(UpperLoop[i]);
		Indices.push_back(LowerLoop[i]);
		Indices.push_back(LowerLoop[(i + 1) % SDivX]);
	}

	for (GLuint y = 0; y < SDivY - 1; ++y)
	{
		for (GLuint i = 0; i < SDivX; ++i)
		{
			UpperLoop[i] = (y * SDivX + 1) + i;
			LowerLoop[i] = (y * SDivX + 1) + SDivX + i;
		}

		for (GLuint x = 0, i = 0; x < SDivX; ++x, ++i)
		{
			GLuint j = (i + 1) % SDivX;
			Indices.push_back(UpperLoop[i]);
			Indices.push_back(LowerLoop[i]);
			Indices.push_back(LowerLoop[j]);

			Indices.push_back(LowerLoop[j]);
			Indices.push_back(UpperLoop[j]);
			Indices.push_back(UpperLoop[i]);
		}
	}

	for (GLuint i = 0; i < SDivX; ++i)
	{
		LowerLoop[i] = Vertices.size() - 1;
		UpperLoop[i] = Vertices.size() - 1 - SDivX + i;
	}

	for (GLuint i = 0; i < SDivX; ++i)
	{
		Indices.push_back(LowerLoop[i]);
		Indices.push_back(UpperLoop[i]);
		Indices.push_back(UpperLoop[(i + 1) % SDivX]);
	}

	///////////////////////////////////////////////

	// UV here?

	///////////////////////////////////////////////

	// Create and store buffers
	GLuint VAO, Verts, EBO, Norms;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &Verts);
	glGenBuffers(1, &Norms);
	glGenBuffers(1, &EBO);

	Buffers["VAO"] = VAO;
	Buffers["Verts"] = Verts;
	Buffers["Norms"] = Norms;
	Buffers["EBO"] = EBO;

	glBindVertexArray(Buffers["VAO"]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers["EBO"]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), Indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers["Verts"]);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * 3 * sizeof(GLfloat), Vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers["Norms"]);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * 3 * sizeof(GLfloat), Normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
}

GLfloat Sphere::GetRadius() const
{
	return Radius;
}

std::vector<glm::vec3> Sphere::GetVertices() const
{
	return Vertices;
}

std::vector<GLuint> Sphere::GetIndices() const
{
	return Indices;
}

std::vector<glm::vec3> Sphere::GetNormals() const
{
	return Normals;
}
