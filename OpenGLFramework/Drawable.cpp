#include "Drawable.h"

Drawable::Drawable() :
	ShaderName(""),
	TextureName("")
{}

Drawable::~Drawable() 
{}

void Drawable::SetShader(const std::string& Name)
{
	ShaderName = Name;
}

void Drawable::SetTexture2D(const std::string& Name)
{
	TextureName = Name;
}