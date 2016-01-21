#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL.h>

ResourceManager::ResourceManager() :
	bIsUBOInit(GL_FALSE)
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

void ResourceManager::InitUBO()
{
	glGenBuffers(1, &UBO);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW); // Allocate memory
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4)); // Bind buffer point

	bIsUBOInit = GL_TRUE;
}

void ResourceManager::SetProjectionMatrix(const glm::mat4& Projection)
{
	if (!bIsUBOInit) InitUBO();

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ResourceManager::SetViewMatrix(const glm::mat4& View)
{
	if (!bIsUBOInit) InitUBO();

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(View));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, const std::string& Name)
{
	if (!bIsUBOInit) InitUBO();

	Shaders[Name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);

	GLuint UBOBlockIndex = glGetUniformBlockIndex(Shaders[Name].ID, "SceneData");
	glUniformBlockBinding(Shaders[Name].ID, UBOBlockIndex, 0);

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
