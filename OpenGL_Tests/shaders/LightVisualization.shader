#shader vertex
#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vPosition;
}

#shader fragment
#version 330 core

out vec4 color;

uniform vec3 lightColor;

void main()
{
	color = vec4(lightColor, 1.0f);
}