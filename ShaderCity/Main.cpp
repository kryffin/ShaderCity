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

void render(GLFWwindow* window) {
	//do things
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

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	std::cout << "Width : " << width << " | Height : " << height << std::endl;

	//GLFW contains 2 buffers : a front and a back one. the front is being displayed while the back is being written to
	glfwSwapInterval(1); //goas against screen tearing (vsync)

	while (!glfwWindowShouldClose(window)) {
		//keeps the window running
		render(window);
		glfwPollEvents(); //processes events
	}

	glfwDestroyWindow(window); //destroys a window

	glfwTerminate(); //terminates all remaining windows

	return EXIT_SUCCESS;
}
