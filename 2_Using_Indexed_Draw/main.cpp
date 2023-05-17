
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



// ----------------- Shader source codes -----------------

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(0.8f, 0.8f, 0.02f, 1.0f);\n"
"}\n\0";

// -------------------------------------------------------



const int width = 800;
const int height = 600;



int main() {
	glfwInit();

#pragma region Creating a Window

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Index Buffers", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

#pragma endregion

	// Load the OpenGL libraries
	gladLoadGL();

	// Define the viewport
	glViewport(0, 0, width, height);

#pragma region Shaders and Shader-Compilation

	// Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete the shaders after they've been linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

#pragma endregion

#pragma region Vertex and Index Data

	GLfloat vertices[] = {
		0, 0, 0,
		0.5, 0.5, 0,
		0.5, -0.5, 0,
		0, -0.5, 0,
		-0.5, -0.5, 0,
		-0.5, 0.5, 0,
		0, 0.5, 0,
		0, 0.75, 0
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 1,
		5, 7, 6,
		1, 7, 6
	};

#pragma endregion 

#pragma region VAO, VBO and EBO

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	// Generate vertexArray and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the generated buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Point to VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);

#pragma endregion


	// While the window isn't closed
	while (!glfwWindowShouldClose(window)) {

		// Wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Clear the color buffer
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);

		glfwPollEvents();
	}

#pragma region Destroying Objects and Buffers	

	glfwDestroyWindow(window);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

#pragma endregion

	glfwTerminate();
	return 0;
}