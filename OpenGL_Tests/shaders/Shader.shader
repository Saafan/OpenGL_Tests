#shader vertex
#version 330 core

layout(location = 0) in vec4 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model* vPosition;
}

#shader fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}