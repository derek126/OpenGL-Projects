#pragma once

#include <GL/glew.h>

/**
* A convenience class for using 2D texture in OpenGL
*/
class Texture2D
{
public:

	friend class ResourceManager;

	// Constructor (sets default texture modes)
	Texture2D();

	// Getters and setters
	GLuint GetID() const;
	GLuint GetWidth() const;
	GLuint GetHeight() const;
	GLuint GetInternalFormat() const;
	GLuint GetImageFormat() const;
	GLuint GetWrapS() const;
	GLuint GetWrapT() const;
	GLuint GetFilterMin() const;
	GLuint GetFilterMax() const;

	Texture2D& SetWrapS(const GLuint& WrapS);
	Texture2D& SetWrapT(const GLuint& WrapT);
	Texture2D& SetFilterMin(const GLuint& FilterMin);
	Texture2D& SetFilterMax(const GLuint& FilterMax);

	// Generates texture from image data
	void Generate(const GLuint& Width, const GLuint& Height, const GLubyte* Data);

	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;

private:

	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;

	// Texture image dimensions
	GLuint Width, Height; // Width and height of loaded image in pixels

						  // Texture Format
	GLuint InternalFormat; // Format of texture object
	GLuint ImageFormat; // Format of loaded image

						 // Texture configuration
	GLuint WrapS; // Wrapping mode on S axis
	GLuint WrapT; // Wrapping mode on T axis

	GLuint FilterMin; // Filtering mode if texture pixels < screen pixels
	GLuint FilterMax; // Filtering mode if texture pixels > screen pixels
};

