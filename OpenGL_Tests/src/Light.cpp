#include "Light.h"


Light::Light(Shader& shader, Camera& camera, LightType type, int lightIndex, std::string structName)
{
	this->shader = &shader;
	this->type = type;
	this->lightIndex = lightIndex;
	this->camera = &camera;
	SetDefaultData();
	SetStructName(structName);

	SetAttenuation();

	ambient = DEFAULT_AMBIENT_COLOR, diffuse = DEFAULT_DIFFUSE_COLOR, specular = DEFAULT_SPECULAR_COLOR, direction = DEFAULT_LIGHT_DIRECTION;
}


void Light::SetStructName(std::string structName)
{
	if (structName.empty())
		return;
	this->structName = structName + "[" + std::to_string(lightIndex) + "].";
}



void Light::UpdateCameraPosition()
{
	GLCall(shader->SetUniform3f(camera->GetCameraPositionUniform(), camera->cameraPos));
}

void Light::RenderDirectional()
{
	GLCall(shader->SetUniform3f(structName + "direction", direction.x, direction.y, direction.z));
	GLCall(shader->SetUniform3f(structName + "ambient", ambient.x, ambient.y, ambient.z));
	GLCall(shader->SetUniform3f(structName + "diffuse", diffuse.x, diffuse.y, diffuse.z));
	GLCall(shader->SetUniform3f(structName + "specular", specular.x, specular.y, specular.z));
}

void Light::RenderPoint()
{
	GLCall(shader->SetUniform3f(structName + "position", position.x, position.y, position.z));

	GLCall(shader->SetUniform1f(structName + "constant", constant));
	GLCall(shader->SetUniform1f(structName + "linear", linear));
	GLCall(shader->SetUniform1f(structName + "quadratic", quadratic));

	GLCall(shader->SetUniform3f(structName + "ambient", ambient.x, ambient.y, ambient.z));
	GLCall(shader->SetUniform3f(structName + "diffuse", diffuse.x, diffuse.y, diffuse.z));
	GLCall(shader->SetUniform3f(structName + "specular", specular.x, specular.y, specular.z));

}

void Light::SetLightColorParam(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void Light::RenderSpot()
{
	GLCall(shader->SetUniform3f(structName + "position", position.x, position.y, position.z));
	GLCall(shader->SetUniform3f(structName + "direction", direction.x, direction.y, direction.z));

	GLCall(shader->SetUniform1f(structName + "constant", constant));
	GLCall(shader->SetUniform1f(structName + "linear", linear));
	GLCall(shader->SetUniform1f(structName + "quadratic", quadratic));

	GLCall(shader->SetUniform1f(structName + "cutOff", glm::cos(glm::radians(cutOff))));
	GLCall(shader->SetUniform1f(structName + "outerCutOff", glm::cos(glm::radians(outerCutOff))));

	GLCall(shader->SetUniform3f(structName + "ambient", ambient.x, ambient.y, ambient.z));
	GLCall(shader->SetUniform3f(structName + "diffuse", diffuse.x, diffuse.y, diffuse.z));
	GLCall(shader->SetUniform3f(structName + "specular", specular.x, specular.y, specular.z));
}

void Light::SetLightCutoffAngles(float cutoff, float outerCutOff)
{
	this->cutOff = cutoff;
	this->outerCutOff = outerCutOff;
}

void Light::SetDirLightParam(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->direction = direction;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}

void Light::SetPointLightParam(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
{
	this->position = position;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::SetSpotLightParam(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
{
	this->position = position;
	this->direction = direction;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
}

void Light::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Light::SetPosition(glm::vec3 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
}

void Light::SetAmbient(glm::vec3 ambient)
{
	this->ambient = ambient;
}

void Light::SetAmbient(float r, float g, float b)
{
	this->ambient.x = r;
	this->ambient.y = g;
	this->ambient.z = b;
}

void Light::SetDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void Light::SetDiffuse(float r, float g, float b)
{
	this->diffuse.x = r;
	this->diffuse.y = g;
	this->diffuse.z = b;
}

void Light::SetSpecular(glm::vec3 specular)
{
	this->specular = specular;
}

void Light::SetSpecular(float r, float g, float b)
{
	this->specular.x = r;
	this->specular.y = g;
	this->specular.z = b;
}

//Add the pointer to the Camera to visualize the light properties in the scene
void Light::Render()
{
	UpdateCameraPosition();

	if (type == LightType::Directional)
		RenderDirectional();
	else if (type == LightType::Point)
		RenderPoint();
	else if (type == LightType::Spot)
		RenderSpot();
	if (visualize)
		RenderVisualizationModel();
}


void Light::RenderVisualizationModel()
{
	static auto* lightShader = new Shader("shaders/LightVisualization.shader");
	static auto* visualizationCube = new Model(ModelType::Cube, lightShader);
	static auto* directionLine = new Model(ModelType::Line, lightShader);

	if (type != LightType::Directional)
		visualizationCube->SetCubeParam(0.25f, 0.25f, 0.25f);
	else
		visualizationCube->SetPlaneParam(0.5f, 0.5f);

	visualizationCube->Translate(position);

	glm::vec3 vizColor = glm::vec3(ambient.r * diffuse.r, ambient.g * diffuse.g, ambient.b * diffuse.b);
	if (camera->viewMatrix && camera->projMatrix)
	{
		lightShader->Bind();
		lightShader->SetUniform3f("lightColor", vizColor);
		lightShader->SetMVPMatrix(visualizationCube->GetModelMatrix(), *camera->viewMatrix, *camera->projMatrix);
	}
	visualizationCube->LookAt(direction);

	lightShader->SetUniform3f("lightColor", vizColor.x * 0.2f, vizColor.y * 0.2f, vizColor.z * 0.2f);
	lightShader->SetUniform3f("lightColor", vizColor.x * 0.75f, vizColor.y * 0.75f, vizColor.z * 0.75f);

	if (type == LightType::Spot)
	{
		directionLine->RenderWireCone(position, direction, cutOff, 2.5f);
		directionLine->RenderWireCone(position, direction, outerCutOff, 2.5f);
	}

	visualizationCube->Render();
}

void Light::SetDefaultData()
{
	if (type == LightType::Directional)
		structName = "dirLights[" + std::to_string(lightIndex) + "].";
	else if (type == LightType::Spot)
		structName = "spotLights[" + std::to_string(lightIndex) + "].";
	else if (type == LightType::Point)
		structName = "pointLights[" + std::to_string(lightIndex) + "].";
}

void Light::SetAttenuation(float constant, float linear, float quadratic)
{
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void Light::VisualizeLight(bool visualize)
{
	this->visualize = visualize;
}
