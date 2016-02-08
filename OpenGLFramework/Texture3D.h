#pragma once

#include <GL/glew.h>
#include <map>
#include <string>

/**
* A convenience class for using 3D (cubemap) texture in OpenGL
*/
class Texture3D
{
public:

	friend class ResourceManager;

	// Constructor (sets default texture modes)
	Texture3D();

	// Getters and setters
	GLuint GetID() const;
	GLuint GetWidth(const std::string& Orientation) const;
	GLuint GetHeight(const std::string& Orientation) const;
	GLuint GetInternalFormat() const;
	GLuint GetImageFormat() const;
	GLuint GetWrapS() const;
	GLuint GetWrapT() const;
	GLuint GetWrapR() const;
	GLuint GetFilterMin() const;
	GLuint GetFilterMax() const;

	Texture3D& SetWrapS(const GLuint& WrapS);
	Texture3D& SetWrapT(const GLuint& WrapT);
	Texture3D& SetWrapR(const GLuint& WrapR);
	Texture3D& SetFilterMin(const GLuint& FilterMin);
	Texture3D& SetFilterMax(const GLuint& FilterMax);

	// Generates texture from image data
	void Generate(const std::map<std::string, GLint>& Width, const std::map<std::string, GLint>& Height, const std::map<std::string, GLubyte*> Data);

	// Binds the texture as the current active GL_TEXTURE_CUBEMAP texture object
	void Bind() const;

private:

	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;

	// Texture image dimensions
	std::map<std::string, GLint> Width, Height; // Width and height of loaded image in pixels, indexed by orientation

	// Texture Format
	GLuint InternalFormat; // Format of texture object
	GLuint ImageFormat; // Format of loaded image

						 // Texture configuration
	GLuint WrapS; // Wrapping mode on S axis
	GLuint WrapT; // Wrapping mode on T axis
	GLuint WrapR; // Wrapping mode on R axis

	GLuint FilterMin; // Filtering mode if texture pixels < screen pixels
	GLuint FilterMax; // Filtering mode if texture pixels > screen pixels
};

