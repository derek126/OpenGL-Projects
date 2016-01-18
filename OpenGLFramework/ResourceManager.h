#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

#include "Texture2D.h"
#include "Shader.h"

#define RESOURCEMANAGER ResourceManager::GetInstance()

/**
* Singleton object used to manage all shaders and 2D textures in our game
*/
class ResourceManager
{
	ResourceManager();
	~ResourceManager();

	ResourceManager(ResourceManager const&) = delete;
	void operator =(ResourceManager const&) = delete;

public:

	static ResourceManager& GetInstance();

	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, const std::string& Name);

	// Retrieves a stored shader
	Shader GetShader(const std::string& Name);

	// Loads (and generates) a texture from file
	Texture2D LoadTexture(const GLchar *File, const GLboolean& Alpha, const std::string& Name);

	// Retrieves a stored texture
	Texture2D GetTexture(const std::string& Name);

	// Properly de-allocates all loaded resources
	void Clear();

private:

	// Resource storage
	std::map<std::string, Shader> Shaders;
	std::map<std::string, Texture2D> Textures;
	std::map<std::string, GLuint> UBOs;
	GLuint UBO; // Meant to be used as a shared resource for ALL shaders managed by this class. (Matrices, lights, etc)

	// Loads and generates a shader from file
	Shader LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

	// Loads a single texture from file
	Texture2D LoadTextureFromFile(const GLchar *File, const GLboolean& Alpha);
};

