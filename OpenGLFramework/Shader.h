#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
* A convenience class for using shaders in OpenGL
*/
class Shader
{
public:

	GLuint ID;

	// Sets the current shader as active
	Shader& Use();

	// Compiles the shader from given source code
	void Compile(const GLchar* VertexSource, const GLchar* FragmentSource, const GLchar* GeometrySource = nullptr); // Note: geometry source code is optional 

	// Utility functions
	void SetFloat(const GLchar* Name, const GLfloat& Value, const GLboolean& UseShader = false);
	void SetInteger(const GLchar* Name, const GLint& Value, const GLboolean& UseShader = false);
	void SetVector2f(const GLchar* Name, const GLfloat& x, const GLfloat& y, const GLboolean& UseShader = false);
	void SetVector2f(const GLchar* Name, const glm::vec2& Value, const GLboolean& UseShader = false);
	void SetVector3f(const GLchar* Name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLboolean& UseShader = false);
	void SetVector3f(const GLchar* Name, const glm::vec3& Value, const GLboolean& UseShader = false);
	void SetVector4f(const GLchar* Name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w, const GLboolean& UseShader = false);
	void SetVector4f(const GLchar* Name, const glm::vec4& Value, const GLboolean& UseShader = false);
	void SetMatrix4(const GLchar* Name, const glm::mat4& Matrix, const GLboolean& UseShader = false);

private:

	// Checks if compilation or linking failed and if so, print the error logs
	void CheckCompileErrors(const GLuint& Object, const std::string& Type);
};