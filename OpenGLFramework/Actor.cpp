#include "Actor.h"

Actor::Actor() :
	bMVPChanged(GL_TRUE)
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
	bMVPChanged = GL_TRUE;
}

void Actor::SetRotation(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	glm::mat4 I;
	RotationMatrix = glm::rotate(I, Deg, RotationAxis);
	bMVPChanged = GL_TRUE;
}

void Actor::SetScale(const glm::vec3& ScaleAmnt)
{
	glm::mat4 I;
	ScaleMatrix = glm::scale(I, ScaleAmnt);
	bMVPChanged = GL_TRUE;
}

void Actor::Translate(const glm::vec3& Translation)
{
	TranslationMatrix = glm::translate(TranslationMatrix, Translation);
	bMVPChanged = GL_TRUE;
}

void Actor::Rotate(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	RotationMatrix = glm::rotate(RotationMatrix, Deg, RotationAxis);
	bMVPChanged = GL_TRUE;
}

void Actor::Scale(const glm::vec3& ScaleAmnt)
{
	ScaleMatrix = glm::scale(ScaleMatrix, ScaleAmnt);
	bMVPChanged = GL_TRUE;
}

void Actor::SetTranslation(const glm::mat4& Translation)
{
	TranslationMatrix = Translation;
	bMVPChanged = GL_TRUE;
}

void Actor::SetRotation(const glm::mat4& Rotation)
{
	RotationMatrix = Rotation;
	bMVPChanged = GL_TRUE;
}

void Actor::SetScale(const glm::mat4& Scale)
{
	ScaleMatrix = Scale;
	bMVPChanged = GL_TRUE;
}

glm::mat4 Actor::GetModelMatrix()
{
	if (bMVPChanged)
	{
		MVPMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		bMVPChanged = GL_FALSE;
	}
	return MVPMatrix;
}

void Actor::ResetTransform()
{
	TranslationMatrix = glm::mat4(1.f);
	RotationMatrix = glm::mat4(1.f);
	ScaleMatrix = glm::mat4(1.f);
	bMVPChanged = GL_TRUE;
}
