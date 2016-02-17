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
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW); // Allocate memory
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4)); // Bind buffer point

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

void ResourceManager::SetLightDirection(const glm::vec3& Direction)
{
	if (!bIsUBOInit) InitUBO();

	glm::vec3 D = glm::normalize(Direction);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(D));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ResourceManager::SetLightColor(const glm::vec3& Color)
{
	if (!bIsUBOInit) InitUBO();

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(Color));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Shader& ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, const std::string& Name)
{
	if (!bIsUBOInit) InitUBO();

	Shaders[Name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);

	GLuint UBOBlockIndex = glGetUniformBlockIndex(Shaders[Name].ID, "SceneData");
	glUniformBlockBinding(Shaders[Name].ID, UBOBlockIndex, 0);

	return Shaders[Name];
}

Shader& ResourceManager::GetShader(const std::string& Name)
{
	return Shaders[Name];
}

Texture2D& ResourceManager::LoadTexture2D(const GLchar *File, const GLboolean& Alpha, const std::string& Name)
{
	Textures2D[Name] = LoadTexture2DFromFile(File, Alpha);
	return Textures2D[Name];
}

Texture2D& ResourceManager::GetTexture2D(const std::string& Name)
{
	return Textures2D[Name];
}

Texture3D& ResourceManager::LoadTexture3D(const std::map<std::string, GLchar*> File, const GLboolean& Alpha, const std::string& Name)
{
	Textures3D[Name] = LoadTexture3DFromFile(File, Alpha);
	return Textures3D[Name];
}

Texture3D& ResourceManager::GetTexture3D(const std::string& Name)
{
	return Textures3D[Name];
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);

	Shaders.clear();

	// (Properly) delete all textures
	for (auto iter : Textures2D)
		glDeleteTextures(1, &iter.second.ID);

	Textures2D.clear();

	// (Properly) delete all cubemaps
	for (auto iter : Textures3D)
		glDeleteTextures(1, &iter.second.ID);

	Textures3D.clear();

	glDeleteBuffers(1, &UBO);
	bIsUBOInit = GL_FALSE;
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

Texture2D ResourceManager::LoadTexture2DFromFile(const GLchar *File, const GLboolean& Alpha)
{
	// Create Texture object
	Texture2D texture;
	if (Alpha)
	{
		texture.InternalFormat = GL_RGBA;
		texture.ImageFormat = GL_RGBA;
	}

	// Load image
	GLint width, height;
	GLubyte* image = SOIL_load_image(File, &width, &height, 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	if (!image)
	{
		std::cerr << "ERROR::TEXTURE: Could not load texture from file." << std::endl;
	}

	// Now generate texture
	texture.Generate(width, height, image);

	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}

// Loads a single cubemap from provided files
Texture3D ResourceManager::LoadTexture3DFromFile(const std::map<std::string, GLchar*> File, const GLboolean& Alpha)
{
	// Create Texture object
	Texture3D texture;
	if (Alpha)
	{
		texture.InternalFormat = GL_RGBA;
		texture.ImageFormat = GL_RGBA;
	}

	// Load image
	std::map<std::string, GLint> Width, Height;
	std::map<std::string, GLubyte*> Image;

	Image["Right"] = SOIL_load_image(File.at("Right"), &Width["Right"], &Height["Right"], 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	Image["Left"] = SOIL_load_image(File.at("Left"), &Width["Left"], &Height["Left"], 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	Image["Top"] = SOIL_load_image(File.at("Top"), &Width["Top"], &Height["Top"], 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	Image["Bottom"] = SOIL_load_image(File.at("Bottom"), &Width["Bottom"], &Height["Bottom"], 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	Image["Back"] = SOIL_load_image(File.at("Back"), &Width["Back"], &Height["Back"], 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	Image["Front"] = SOIL_load_image(File.at("Front"), &Width["Front"], &Height["Front"], 0, texture.GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	if (!Image["Right"] || !Image["Left"] || !Image["Top"] || !Image["Bottom"] || !Image["Back"] || !Image["Front"])
	{
		std::cerr << "ERROR::TEXTURE: Could not load texture from file." << std::endl;
	}

	// Now generate texture
	texture.Generate(Width, Height, Image);

	// And finally free image data
	SOIL_free_image_data(Image["Right"]);
	SOIL_free_image_data(Image["Left"]);
	SOIL_free_image_data(Image["Top"]);
	SOIL_free_image_data(Image["Bottom"]);
	SOIL_free_image_data(Image["Back"]);
	SOIL_free_image_data(Image["Front"]);
	return texture;
}
