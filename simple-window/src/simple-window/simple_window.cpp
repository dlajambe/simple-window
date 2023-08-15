#include <iostream>
#define GLFW_INCLUDE_NONE
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

		// Checks if the GLFW library was successfully initialized
		if (!glfwInit())
			exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		GLFWwindow* window = glfwCreateWindow(640, 480, "Simple Window", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwSetKeyCallback(window, key_callback);
		glfwMakeContextCurrent(window);
		
		glfwSwapInterval(1);

		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
			glfwPollEvents();
		}


		glfwDestroyWindow(window);

		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}