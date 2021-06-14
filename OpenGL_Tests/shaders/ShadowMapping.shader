#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	TexCoords = aTexCoords;

	gl_Position = vec4(aPos, 1.0);
}


#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


out vec3 fragColor;
uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, TexCoords).xyz;
}