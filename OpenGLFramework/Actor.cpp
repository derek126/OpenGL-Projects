#include "Actor.h"

Actor::Actor() :
	bModelChanged(GL_TRUE)
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
	bModelChanged = GL_TRUE;
}

void Actor::SetRotation(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	glm::mat4 I;
	RotationMatrix = glm::rotate(I, Deg, RotationAxis);
	bModelChanged = GL_TRUE;
}

void Actor::SetScale(const glm::vec3& ScaleAmnt)
{
	glm::mat4 I;
	ScaleMatrix = glm::scale(I, ScaleAmnt);
	bModelChanged = GL_TRUE;
}

void Actor::Translate(const glm::vec3& Translation)
{
	TranslationMatrix = glm::translate(TranslationMatrix, Translation);
	bModelChanged = GL_TRUE;
}

void Actor::Rotate(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	RotationMatrix = glm::rotate(RotationMatrix, Deg, RotationAxis);
	bModelChanged = GL_TRUE;
}

void Actor::Scale(const glm::vec3& ScaleAmnt)
{
	ScaleMatrix = glm::scale(ScaleMatrix, ScaleAmnt);
	bModelChanged = GL_TRUE;
}

void Actor::SetTranslation(const glm::mat4& Translation)
{
	TranslationMatrix = Translation;
	bModelChanged = GL_TRUE;
}

void Actor::SetRotation(const glm::mat4& Rotation)
{
	RotationMatrix = Rotation;
	bModelChanged = GL_TRUE;
}

void Actor::SetScale(const glm::mat4& Scale)
{
	ScaleMatrix = Scale;
	bModelChanged = GL_TRUE;
}

glm::mat4 Actor::GetModelMatrix()
{
	if (bModelChanged)
	{
		ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		bModelChanged = GL_FALSE;
	}
	return ModelMatrix;
}

void Actor::SetModel(const glm::mat4& Model)
{
	bModelChanged = GL_FALSE;
	ModelMatrix = Model;
}

void Actor::ResetTransform()
{
	TranslationMatrix = glm::mat4(1.f);
	RotationMatrix = glm::mat4(1.f);
	ScaleMatrix = glm::mat4(1.f);
	bModelChanged = GL_TRUE;
}
