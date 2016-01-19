#pragma once

#include "Actor.h"

#include <map>
#include <string>

#include "ResourceManager.h"

/*
* Base class for any base object that can be rendered in the scene
*/
class Drawable : public Actor
{
public:

	Drawable();
	virtual ~Drawable();

	virtual void Draw(const glm::mat4& View, const glm::mat4& Projection) = 0;

	// Set the objects shader and texture
	void SetShader(const std::string& Name);
	void SetTexture2D(const std::string& Name);

protected:

	std::string ShaderName, TextureName;

	// Buffer storage
	std::map<std::string, GLuint> Buffers;
};