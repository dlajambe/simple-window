#include <iostream>
//#define GLFW_INCLUDE_NONE
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace simple_window
{
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	void create_window()
	{
		glfwSetErrorCallback(error_callback);

		// Step 1 - Initialize the GLFW library
		if (!glfwInit())
			exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		// Step 2 - Create a window and set callbacks
		GLFWwindow* window = glfwCreateWindow(640, 480, "Simple Window", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwSetKeyCallback(window, key_callback);

		// Step 3 - Initialize the context
		glfwMakeContextCurrent(window);
		std::cout << "GL Connection: " << glGetString(GL_VERSION) << std::endl;

		// Step 4 - With the context initialized, GLEW can be loaded. GLEW loads
		// all of the modern OpenGL function implementations from the GPU drivers
		// so that they can be used even though Windows stopped supporting Open GL
		// decades ago
		if (glewInit() != GLEW_OK)
		{
			throw std::exception("GLEW initialization with glewInit() failed");
			exit(EXIT_FAILURE);
		}

		// Step 5 - Create a buffer
		// To render anything, at least one buffer is required. A buffer ID is required
		unsigned int buffer_id;
		glGenBuffers(1, &buffer_id);

		// Binding selects the buffer to which data are to be added
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

		// Sample data to draw a triangle
		const std::size_t data_size = 6;
		float positions[data_size] = {
			-0.5f, 0.5f,
			0.0f, -0.5f,
			0.5f, 0.5f
		};

		// Step 6 - Pass the data to the buffer
		// Remember - OpenGL is a highly stateful API; the glBufferData function adds the data
		// to whichever buffer is currently bound
		glBufferData(GL_ARRAY_BUFFER, data_size * sizeof(float), positions, GL_STATIC_DRAW);

		// Step 7 - Tell OpenGL how the buffer's data are structured so that it can interpret them
		// corectly while rendering. Once again, due to the stateful nature of OpenGL's API, this
		// is applied automatically to whatever buffer is currently bound

		// The first (and only) attribute (index 0) is the vertex position.
		// Each position is 2 floats, so the count is 2 and the slider is 2 * 8 bytes = 16 bytes
		// There is no offset, so the final argument is set to 0
		constexpr int attribute_idx = 0;
		glVertexAttribPointer(attribute_idx, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		// The attribute is only used if it is enabled with the following function
		glEnableVertexAttribArray(attribute_idx);

		// This controls how often the buffer being displayed is swapped out for the one being rendered
		// A value of 1 indicates the buffers should be swapped every screen update
		glfwSwapInterval(1);

		while (!glfwWindowShouldClose(window))
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}


		glfwDestroyWindow(window);

		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}