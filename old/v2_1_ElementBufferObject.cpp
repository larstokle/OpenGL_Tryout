//GLEW
#define GLEW_STATIC
#include "GL/glew.h"
//GLFW
//#define GLFW_DLL
#include "GLFW/glfw3.h"

#include <iostream>

//prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLFWwindow* setupGLFW(const int width, const int height, char* name);
GLenum setupGLEW();
GLint compileVertexShader();
GLint compileFragmentShader();
GLint linkShaders(GLint vertexShader, GLint fragmentShader);
void setupBinding(GLuint* VBO, GLuint* VAO, GLfloat* vertices);

//Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";


int main(){
	char name[] = "openGLTesting";
	GLFWwindow* window = setupGLFW(800, 600, name);
	if(window == nullptr){
		return -1;
	}

	GLenum err = setupGLEW();;
	if (err != GLEW_OK){
		return -1;
	}

	glViewport(0, 0, 800, 600);

	GLint vertexShader = compileVertexShader();
	GLint fragmentShader = compileFragmentShader();

	GLint shaderProgram = linkShaders(vertexShader, fragmentShader);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	std::cout << "Shaders deleted" << std::endl;

	//Set up vertex data (and buffers)) and attribute pointers
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5, 0.5, 0.0f
	};
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	std::cout << "initial binding complete" << std::endl;

	std::cout << "Setup complete, entering Game Loop" << std::endl;
	while(!glfwWindowShouldClose(window)){

		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	std::cout << "window should close registered" << std::endl;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	std::cout << "Sources Deallocated and GLFW Terminated" << std::endl;
	return 0;
}

GLFWwindow* setupGLFW(const int width, const int height, char* name){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	if(window == nullptr){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}else{
		std::cout << "GLFW window created" << std::endl;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	return window;
}

GLenum setupGLEW(){
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK){
		std::cout << "Failed to initialize GLEW. error: " << glewGetErrorString(err) << std::endl;
	}else{
		std::cout << "Glew initialized" << std::endl;
	}

	return err;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		std::cout << "Escape key registered" << std::endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

GLint compileVertexShader(){
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}else{
		std::cout << "Vertex shader compiled" << std::endl;
	}

	return vertexShader;
}

GLint compileFragmentShader(){
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}else{
		std::cout << "Fragment shader compiled" << std::endl;
	}

	return fragmentShader;
}

GLint linkShaders(GLint vertexShader, GLint fragmentShader){
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}else{
		std::cout << "Shader program linked" << std::endl;
	}

	return shaderProgram;
}

//TODO, does not work------------------------------------------------
void setupBinding(GLuint VBO, GLuint VAO, GLfloat* vertices){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	std::cout << "initial binding complete" << std::endl;
}
//-------------------------------------------------------