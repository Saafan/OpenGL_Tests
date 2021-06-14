
#include "Renderer.h"
#include "imgui/imgui.h"

#include "ShadowMapping.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "Light.h"

test::ShadowMapping::ShadowMapping(GLFWwindow* window)
{
	this->window = window;
	OnLoad();
}

test::ShadowMapping::~ShadowMapping()
{


}

Model* plane = nullptr;
Model* cube = nullptr;
Shader* shader = nullptr;
Shader* shader2 = nullptr;
Texture* tex = nullptr;
Camera* camera = nullptr;
FrameBuffer* fb = nullptr;
Material* mat = nullptr;
Light* light = nullptr;

void test::ShadowMapping::OnLoad()
{
	shader = new Shader("shaders/ShadowMapping.shader");
	shader2 = new Shader("shaders/LightingShader.shader");

	mat = new Material(shader2);
	mat->AddTexture("images/container2.png", aiTextureType_DIFFUSE, 0);

	plane = new Model(ModelType::Plane, shader);
	plane->SetPlaneParam(1.0f,1.0f);


	camera = new Camera(window, shader2);
	light = new Light(*shader2, *camera, LightType::Spot);
	light->SetSpotLightParam(glm::vec3(3.0f), glm::vec3(-3.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));


	cube = new Model(ModelType::Cube, shader2);

	tex = new Texture("images/container2.png");

	fb = new FrameBuffer(WIDTH / 2, HEIGHT / 2 );
}

void RenderScene()
{
	shader2->Bind();
	light->Render();
	camera->CalculateViewMatrix();
	
	cube->Render();
}

void test::ShadowMapping::OnUpdate(float deltaTime)
{
	fb->BindTexture();

	fb->Bind();

	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	RenderScene();
	fb->UnBind();
	
	glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	shader->Bind();
	fb->tex->Bind(0);
	shader->SetUniform1i("tex", 0);
	plane->Render();
	fb->tex->Unbind();
}


void test::ShadowMapping::OnRender()
{

}

void test::ShadowMapping::OnImGuiRender()
{
	static float lastTime;
	float fps = 1.0f / (glfwGetTime() - lastTime);
	lastTime = glfwGetTime();

	ImGui::Text("%f", fps);
}


