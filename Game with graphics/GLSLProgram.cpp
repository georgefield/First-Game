#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>
#include <iostream>


GLSLProgram::GLSLProgram() : _numAttrib(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0) {
}


GLSLProgram::~GLSLProgram()
{
}


////PUBLIC----

void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

	compileShader(vertexShaderFilePath, _vertexShaderID, GL_VERTEX_SHADER);
	compileShader(fragmentShaderFilePath, _fragmentShaderID, GL_FRAGMENT_SHADER);
}


void GLSLProgram::addAttribute(const std::string& attribName) {

	glBindAttribLocation(_programID, _numAttrib++, attribName.c_str()); //add attribute and increase index to point to next location in GLSL input
}


void GLSLProgram::linkShaders() {

	_programID = glCreateProgram();
	glAttachShader(_programID, _vertexShaderID); //attach both shaders to a program
	glAttachShader(_programID, _fragmentShaderID);

	glLinkProgram(_programID); //link the program to ???

	GLint linkOK = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, &linkOK); //test if program failed
	if (linkOK == GL_FALSE) {
		GLint logSize = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &logSize);  //get length of error log to be printed

		std::string programLinkLog;
		programLinkLog.resize(logSize);
		glGetProgramInfoLog(_programID, logSize, &logSize, &programLinkLog[0]); //input error log into string

		glDeleteProgram(_programID); //delete all programs and shaders
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		error(programLinkLog + "\linking program failed", true); //exit game
	}

	glDetachShader(_programID, _vertexShaderID); //detach both shaders from program
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}


void GLSLProgram::use() {

	glUseProgram(_programID);
	for (int i = 0; i < _numAttrib; i++) {
		glEnableVertexAttribArray(i);
	}
}


void GLSLProgram::unUse() {

	glUseProgram(0);
	for (int i = 0; i < _numAttrib; i++) {
		glDisableVertexAttribArray(i);
	}
}


////PRIVATE----

void GLSLProgram::compileShader(const std::string& shaderFilePath, GLuint& shaderID, GLenum type) {

	//create the shader ID
	shaderID = glCreateShader(type); //create vertex shader
	if (shaderID == 0) { //check for failure
		std::string typeStr;
		switch(type){
		case GL_VERTEX_SHADER:
			typeStr = "vertex shader";
			break;
		case GL_FRAGMENT_SHADER:
			typeStr = "fragment shader";
			break;
		default:
			typeStr = "other shader";
			break;
		}
		error(typeStr + " failed to be created", true);
	}


	//load vertex shader code
	std::ifstream shaderFile(shaderFilePath);
	if (shaderFile.fail()) {
		perror(shaderFilePath.c_str());
		error("failed to open" + shaderFilePath, true);
	}

	//load vertex shader file in one string
	std::string fileContents = "";
	std::string currentLine;

	while (std::getline(shaderFile, currentLine)) {
		fileContents += currentLine;
	}
	shaderFile.close();
	currentLine.clear();


	//tell openGL where we have stored the source and what type of shader to use - info of type in _vertexShaderID
	const char* shaderFileContentsPtr = fileContents.c_str();
	glShaderSource(shaderID, 1, &shaderFileContentsPtr, nullptr); //which shader, how many strings, pointer to lines/line, length of each line (nullptr for one line)

	glCompileShader(shaderID); //attemp to compile shader

	//check for errors
	GLint compileOK = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileOK); //test if code failed to compile
	if (compileOK == GL_FALSE) {
		GLint logSize = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);  //get length of error log to be printed

		std::string shaderCompileLog;
		shaderCompileLog.resize(logSize);
		glGetShaderInfoLog(shaderID, logSize, &logSize, &shaderCompileLog[0]); //input error log into string

		glDeleteShader(shaderID); //delete shader
		error(shaderCompileLog + "shader compilation failed in file " + shaderFilePath, true); //exit game
	}
}