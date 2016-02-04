#pragma once

#include "Cube.h"

/*
* A self-contained class to simulate an angular spring
*/
class AngularSpring : public Drawable
{
public:
	AngularSpring(const std::string& ShaderName, const GLfloat& Length, const GLfloat& SpringContant, const GLfloat& Dampening = 0.f, const GLfloat& RestAngle = glm::radians(45.f));
	~AngularSpring();

	void Init();
	void Simulate(const GLfloat& dt);
	void Draw();

private:

	Cube FixedMass;
	std::string ShaderName;
	GLfloat Length, AngularVelocity, Rotation, SpringContant, RestAngle, Dampening;
	GLuint VAO, VBO;

	glm::mat4 Model; // The model matrix for the moveable point
};

