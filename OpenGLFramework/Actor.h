#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
* Base class for any object that can present in the scene
*/
class Actor
{

public:

	Actor();
	virtual ~Actor();

	// Return the position of the object
	glm::vec3 GetPosition() const;

	// Transform functions that add to the current transform
	void Translate(const glm::vec3& Translation);
	void Rotate(const GLfloat& Deg, const glm::vec3& RotationAxis);
	void Scale(const glm::vec3& ScaleAmnt);

	// Transform functions that reset the current transform
	void SetTranslation(const glm::vec3& Translation);
	void SetRotation(const GLfloat& Deg, const glm::vec3& RotationAxis);
	void SetScale(const glm::vec3& ScaleAmnt);

	// Transform functions that reset the current transform with a new matrix
	void SetTranslation(const glm::mat4& Translation);
	void SetRotation(const glm::mat4& Rotation);
	void SetScale(const glm::mat4& Scale);

	// Reset the objects transform back to origin and default scale and rotation
	void ResetTransform();

protected:

	// Computes the model matrix
	glm::mat4 GetModelMatrix();

	// State
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScaleMatrix;
	glm::mat4 ModelMatrix;
	GLboolean bModelChanged;
};

