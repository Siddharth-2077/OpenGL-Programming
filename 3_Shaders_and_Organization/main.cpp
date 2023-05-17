
/*
	A simple OpenGL program to draw a Triangle with a custom Vertex and Fragment Shader

	- To draw an RGB triangle with interpolated colors
	- To keep shader source code separate from the main code

*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



// A struct that will hold the vertex and frag shader source codes (read from file)
struct ShaderSource {
	std::string VertexShaderSource{};
	std::string FragmentShaderSource{};
};



// Parses the shader from the file and returns struct: 
static ShaderSource ParseShader(const std::string& filepath) {

	std::ifstream stream (filepath);		// Open the shader file

	std::string line;
	std::stringstream ss[2];				// Vertex shader code -> 0 | Frag shader code -> 1

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	ShaderType shaderType = ShaderType::NONE;

	while (getline(stream, line)) {			// Read the file-line into 'line' variable

		if (line.find("#shader") != std::string::npos) {
			// Found the beginning of a shader
			if (line.find("vertex") != std::string::npos) {
				shaderType = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				shaderType = ShaderType::FRAGMENT;
			}

		}
		else {
			// Add the shader-code to the buffer (set by shaderType)
			if (shaderType != ShaderType::NONE)
				ss[(int)shaderType] << line << '\n';

		}

	}

	// Return the shader source codes
	return ( ShaderSource{ ss[0].str(), ss[1].str() });

}


// Global Variables
const int width{ 800 };					// Window width
const int height{ 600 };				// Window height

bool VERBOSE = true;					// Enable logging or not
bool WIREFRAME_MODE = false;			// Wireframe mode or Fill mode



int main() {
	glfwInit();

#pragma region Creating a Window and Viewport

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Triangle Exercise", nullptr, nullptr);
	if (!window) {
		std::cout << " >> Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

#pragma endregion 

	gladLoadGL();

	glViewport(0, 0, width, height);

#pragma region Shader Compilation

	ShaderSource source = ParseShader("Resources/Shaders/BasicTriangle.shader");

	const char* vertexSource = source.VertexShaderSource.c_str();
	const char* fragmentSource = source.FragmentShaderSource.c_str();

	if (VERBOSE) {		
		std::cout << "VERTEX SHADER" << std::endl;
		std::cout << vertexSource << std::endl;

		std::cout << "FRAGMENT SHADER" << std::endl;
		std::cout << fragmentSource << std::endl;
	}

	// Vertex Shader	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	// Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	// Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);	

	// Delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

#pragma endregion 

#pragma region Vertex and Index data

	// Vertex data
	GLfloat vertices[] = {
		// positions			// colors
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f   // top  
	};

	// Index data
	GLuint indices[] = {
		0, 1, 2
	};

#pragma endregion 

#pragma region VAO, VBO and EBO

	GLuint VAO;		// Vertex Attribute Object ref
	GLuint VBO;		// Vertex Buffer Object ref
	GLuint EBO;		// Element Buffer Object ref

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Point to the X,Y,Z vertex data in the VBO (at layout location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)0);			
	glEnableVertexAttribArray(0);

	// Point to the RGB color data in the VBO (at layout location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);				// unbind VBO
	glBindVertexArray(0);							// unbind VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		// unbind EBO (contained in VAO)

#pragma endregion 

	// While window isn't closed
	while (!glfwWindowShouldClose(window)) {

		if (WIREFRAME_MODE) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// Wireframe mode
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		// Fill mode
		}

		glClearColor(0, 0, 0, 1);							// black background
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Indexed Draw calls
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

#pragma region Cleanup

	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glfwDestroyWindow(window);

#pragma endregion

	glfwTerminate();
	return 0;
}