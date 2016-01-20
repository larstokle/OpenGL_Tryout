#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "GL/glew.h"

class Shader
{
public:
	Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);

	GLuint Program;
	void use();
};

#endif