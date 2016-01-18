#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <string>

#include "ResourceManager.h"

class Drawable
{
public:

	Drawable();
	virtual ~Drawable();

	virtual void Draw(const glm::mat4& View, const glm::mat4& Projection) = 0;

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

	// Reset the objects transform back to origin and default scale and rotation
	void ResetTransform();

	// Set the objects shader and texture
	void SetShader(const std::string& Name);
	void SetTexture2D(const std::string& Name);

protected:

	// Computes the model matrix
	glm::mat4 GetModelMatrix() const;

	// State
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScaleMatrix;
	std::string ShaderName, TextureName;

	// Buffer storage
	std::map<std::string, GLuint> Buffers;
};