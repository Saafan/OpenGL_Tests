
#include "Renderer.h"
#include "imgui/imgui.h"

#include "ShadowMapping.h"
#include "Model.h"

test::ShadowMapping::ShadowMapping(GLFWwindow* window)
{
	this->window = window;
	OnLoad();
}

test::ShadowMapping::~ShadowMapping()
{


}

Model* plane = nullptr;
Shader* shader = nullptr;

void test::ShadowMapping::OnLoad()
{
	shader = new Shader("shaders/ShadowMapping.shader");
	plane = new Model(ModelType::Cube, shader);
	plane->Scale(glm::vec3(3.0));
}

void test::ShadowMapping::OnUpdate(float deltaTime)
{
	shader->Bind();
	plane->Render();
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


