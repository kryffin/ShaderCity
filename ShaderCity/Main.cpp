#define GLFW_INCLUDE_NONE

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

std::string readFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void render(GLFWwindow* window) {
	//do things
	glClearColor(0.894f, 0.514f, 0.714f, 1.f);
}

int main() {
	std::cout << "Hello World !" << std::endl;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		std::cerr << "GLFW initialization failed" << std::endl;
		return EXIT_FAILURE;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "ShaderCity", NULL, NULL);
	if (!window) {
		std::cerr << "GLFW window or context creation failed" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window); //making the window the current context (focus?)

	glewExperimental = true;

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) {
		std::cerr << "GLEW initialization failed" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	std::cout << "Width : " << width << " | Height : " << height << std::endl;

	// -------------------------
	// Loading the vertex shader

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	if (0 == vertShader) {
		std::cerr << "Error creating vertex shader" << std::endl;
		return EXIT_FAILURE;
	}

	std::string shaderCode = readFile("basic.vert.glsl");
	const GLchar* codeArray[] = {shaderCode.c_str()};
	glShaderSource(vertShader, 1, codeArray, NULL);

	glCompileShader(vertShader);

	GLint result;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		std::cerr << "Vertex shader compilation failed!" << std::endl;

		// Get and print the info log
		GLint logLen;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			std::string log(logLen, ' ');
			GLsizei written;
			glGetShaderInfoLog(vertShader, logLen, &written, &log[0]);
			std::cerr << "Shader log: " << std::endl << log;
		}

		return EXIT_FAILURE;
	}

	// -------------------------

	while (!glfwWindowShouldClose(window)) {
		//keeps the window running
		//render(window);
		glClearColor(0.894f, 0.514f, 0.714f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//GLFW contains 2 buffers : a front and a back one. the front is being displayed while the back is being written to
		glfwSwapBuffers(window);
		glfwPollEvents(); //processes events
	}

	glfwDestroyWindow(window); //destroys a window

	glfwTerminate(); //terminates all remaining windows

	return EXIT_SUCCESS;
}
