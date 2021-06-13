#pragma once
#include "DefaultNamingConventions.h"
#include "Renderer.h"
#include "Camera.h"
#include "Model.h"
#include "glm/gtc/type_ptr.hpp"

enum class LightType
{
	Directional = 0,
	Point = 1,
	Spot = 2
};




//This Class's implementation Assumes that all the Structs Uniforms in the Shader Program are arrays

class Light
{
public:
	Light(Shader& shader, Camera& camera, LightType type = LightType::Point, int lightIndex = 0, std::string structName = "");

	void SetStructName(std::string structName);
	std::string structName;

	glm::vec3 position;
	glm::vec3 direction;

	void RenderDirectional();
	void UpdateCameraPosition();
	void RenderSpot();
	void RenderPoint();

	void SetLightColorParam(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void SetLightCutoffAngles(float cutoff = DEFAULT_SPOTLIGHT_CUTOFF, float outerCutOff = DEFAULT_SPOTLIGHT_OUTERCUTOFF);

	void SetDirLightParam(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void SetPointLightParam(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant = DEFAULT_LIGHT_CONSTANT, float linear = DEFAULT_LIGHT_LINEAR, float quadratic = DEFAULT_LIGHT_QUADRATIC);
	void SetSpotLightParam(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff = DEFAULT_SPOTLIGHT_CUTOFF, float outerCutOff = DEFAULT_SPOTLIGHT_OUTERCUTOFF, float constant = DEFAULT_LIGHT_CONSTANT, float linear = DEFAULT_LIGHT_LINEAR, float quadratic = DEFAULT_LIGHT_QUADRATIC);

	void SetPosition(glm::vec3 pos);
	void SetPosition(float x, float y, float z);

	void SetAmbient(glm::vec3 ambient);
	void SetAmbient(float r, float g, float b);
	void SetDiffuse(glm::vec3 diffuse);
	void SetDiffuse(float r, float g, float b);
	void SetSpecular(glm::vec3 specular);
	void SetSpecular(float r, float g, float b);

	void Render();
	void RenderVisualizationModel();
	void VisualizeLight(bool visualize = true);

	void SetDefaultData();

	void SetAttenuation(float constant = DEFAULT_LIGHT_CONSTANT, float linear = DEFAULT_LIGHT_LINEAR, float quadratic = DEFAULT_LIGHT_QUADRATIC);
private:
	float cutOff = DEFAULT_SPOTLIGHT_CUTOFF;
	float outerCutOff = DEFAULT_SPOTLIGHT_OUTERCUTOFF;

	float constant = DEFAULT_LIGHT_CONSTANT;
	float linear = DEFAULT_LIGHT_LINEAR;
	float quadratic = DEFAULT_LIGHT_QUADRATIC;

	int lightIndex;

	bool visualize = true;
	

	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;

	Shader* shader = nullptr;
	Camera* camera = nullptr;
	LightType type = LightType::Point;
};