#include "Drawable.h"

Drawable::Drawable() :
	ShaderName(""),
	TextureName("")
{}

Drawable::~Drawable() 
{}

glm::vec3 Drawable::GetPosition() const
{
	return glm::vec3(TranslationMatrix[3][0], TranslationMatrix[3][1], TranslationMatrix[3][2]);
}

void Drawable::SetTranslation(const glm::vec3& Translation)
{
	glm::mat4 I;
	TranslationMatrix = glm::translate(I, Translation);
}

void Drawable::SetRotation(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	glm::mat4 I;
	RotationMatrix = glm::rotate(I, Deg, RotationAxis);
}

void Drawable::SetScale(const glm::vec3& ScaleAmnt)
{
	glm::mat4 I;
	ScaleMatrix = glm::scale(I, ScaleAmnt);
}

void Drawable::Translate(const glm::vec3& Translation)
{
	TranslationMatrix = glm::translate(TranslationMatrix, Translation);
}

void Drawable::Rotate(const GLfloat& Deg, const glm::vec3& RotationAxis)
{
	RotationMatrix = glm::rotate(RotationMatrix, Deg, RotationAxis);
}

void Drawable::Scale(const glm::vec3& ScaleAmnt)
{
	ScaleMatrix = glm::scale(ScaleMatrix, ScaleAmnt);
}


glm::mat4 Drawable::GetModelMatrix() const
{
	return TranslationMatrix * RotationMatrix * ScaleMatrix;
}

void Drawable::ResetTransform()
{
	TranslationMatrix = glm::mat4(1.f);
	RotationMatrix = glm::mat4(1.f);
	ScaleMatrix = glm::mat4(1.f);
}

void Drawable::SetShader(const std::string& Name)
{
	ShaderName = Name;
}

void Drawable::SetTexture2D(const std::string& Name)
{
	TextureName = Name;
}