#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 normals;
out vec3 fragPos;
out vec2 TexCoords;
void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0));
	normals = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoords;

	gl_Position = proj * view * vec4(fragPos, 1.0);

}

#shader fragment
#version 330 core
out vec4 color;

uniform vec3 cameraPos;
uniform vec3 lightPos;

in vec3 fragPos;
in vec3 normals;
in vec2 TexCoords;

uniform sampler2D depthTex;

void main()
{
	float dotProduct = dot(normalize(lightPos - fragPos), normalize(normals));
	float diffuse = clamp(dotProduct, 0.0f, 1.0f);

	color = texture(depthTex, TexCoords);
}