#include "TestTemplate.h"

#include "Renderer.h"
#include "imgui/imgui.h"


test::TestTemplate::TestTemplate(GLFWwindow* window)
{
	this->window = window;
	OnLoad();
}

test::TestTemplate::~TestTemplate()
{


}


void test::TestTemplate::OnLoad()
{


}

void test::TestTemplate::OnUpdate(float deltaTime)
{

}


void test::TestTemplate::OnRender()
{

}

void test::TestTemplate::OnImGuiRender()
{
	static float lastTime;
	float fps = 1.0f / (glfwGetTime() - lastTime);
	lastTime = glfwGetTime();

	ImGui::Text("%f", fps);
}


