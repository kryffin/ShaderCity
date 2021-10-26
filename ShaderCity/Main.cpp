#define GLFW_INCLUDE_NONE

#include <iostream>
#include <stdlib.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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

	while (!glfwWindowShouldClose(window)) {
		//keeps the window running
		glfwPollEvents();
	}

	glfwDestroyWindow(window); //destroys a window

	glfwTerminate(); //terminates all remaining windows

	return EXIT_SUCCESS;
}
