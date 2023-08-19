#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#define GLFW_INCLUDE_NONE
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace
{
	struct ShaderSource
	{
		std::string vertex_source;
		std::string fragment_source;
	};

	/// <summary>
	/// Parses the shader source code located at the provided filepath.
	/// 
	/// </summary>
	/// <param name="filepath">The path to the file containing shader source code.</param>
	/// <returns>The parsed shader source code.</returns>
	ShaderSource parse_shader(const std::string& filepath)
	{

		enum class ShaderType
		{
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1
		};

		std::ifstream ifs{ filepath };
		if (!ifs.good())
		{
			std::string error_msg = "Error opening file " + filepath;
			throw std::exception{ error_msg.c_str() };
		}

		std::string line;
		int line_no = 1;
		ShaderType type = ShaderType::NONE;

		// The source code is stored in a stringstream array with two 
		// elements: one for the vertex shader and one for the fragment shader
		std::stringstream ss[2];
		while (std::getline(ifs, line))
		{
			// The programs are delineated by the #shader tag
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::FRAGMENT;
				}
				else {
					std::string error_msg = "Unknown shader type found in line "
						+ std::to_string(line_no) + "of " + filepath + ": " + line;
					throw std::exception{ error_msg.c_str() };
				}
			}
			else {
				ss[(int)type] << line << "\n";
			}

			++line_no;
		}

		return ShaderSource{ ss[0].str(), ss[1].str() };
	}

	/// <summary>
	/// Compiles a shader from source code into the desired type.
	/// </summary>
	/// <param name="type">The type of shader to be compiled, eg. GL_VERTEX_SHADER.</param>
	/// <param name="source">The GLSL source code to be used to compile the shader</param>
	/// <returns>A integer id of the compiled shader.</returns>
	unsigned int compile_shader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		// The compilation status (pass or fail) is saved into 
		// the result variable
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		// If the compilation failed, the compile stastus will be false
		// and an error message should be printed to help with debugging
		if (result == GL_FALSE)
		{
			// To extract the error message, the length of the log file
			// is required
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			// The error message is saved into a message char with a 
			// large enough buffer to store the entire message
			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			// Finally, the message can be printed.
			std::cerr << "Failed to compile " << 
				(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
				" shader!" << std::endl;
			std::cout << message << std::endl;

			// Since the compilation failed, the created shader is useless
			// and can be deleted
			glDeleteShader(id);
		}
		return id;
	}

	/// <summary>
	/// Compiles and links a shader program comprised of a vertex and fragment shader
	/// using the OpenGL API.
	/// 
	/// Shaders are programs that run on the GPU.
	/// 
	/// </summary>
	/// <param name="vertex_shader">The source code of vertex shader.</param>
	/// <param name="fragment_shader">The source code of the fragment shader</param>
	/// <returns>The ID of the generated shader program.</returns>
	unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
	{
		// The vertex and fragment shaders will be compiled into a single shader program
		unsigned int program = glCreateProgram();
		unsigned int vs_id = compile_shader(GL_VERTEX_SHADER, vertex_shader);
		unsigned int fs_id = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

		// The successfully compiled shaders must be attached to the program prior
		// to linking
		glAttachShader(program, vs_id);
		glAttachShader(program, fs_id);
		glLinkProgram(program);
		glValidateProgram(program);

		// The program has been successfully linked, so the intermediate object files can be deleted
		glDeleteShader(vs_id);
		glDeleteShader(fs_id);

		return program;
	}
}

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

		// Shaders are required to tell the GPU how to render the vertices on the screen
		ShaderSource shader_source = parse_shader("../simple-window/res/shaders/basic.shader");
		unsigned int shader = create_shader(shader_source.vertex_source, shader_source.fragment_source);
		glUseProgram(shader);

		while (!glfwWindowShouldClose(window))
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		glDeleteProgram(shader);

		glfwDestroyWindow(window);

		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}