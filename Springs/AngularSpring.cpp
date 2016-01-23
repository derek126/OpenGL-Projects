#include "AngularSpring.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/random.hpp>

AngularSpring::AngularSpring(const std::string& ShaderName, const GLfloat& Radius, const GLfloat& SpringContant, const GLfloat& Dampening, const GLfloat& RestAngle) :
	ShaderName(ShaderName),
	Radius(Radius),
	SpringContant(SpringContant),
	RestAngle(RestAngle),
	Dampening(Dampening)
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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

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

	FixedMass.SetShader(ShaderName);
}

AngularSpring::~AngularSpring()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void AngularSpring::Init()
{
	Model = TranslationMatrix * glm::translate(Model, glm::vec3(Radius, 0.f, 0.f));
	Model *= ScaleMatrix;
}

void AngularSpring::Simulate(const GLfloat& dt)
{
	GLfloat CurrentAngle = glm::asin((Model[3][1] - TranslationMatrix[3][1]) / glm::length(Model[3] - TranslationMatrix[3]));
	GLfloat Torque = SpringContant * (RestAngle - CurrentAngle) - Dampening * AngularVelocity;

	AngularVelocity += Torque / (2.f / 5.f * 10 * glm::pow(Radius, 2)) * dt;
	Rotation += AngularVelocity * dt;

	Model = glm::rotate(glm::mat4(), Rotation, glm::vec3(0.f, 0.f, 1.f));
	Model = TranslationMatrix * glm::translate(Model, glm::vec3(Radius, 0.f, 0.f));
	Model *= ScaleMatrix;
}

void AngularSpring::Draw()
{
	RESOURCEMANAGER.GetShader("Phong").SetVector3f("Color", glm::vec3(0.f, 0.75f, 1.f), true);
	FixedMass.SetTranslation(TranslationMatrix);
	FixedMass.SetScale(ScaleMatrix);
	FixedMass.Draw();

	// Draw the objects
	glBindVertexArray(VAO);

	RESOURCEMANAGER.GetShader(ShaderName).SetMatrix4("Model", Model);
	RESOURCEMANAGER.GetShader("Phong").SetVector3f("Color", glm::vec3(1.0f, 0.5f, 0.2f), true);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}
