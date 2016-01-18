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
	void Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); // Note: geometry source code is optional 

	// Utility functions
	void SetFloat(const GLchar* name, const GLfloat& value, const GLboolean& useShader = false);
	void SetInteger(const GLchar* name, const GLint& value, const GLboolean& useShader = false);
	void SetVector2f(const GLchar* name, const GLfloat& x, const GLfloat& y, const GLboolean& useShader = false);
	void SetVector2f(const GLchar* name, const glm::vec2& value, const GLboolean& useShader = false);
	void SetVector3f(const GLchar* name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLboolean& useShader = false);
	void SetVector3f(const GLchar* name, const glm::vec3& value, const GLboolean& useShader = false);
	void SetVector4f(const GLchar* name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w, const GLboolean& useShader = false);
	void SetVector4f(const GLchar* name, const glm::vec4& value, const GLboolean& useShader = false);
	void SetMatrix4(const GLchar* name, const glm::mat4& matrix, const GLboolean& useShader = false);

private:

	// Checks if compilation or linking failed and if so, print the error logs
	void CheckCompileErrors(const GLuint& object, const std::string& type);
};