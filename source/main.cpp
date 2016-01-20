//-------------------------<INCLUDES>---------------------------------------------
//GLEW: get platform spesific functions
#define GLEW_STATIC
#include "GL/glew.h"
//GLFW: manage window
#include <GLFW/glfw3.h>
//SOIL: manage images to use as textures
#include <SOIL.h>
//GLM: GL-mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
//iostream: cout for debugging
#include <iostream>
//unistd: for looptiming
#include <unistd.h>
//implemented classheaders
#include "ShaderClass.h"
//------------------------</INCLUDES>---------------------------------------


//------------------------<PROTOTYPES>----------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLFWwindow* setupGLFW(const int width, const int height, char* name);
GLenum setupGLEW();
GLint compileVertexShader();
GLint compileFragmentShader();
GLint linkShaders(GLint vertexShader, GLint fragmentShader);
void setupBinding(GLuint* VBO, GLuint* VAO, GLfloat* vertices);
//------------------------</PROTOTYPES>----------------------------------------------

//------------------------<CONSTANTS>----------------------------------------------
const GLchar* vertexShaderPath = "/home/lars-christian/Programing/OpenGLtryout/shaders/simpleVS.vshader";
const GLchar* fragmentShaderPath = "/home/lars-christian/Programing/OpenGLtryout/shaders/simpleFS.fshader";
const char* containerTexturePath = "/home/lars-christian/Programing/OpenGLtryout/textures/container.jpg"; 

const GLfloat FPS = 60.0;
const GLfloat UPDATE_PERIODE = 1/FPS;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
//------------------------</CONSTANTS>----------------------------------------------

//------------------------<GLOBAL VARIABLES>-----------------------------------------
int containerWidth;
int containerHeigth;

glm::vec3 transVec(0,0,0);
glm::vec3 rotVec(0,0,0);
glm::vec3 scaleVec(1,1,1);

//------------------------</GLOBAL VARIABLES>----------------------------------------


//------------------------<MAIN>----------------------------------------------------
int main(){
	char name[] = "openGLTesting";
	GLFWwindow* window = setupGLFW(WINDOW_WIDTH, WINDOW_HEIGHT, name);
	if(window == nullptr){
		return -1;
	}

	GLenum err = setupGLEW();
	if (err != GLEW_OK){
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	Shader shader(vertexShaderPath, fragmentShaderPath);

	//Set up vertex data (and buffers)) and attribute pointers
	GLfloat vertices[] = {
		//coords				colors		texcoord
		-0.5f, -0.5f, -0.5f,	0, 0, 0,	0, 0,	//bottom left back
		0.5f, -0.5f, -0.5f,		0, 0, 0,	1, 0,	//bottom right back
		0.5f, 0.5f, -0.5f,		0, 0, 0,	1, 1,	//top right back
		-0.5f, 0.5f, -0.5f,		0, 0, 0,	0, 1,	//top left back
		-0.5f, -0.5f, 0.5f,		0, 0, 0,	1, 0,	//bottom left front
		0.5f, -0.5f, 0.5f,		0, 0, 0,	0, 0,	//bottom right front
		0.5f, 0.5f, 0.5f,		0, 0, 0,	0, 1,	//top right front
		-0.5f, 0.5f, 0.5f,		0, 0, 0,	1, 1,	//top left front
	};

	GLuint indices[] = {
		0, 1, 2,	//back lower
		0, 2, 3,	//back upper
		1, 5, 6,	//right lower
		1, 6, 2,	//right upper
		4, 5, 6,	//front lower
		4, 6, 7,	//front upper
		0, 4, 7,	//left lower
		0, 7, 3,	//left upper
		3, 2, 6,	//top front
		3, 6, 7,	//top back
		0, 1, 5,	//bottom front
		0, 6, 4,	//bottom back
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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	std::cout << "initial binding complete" << std::endl;

	//textures
	unsigned char* containerImage = SOIL_load_image(containerTexturePath, &containerWidth, &containerHeigth, 0, SOIL_LOAD_RGB);
	GLuint contianerTexture;
	glGenTextures(1, &contianerTexture);
	glBindTexture(GL_TEXTURE_2D, contianerTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, containerWidth, containerHeigth, 0, GL_RGB, GL_UNSIGNED_BYTE, containerImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(containerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	std::cout << "Texture binding complete" << std::endl;
	glEnable(GL_DEPTH_TEST);
	std::cout << "Setup complete, entering Game Loop" << std::endl;
	while(!glfwWindowShouldClose(window)){
		GLfloat loopStartTime = glfwGetTime();
		glfwPollEvents();

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::translate(model, glm::vec3(0.0,0.0,-1.0));

		view = glm::rotate(view, rotVec.x,glm::vec3(1.0,0.0,0.0));
		view = glm::rotate(view, rotVec.y,glm::vec3(0.0,1.0,0.0));
		view = glm::rotate(view, rotVec.z,glm::vec3(0.0,0.0,1.0));
		view = glm::translate(view, transVec);
		view = glm::scale(view, scaleVec);

		projection =  glm::perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLint modelMatrixLocation = glGetUniformLocation(shader.Program, "model");
		GLint viewMatrixLocation = glGetUniformLocation(shader.Program, "view");
		GLint projectionMatrixLocation = glGetUniformLocation(shader.Program, "projection");
		glBindTexture(GL_TEXTURE_2D, contianerTexture);
		shader.use();
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

		//make wanted FPS
		GLfloat loopDuration = glfwGetTime() - loopStartTime;

		if(loopDuration < UPDATE_PERIODE){ usleep((int)((UPDATE_PERIODE - loopDuration)*1000000.0)); }


	}
	std::cout << "window should close registered" << std::endl;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	std::cout << "Sources Deallocated and GLFW Terminated" << std::endl;
	return 0;
}
//------------------------</MAIN>----------------------------------------------------

//---------------------------<FUNCTIONS>-------------------------------
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
	switch(key){
		case GLFW_KEY_ESCAPE:
			if(action == GLFW_PRESS){
				std::cout << "Escape key registered" << std::endl;
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			break;
		case GLFW_KEY_UP:
			if(action == GLFW_PRESS){
				transVec.z += 0.1f*cos(rotVec.y);
				transVec.x -= 0.1f*sin(rotVec.y);
			}
			break;
		case GLFW_KEY_DOWN:
			if(action == GLFW_PRESS){
				transVec.z -= 0.1f*cos(rotVec.y);
				transVec.x += 0.1f*sin(rotVec.y);
			}
			break;
		case GLFW_KEY_RIGHT:
			if(action == GLFW_PRESS){
				transVec.x -= 0.1f*cos(rotVec.y);
				transVec.z -= 0.1f*sin(rotVec.y);
			}
			break;
		case GLFW_KEY_LEFT:
			if(action == GLFW_PRESS){
				transVec.x += 0.1f*cos(rotVec.y);
				transVec.z += 0.1f*sin(rotVec.y);
			}
			break;
		case GLFW_KEY_A:
			if(action == GLFW_PRESS){
				rotVec.y -= 6.28f/20.0f;
			}
			break;
		case GLFW_KEY_D:
			if(action == GLFW_PRESS){
				rotVec.y += 6.28f/20.0f;
			}
			break;
	}
}
//---------------------------</FUNCTIONS>-------------------------------