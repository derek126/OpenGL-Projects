#include "Texture3D.h"

Texture3D::Texture3D()
	: Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Wrap_R(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
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
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->Wrap_R);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

	// Unbind texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture3D::Bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}
