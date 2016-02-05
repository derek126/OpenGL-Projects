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

	// Constructor (sets default texture modes)
	Texture3D();

	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;

	// Texture image dimensions
	std::map<std::string, GLint> Width, Height; // Width and height of loaded image in pixels, indexed by orientation

	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image

	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Wrap_R; // Wrapping mode on R axis

	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels

	// Generates texture from image data
	void Generate(const std::map<std::string, GLint>& Width, const std::map<std::string, GLint>& Height, const std::map<std::string, GLubyte*> Data);

	// Binds the texture as the current active GL_TEXTURE_CUBEMAP texture object
	void Bind() const;
};

