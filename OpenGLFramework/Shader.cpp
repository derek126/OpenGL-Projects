#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader& Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const GLchar* VertexSource, const GLchar* FragmentSource, const GLchar* GeometrySource)
{
	GLuint sVertex, sFragment, gShader;

	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &VertexSource, NULL);
	glCompileShader(sVertex);
	CheckCompileErrors(sVertex, "VERTEX");

	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &FragmentSource, NULL);
	glCompileShader(sFragment);
	CheckCompileErrors(sFragment, "FRAGMENT");

	// If geometry shader source code is given, also compile geometry shader
	if (GeometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &GeometrySource, NULL);
		glCompileShader(gShader);
		CheckCompileErrors(gShader, "GEOMETRY");
	}

	// Shader Program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, sVertex);
	glAttachShader(this->ID, sFragment);
	if (GeometrySource != nullptr)
		glAttachShader(this->ID, gShader);
	glLinkProgram(this->ID);
	CheckCompileErrors(this->ID, "PROGRAM");

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (GeometrySource != nullptr)
		glDeleteShader(gShader);
}

void Shader::SetFloat(const GLchar* Name, const GLfloat& Value, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->ID, Name), Value);
}

void Shader::SetInteger(const GLchar* Name, const GLint& Value, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID, Name), Value);
}

void Shader::SetVector2f(const GLchar* Name, const GLfloat& x, const GLfloat& y, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, Name), x, y);
}

void Shader::SetVector2f(const GLchar* Name, const glm::vec2& Value, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, Name), Value.x, Value.y);
}

void Shader::SetVector3f(const GLchar* Name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, Name), x, y, z);
}

void Shader::SetVector3f(const GLchar* Name, const glm::vec3& Value, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, Name), Value.x, Value.y, Value.z);
}

void Shader::SetVector4f(const GLchar* Name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, Name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar* Name, const glm::vec4& Value, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, Name), Value.x, Value.y, Value.z, Value.w);
}

void Shader::SetMatrix4(const GLchar* Name, const glm::mat4& Matrix, const GLboolean& UseShader)
{
	if (UseShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, Name), 1, GL_FALSE, glm::value_ptr(Matrix));
}

void Shader::CheckCompileErrors(const GLuint& Object, const std::string& Type)
{
	GLint Success;
	GLchar InfoLog[1024];

	if (Type != "PROGRAM")
	{
		glGetShaderiv(Object, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(Object, 1024, NULL, InfoLog);
			std::cerr << "| ERROR::SHADER: Compile-time error: Type: " << Type << "\n"
				<< InfoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(Object, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(Object, 1024, NULL, InfoLog);
			std::cerr << "| ERROR::Shader: Link-time error: Type: " << Type << "\n"
				<< InfoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}