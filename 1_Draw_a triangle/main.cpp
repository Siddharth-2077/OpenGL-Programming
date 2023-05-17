
// Includes
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



int Width = 800;
int Height = 600;



int main() {

	glfwInit();

	/*
	* Tell(hint) the GLFW what version of OpenGL we're using.
	* In this case we're using OpenGL 3.3, hence we pass 3 (only int values allowed).
	* We need to hint the Major, Minor and the OpenGL profile version we're gonna use.
	* OpenGL profiles are Core (modern) and Compatibility (modern + legacy).
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window
	GLFWwindow* window = glfwCreateWindow(Width, Height, "OpenGL Window", nullptr, nullptr);
	if (!window) {
		std::cout << " >> ERROR: Failed to create a Window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Use the window
	glfwMakeContextCurrent(window);

	// Glad is a sort of 'function loader' for OpenGL queried from a web-server.
	// gladLoadGL() is used to dynamically load the OpenGL library.
	gladLoadGL();

	// Tells OpenGL the area of the Window that we want to render in (Viewport) 
	// Takes (x,y) the bottom left corner (usually 0,0) and width and height
	glViewport(0, 0, Width, Height);




	// BUILD AND COMPILE OUR SHADERS INTO A SHADER PROGRAM ----------------------------------------

	// Create and Compile the Vertex and Fragment shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);			// Compile the shader for the GPU

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);		// Compile the shader for the GPU

	/*
	* A shader program object is the final linked version of multiple shaders combined
	* The activated shader program's shaders will be used when we issue render calls.
	*
	* When linking the shaders into a program it links the outputs of each shader to the inputs of the next shader.
	* This is also where you'll get linking errors if your outputs and inputs do not match.
	*/
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// We don't need the shader objects anymore, since they've been combined in a shader program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// --------------------------------------------------------------------------------------------



	/*
	* Create the vertices to draw the Triangle.
	* NOTE: The coordinates are in a Normalized Window.
	*
	* REFER TO LINK: "Creating a Triangle OpenGL"
	*/
	GLfloat triangle_vertices[] = {
		-0.5f, -0.5f, 0,
		0.5f, -0.5f, 0,
		0.0f, 0.5f, 0
	};

	GLuint VAO;			// Reference to Vertex Array Object
	GLuint VBO;			// Reference to Vertex Buffer Object

	// Generate the VAO and VBO arrays with only 1 object each (order is important)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	// Config the Vertex Attribute Pointer so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof(float)), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows how to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0, so that we don't accidentally modify the VAO and VBO later
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);





	// While the window isn't closed
	while (!glfwWindowShouldClose(window)) {

		// RGB-A
		// (Prepares to) Set a new value for the color of the back-buffer
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Does the actual setting (as specified above)
		glClear(GL_COLOR_BUFFER_BIT);


		// Used for render calls (Tell OpenGL which shader program we wanna use)
		glUseProgram(shaderProgram);
		// Bind the VAO so that OpenGL knows to use it
		glBindVertexArray(VAO);
		// Draw the Triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/*
		* We now have a back-buffer with the color we wish to set for the window, and the triangle we wanna draw.
		* And, we have a front-buffer with the default window.
		* We need to Swap the 2 buffers for our changes to reflect and set our custom color, and draw the triangle.
		*/
		glfwSwapBuffers(window);


		// Keep polling for events such as: Window close, Window resizing etc.
		glfwPollEvents();
	}



	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);



	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}