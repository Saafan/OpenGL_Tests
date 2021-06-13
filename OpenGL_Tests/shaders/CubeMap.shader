#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 proj;
uniform mat4 view;

void main()
{
	TexCoord = aPos;
	gl_Position = proj * view * vec4(aPos, 1.0f);
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, TexCoord);
}