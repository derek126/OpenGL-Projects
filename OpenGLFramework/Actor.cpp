#include "Actor.h"

Actor::Actor()
{
}


Actor::~Actor()
{
}

glm::vec3 Actor::GetPosition() const
{
	return glm::vec3(TranslationMatrix[3][0], TranslationMatrix[3][1], TranslationMatrix[3][2]);
}

void Actor::SetTranslation(const glm::vec3& Translation)
{
	glm::mat4 I;
	TranslationMatrix = glm::translate(I, Translation);
}

void Actor::SetRotation(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	glm::mat4 I;
	RotationMatrix = glm::rotate(I, Deg, RotationAxis);
}

void Actor::SetScale(const glm::vec3& ScaleAmnt)
{
	glm::mat4 I;
	ScaleMatrix = glm::scale(I, ScaleAmnt);
}

void Actor::Translate(const glm::vec3& Translation)
{
	TranslationMatrix = glm::translate(TranslationMatrix, Translation);
}

void Actor::Rotate(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	RotationMatrix = glm::rotate(RotationMatrix, Deg, RotationAxis);
}

void Actor::Scale(const glm::vec3& ScaleAmnt)
{
	ScaleMatrix = glm::scale(ScaleMatrix, ScaleAmnt);
}


glm::mat4 Actor::GetModelMatrix() const
{
	return TranslationMatrix * RotationMatrix * ScaleMatrix;
}

void Actor::ResetTransform()
{
	TranslationMatrix = glm::mat4(1.f);
	RotationMatrix = glm::mat4(1.f);
	ScaleMatrix = glm::mat4(1.f);
}
