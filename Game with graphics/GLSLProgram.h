#pragma once
#include <string>
#include <GL\glew.h>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void addAttribute(const std::string& attribName);
	void linkShaders();

	void use();
	void unUse();

private:

	void compileShader(const std::string& shaderFilePath, GLuint& shaderID, GLenum type);

	GLuint _numAttrib;

	GLuint _programID;

	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
};

