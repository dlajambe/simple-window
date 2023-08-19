#shader vertex
#version 410 core
layout(location = 0) in vec4 position;
void main()
{
	gl_Position = position;
};

#shader fragment
#version 410 core
out vec4 Color;
void main()
{
	Color = vec4(1.0, 0.0, 1.0, 1.0);
};