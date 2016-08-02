#include "ShaderClass.h"

#include "GL/glew.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

using namespace std;


Shader::Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath){
	string vertexCode, fragmentCode;
	ifstream vertexShaderFile, fragmentShaderFile;

	vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try{
		vertexShaderFile.open(vertexShaderPath);
		cout << "vertex file open\n";
		fragmentShaderFile.open(fragmentShaderPath);
		cout << "fragment file open\n";

		stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		cout << "vertex file read\n";
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		cout << "fragment file read\n";

		vertexShaderFile.close();
		cout << "vertex file closed\n";
		fragmentShaderFile.close();
		cout << "fragment file closed\n";

		vertexCode = vertexShaderStream.str();
		cout << "vertex code made\n";
		fragmentCode = fragmentShaderStream.str();
		cout << "fragment code made\n";
	}catch(ifstream::failure e){
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << endl;

	}

	const GLchar* vertexShaderCode = vertexCode.c_str();
	const GLchar* fragmentShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::fragment::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use(){
	glUseProgram(this->Program);
}