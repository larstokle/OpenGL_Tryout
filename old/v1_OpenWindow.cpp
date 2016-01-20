//GLEW
#define GLEW_STATIC
#include "GL/glew.h"
//GLFW
//#define GLFW_DLL
#include "GLFW/glfw3.h"

#include <iostream>

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800,600, "openGLTesting", nullptr, nullptr);
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}else{
		std::cout << "GLFW window created" << std::endl;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK){
		std::cout << "Failed to initialize GLEW. error: " << glewGetErrorString(err) << std::endl;
		return -1;
	}else{
		std::cout << "Glew initialized" << std::endl;
	}

	glViewport(0, 0, 800, 600);

	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;

}