#include "Texture3D.h"

Texture3D::Texture3D()
	: InternalFormat(GL_RGB), ImageFormat(GL_RGB), WrapS(GL_REPEAT), WrapT(GL_REPEAT), WrapR(GL_REPEAT), FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
{
	glGenTextures(1, &ID);

	Width["Right"] = 0;
	Width["Left"] = 0;
	Width["Top"] = 0;
	Width["Bottom"] = 0;
	Width["Back"] = 0;
	Width["Front"] = 0;

	Height["Right"] = 0;
	Height["Left"] = 0;
	Height["Top"] = 0;
	Height["Bottom"] = 0;
	Height["Back"] = 0;
	Height["Front"] = 0;
}

void Texture3D::Generate(const std::map<std::string, GLint>& Width, const std::map<std::string, GLint>& Height, const std::map<std::string, GLubyte*> Data)
{
	this->Width["Right"] = Width.at("Right");
	this->Width["Left"] = Width.at("Left");
	this->Width["Top"] = Width.at("Top");
	this->Width["Bottom"] = Width.at("Bottom");
	this->Width["Back"] = Width.at("Back");
	this->Width["Front"] = Width.at("Front");

	this->Height["Right"] = Height.at("Right");
	this->Height["Left"] = Height.at("Left");
	this->Height["Top"] = Height.at("Top");
	this->Height["Bottom"] = Height.at("Bottom");
	this->Height["Back"] = Height.at("Back");
	this->Height["Front"] = Height.at("Front");

	// Create Texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, this->Width["Right"], this->Height["Right"], 0, GL_RGB, GL_UNSIGNED_BYTE, Data.at("Right"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, this->Width["Left"], this->Height["Left"], 0, GL_RGB, GL_UNSIGNED_BYTE, Data.at("Left"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, this->Width["Top"], this->Height["Top"], 0, GL_RGB, GL_UNSIGNED_BYTE, Data.at("Top"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, this->Width["Bottom"], this->Height["Bottom"], 0, GL_RGB, GL_UNSIGNED_BYTE, Data.at("Bottom"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, this->Width["Back"], this->Height["Back"], 0, GL_RGB, GL_UNSIGNED_BYTE, Data.at("Back"));
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, this->Width["Front"], this->Height["Front"], 0, GL_RGB, GL_UNSIGNED_BYTE, Data.at("Front"));
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->WrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->WrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->WrapR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->FilterMax);

	// Unbind texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture3D::Bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

GLuint Texture3D::GetID() const
{
	return ID;
}

GLuint Texture3D::GetWidth(const std::string& Orientation) const
{
	return Width.at(Orientation);
}

GLuint Texture3D::GetHeight(const std::string& Orientation) const
{
	return Height.at(Orientation);
}

GLuint Texture3D::GetInternalFormat() const
{
	return InternalFormat;
}

GLuint Texture3D::GetImageFormat() const
{
	return ImageFormat;
}

GLuint Texture3D::GetWrapS() const
{
	return WrapS;
}

GLuint Texture3D::GetWrapT() const
{
	return WrapT;
}

GLuint Texture3D::GetWrapR() const
{
	return WrapR;
}

GLuint Texture3D::GetFilterMin() const
{
	return FilterMin;
}

GLuint Texture3D::GetFilterMax() const
{
	return FilterMax;
}

Texture3D& Texture3D::SetWrapS(const GLuint& WrapS)
{
	this->WrapS = WrapS;

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->WrapS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return *this;
}

Texture3D& Texture3D::SetWrapT(const GLuint& WrapT)
{
	this->WrapT = WrapT;

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->WrapT);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return *this;
}

Texture3D& Texture3D::SetWrapR(const GLuint& WrapR)
{
	this->WrapR = WrapR;

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, WrapR);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return *this;
}

Texture3D& Texture3D::SetFilterMin(const GLuint& FilterMin)
{
	this->FilterMin = FilterMin;

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return *this;
}

Texture3D& Texture3D::SetFilterMax(const GLuint& FilterMax)
{
	this->FilterMax = FilterMax;

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->ID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return *this;
}

