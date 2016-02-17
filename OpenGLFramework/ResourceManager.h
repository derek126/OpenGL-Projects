#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

#include "Texture2D.h"
#include "Texture3D.h"
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

	// The following functions are for adding to or updating the shared UBO
	void SetProjectionMatrix(const glm::mat4& Projection);
	void SetViewMatrix(const glm::mat4& View);
	void SetLightDirection(const glm::vec3& Direction);
	void SetLightColor(const glm::vec3& Color);

	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	Shader& LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, const std::string& Name);

	// Retrieves a stored shader
	Shader& GetShader(const std::string& Name);

	// Loads (and generates) a texture from file
	Texture2D& LoadTexture2D(const GLchar *File, const GLboolean& Alpha, const std::string& Name);

	// Loads (and generates) a cubemap from provided files
	Texture3D& LoadTexture3D(const std::map<std::string, GLchar*> File, const GLboolean& Alpha, const std::string& Name);

	// Retrieves a stored texture
	Texture2D& GetTexture2D(const std::string& Name);

	// Retrieves a stored cubemap
	Texture3D& GetTexture3D(const std::string& Name);

	// Properly de-allocates all loaded resources
	void Clear();

private:

	// Has the UBO been initialized?
	GLboolean bIsUBOInit;
	void InitUBO(); // Initializes the UBO

	// Resource storage
	std::map<std::string, Shader> Shaders;
	std::map<std::string, Texture2D> Textures2D;
	std::map<std::string, Texture3D> Textures3D;
	GLuint UBO; // Meant to be used as a shared resource for ALL shaders managed by this class. (Matrices, lights, etc)

	// Loads and generates a shader from file
	Shader LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

	// Loads a single texture from file
	Texture2D LoadTexture2DFromFile(const GLchar *File, const GLboolean& Alpha);

	// Loads a single cubemap from provided files
	Texture3D LoadTexture3DFromFile(const std::map<std::string, GLchar*> File, const GLboolean& Alpha);
};

