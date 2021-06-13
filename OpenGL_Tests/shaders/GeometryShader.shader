#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 normals;

out VS_OUT{
	vec3 normal;
} vs_out;

void main()
{
	gl_Position = view * model * vec4(aPos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}


#shader geometry
#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 36) out;

vec4 TriangleDistance(vec4 position, vec4 normal)
{
	return position + normal * 0.1;
}

uniform mat4 proj;
in VS_OUT{
	vec3 normal;
} gs_in[];

vec3 GetNormal()
{
	vec3 v1 = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 v2 = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(v1, v2));
}

void main()
{
	const float MAGNITUDE = 0.05f;
	gl_Position = proj * gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = proj * vec4(gl_in[0].gl_Position.xyz + gs_in[0].normal * MAGNITUDE,1.0);
	EmitVertex();

	EndPrimitive();
}

#shader fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(0.0f, 0.0f, 0.7f, 1.0f);
}