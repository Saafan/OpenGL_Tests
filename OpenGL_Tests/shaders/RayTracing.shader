#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
	gl_Position = model * vec4(aPos, 1.0f);
}


#shader fragment
#version 330 core

out vec3 fragColor;

uniform float t;

const float WIDTH = 3840;
const float HEIGHT = 2160;

bool PlaneIntersect(vec3 rayOrigin, vec3 rayDir, vec3 planeOrigin, vec3 planeNormal, inout vec3 intersectPos)
{
	float d = dot(rayDir, planeNormal);
	if (d < 0.00001f)
	{
		float distance = dot((planeOrigin - rayOrigin), planeNormal) / d;
		intersectPos = rayOrigin + rayDir * distance;
		return true;
	}
	else
		return false;
}

bool SphereIntersect(vec3 o, vec3 dir, vec3 c, float radius, inout vec3 intersectPos, inout vec3 normal)
{
	vec3 u = normalize(dir);
	float delta = pow(dot(u, (o - c)), 2) - (pow(length(o - c), 2) - pow(radius, 2));
	float d = min(-(dot(u, (o - c))) + sqrt(delta), -(dot(u, (o - c))) - sqrt(delta));
	intersectPos = o + d * u;
	normal = normalize(intersectPos - c);
	if (delta >= 0)
		return true;
	else
		return false;
}


struct Sphere
{
	vec3 color;
	vec3 center;
	float radius;
	float reflectivity;
};

struct Plane
{
	vec3 color;
	vec3 normal;
};

struct Light
{
	vec3 pos;
	vec3 color;
};

const int NUM_OF_SPHERES = 3;
const int NUM_OF_LIGHTS = 1;

Sphere spheres[NUM_OF_SPHERES];
Light lights[NUM_OF_LIGHTS];


void Rotate(inout vec3 pixel, vec3 point, float pitch, float  yaw, float  roll) {

	vec3 tPoint = vec3(pixel - point);

	float cosa = cos(radians(roll));
	float sina = sin(radians(roll));
	float cosb = cos(radians(pitch));
	float sinb = sin(radians(pitch));
	float cosc = cos(radians(yaw));
	float sinc = sin(radians(yaw));
	float Axx = cosa * cosb;
	float Axy = cosa * sinb * sinc - sina * cosc;
	float Axz = cosa * sinb * cosc + sina * sinc;
	float Ayx = sina * cosb;
	float Ayy = sina * sinb * sinc + cosa * cosc;
	float Ayz = sina * sinb * cosc - cosa * sinc;
	float Azx = -sinb;
	float Azy = cosb * sinc;
	float Azz = cosb * cosc;


	vec3 ttPoint = tPoint;

	tPoint.x = Axx * ttPoint.x + Axy * ttPoint.y + Axz * ttPoint.z;
	tPoint.y = Ayx * ttPoint.x + Ayy * ttPoint.y + Ayz * ttPoint.z;
	tPoint.z = Azx * ttPoint.x + Azy * ttPoint.y + Azz * ttPoint.z;

	tPoint += point;

	pixel = tPoint;
}

uniform float pitch;
uniform float yaw;

vec3 intersectPos;
vec3 normal;

bool rayTrace = true;
int sphereIndex = -1;

const int MAX_RAYS = 20;
int rayNum = 0;
float diffuse = 1.0f;
vec3 result = vec3(0, 0, 0);

uniform sampler2D HDRItex;
bool first = true;
vec3 firstIP = vec3(0, 0, 0);
int firstSphereIndex = -1;

float atan2(in float y, in float x)
{
	bool s = (abs(x) > abs(y));
	return mix(3.1415f / 2.0 - atan(x, y), atan(y, x), s);
}

vec3 HDRI()
{

	float u, v;
	vec3 d;
	if (firstSphereIndex == -1)
		d = normalize(normal);
	else
		d = normalize(firstIP - spheres[sphereIndex].center);
	u = 0.5f + atan2(d.x, d.z) / (2 * 3.1415f);
	v = 0.5f - asin(d.y) / 3.1415f;


	return texture(HDRItex, vec2(u, v)).xyz ;
}

vec3 RayTrace(vec3 origin, vec3 rayDir)
{

	if (rayNum++ > MAX_RAYS)
		rayTrace = false;

	int i;
	for (i = 0; i < NUM_OF_SPHERES; i++)
	{
		if (SphereIntersect(origin, rayDir, spheres[i].center, spheres[i].radius, intersectPos, normal))
		{
			if (sphereIndex == i)
				continue;
			sphereIndex = i;
			int j;
			if (first)
			{
				for (j = 0; j < NUM_OF_LIGHTS; j++)
				{
					diffuse = dot(normalize(lights[j].pos - intersectPos), normal);
				}
				firstIP = intersectPos;
				firstSphereIndex = i;
				first = false;
			}
			return spheres[i].color;
		}
	}

	//Create Circular Patterns
	if (PlaneIntersect(origin, rayDir, vec3(0, 0, 0), vec3(0, 1, 0), intersectPos))
	{
		rayTrace = false;
		normal = vec3(0, 1, 0);
		if (sin(distance(intersectPos, vec3(0.0f)) / 100) > 0.0f)
			return vec3(0.86, 0.16, 0.21);
		else
			return vec3(0.29, 0.05, 0.07);
	}
	else
	{
		rayTrace = false;
		return HDRI();
	}

}

uniform vec3 motion;
void main()
{
	spheres[0].color = vec3(1, 0, 1);
	spheres[0].center = vec3((WIDTH / 2), (HEIGHT / 2), -50);
	spheres[0].radius = 300.0f;
	spheres[0].reflectivity = 1.0f;

	spheres[1].color = vec3(1, 0, 0);
	spheres[1].center = vec3((WIDTH / 2) - 700, (HEIGHT / 2), -50);
	spheres[1].radius = 300.0f;
	spheres[1].reflectivity = 0.5f;

	spheres[2].color = vec3(0, 0, 1);
	spheres[2].center = vec3((WIDTH / 2) + 700, (HEIGHT / 2), -50);
	spheres[2].radius = 300.0f;
	spheres[2].reflectivity = 0.2f;


	lights[0].pos = vec3((WIDTH / 2), (HEIGHT / 2), 1000);

	float radius = 100.0f;

	float fovAngle = 30.0f;

	float zPoint = (WIDTH / 2) / (tan(radians(fovAngle)));
	vec3 projPoint = vec3(WIDTH / 2 + motion.x, HEIGHT / 2 + motion.y, zPoint + motion.z);

	vec3 pixel = vec3(gl_FragCoord.x + motion.x, gl_FragCoord.y + motion.y, motion.z);
	Rotate(pixel, projPoint, yaw, pitch, 0.0f);

	vec3 rayDir = normalize(pixel - projPoint);

	intersectPos = pixel;
	normal = rayDir;

	while (rayTrace)
	{
		result = RayTrace(intersectPos, normal);
	}


	fragColor = result * diffuse;

}