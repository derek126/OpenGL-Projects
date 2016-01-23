#pragma once

#include "Cube.h"

class AngularSpring : public Drawable
{
public:
	AngularSpring(const std::string& ShaderName, const GLfloat& Radius, const GLfloat& SpringContant, const GLfloat& RestAngle = glm::radians(45.f), const GLfloat& Dampening = 1.f);
	~AngularSpring();

	void Init();
	void Simulate(const GLfloat& dt);
	void Draw();

private:

	Cube FixedMass;
	std::string ShaderName;
	GLfloat Radius, AngularVelocity, Rotation, SpringContant, RestAngle, Dampening;
	GLuint VAO, VBO;

	glm::mat4 Model; // The model matrix for the moveable point
};

