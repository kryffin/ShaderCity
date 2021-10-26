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

	// Forcing an OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	// VBO MACHIN TRUC

	    // VERTICES
    GLfloat vertices[]{
        -0.5f    ,    0.25f,
         0.5f    ,    0.25f,
         0.5f    ,    -0.25f,
         -0.5f    ,    -0.25f,

         -0.5f    ,    -0.25f,
         0.5f    ,    -0.25f,
         0.5f    ,    -0.75f,
         -0.5f    ,    -0.75f,
    };

    // VERTICES
    GLuint indices[]{
        0, 2, 3,
        0, 1, 2,

        4, 6, 7,
        4, 5, 6,
    };

    // VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Attribution du tableau au GPU qui le transform en vec2
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    glEnableVertexAttribArray(0);

    // EBO
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// -------------------------
	// Loading the vertex shader

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string vertexShaderFile = readFile("basic_vert.glsl");
	const char* vertexShaderSource = vertexShaderFile.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Error loading the vertex shader\n" << infoLog << std::endl;
		return EXIT_FAILURE;
	}

	// ---------------------------
	// Loading the fragment shader

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string fragmentShaderFile = readFile("basic_frag.glsl");
	const char* fragmentShaderSource = fragmentShaderFile.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Error attaching the shaders\n" << infoLog << std::endl;
		return EXIT_FAILURE;
	}

	glUseProgram(shaderProgram);

	// We don't need the shaders anymore, they are already linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ---------------------------

	while (!glfwWindowShouldClose(window)) {
		//keeps the window running
		//render(window);
		glClearColor(0.894f, 0.514f, 0.714f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// affiche le triangle depuis le vbo
		glDrawArrays(GL_TRIANGLES, 0, 2);

		// affiche le rect depuis le ebo
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (GLvoid*)(0));
		// affiche ls triangles rempli
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// affiche le rect depuis le ebo
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(0));
		// affiche ls triangles en fil de fer
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//GLFW contains 2 buffers : a front and a back one. the front is being displayed while the back is being written to
		glfwSwapBuffers(window);
		glfwPollEvents(); //processes events
	}

	glfwDestroyWindow(window); //destroys a window

	glfwTerminate(); //terminates all remaining windows

	return EXIT_SUCCESS;
}
