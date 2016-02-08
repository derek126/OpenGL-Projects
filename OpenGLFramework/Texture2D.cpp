#include "Texture2D.h"

Texture2D::Texture2D()
	: Width(0), Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(const GLuint& Width, const GLuint& Height, const GLubyte* Data)
{
	this->Width = Width;
	this->Height = Height;

	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, Width, Height, 0, this->ImageFormat, GL_UNSIGNED_BYTE, Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

GLuint Texture2D::GetID() const
{
	return ID;
}

GLuint Texture2D::GetWidth() const
{
	return Width;
}

GLuint Texture2D::GetHeight() const
{
	return Height;
}

GLuint Texture2D::GetInternalFormat() const
{
	return InternalFormat;
}

GLuint Texture2D::GetImageFormat() const
{
	return ImageFormat;
}

GLuint Texture2D::GetWrapS() const
{
	return WrapS;
}

GLuint Texture2D::GetWrapT() const
{
	return WrapT;
}

GLuint Texture2D::GetFilterMin() const
{
	return FilterMin;
}

GLuint Texture2D::GetFilterMax() const
{
	return FilterMax;
}

Texture2D& Texture2D::SetWrapS(const GLuint& GLuint)
{
	this->WrapS = WrapS;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
	glBindTexture(GL_TEXTURE_2D, 0);

	return *this;
}

Texture2D& Texture2D::SetWrapT(const GLuint& WrapT)
{
	this->WrapT = WrapT;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
	glBindTexture(GL_TEXTURE_2D, 0);

	return *this;
}

Texture2D& Texture2D::SetFilterMin(const GLuint& FilterMin)
{
	this->FilterMin = FilterMin;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glBindTexture(GL_TEXTURE_2D, 0);

	return *this;
}

Texture2D& Texture2D::SetFilterMax(const GLuint& FilterMax)
{
	this->FilterMax = FilterMax;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	glBindTexture(GL_TEXTURE_2D, 0);

	return *this;
}
