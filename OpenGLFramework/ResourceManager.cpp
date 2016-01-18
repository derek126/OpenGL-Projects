#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL.h>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	Clear();
}

ResourceManager& ResourceManager::GetInstance()
{
	static ResourceManager Instance;
	return Instance;
}

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, const std::string& Name)
{
	Shaders[Name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[Name];
}

Shader ResourceManager::GetShader(const std::string& Name)
{
	return Shaders[Name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *File, const GLboolean& Alpha, const std::string& Name)
{
	Textures[Name] = LoadTextureFromFile(File, Alpha);
	return Textures[Name];
}

Texture2D ResourceManager::GetTexture(const std::string& Name)
{
	return Textures[Name];
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);

	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);

	// (Properly) delete all UBO
	for (auto iter : UBOs)
		glDeleteBuffers(1, &iter.second);

	glDeleteBuffers(1, &UBO);
}

Shader ResourceManager::LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	// Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;

		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	// Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const GLchar *File, const GLboolean& Alpha)
{
	// Create Texture object
	Texture2D texture;
	if (Alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	// Load image
	GLint width, height;
	GLubyte* image = SOIL_load_image(File, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	// Now generate texture
	texture.Generate(width, height, image);

	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}
