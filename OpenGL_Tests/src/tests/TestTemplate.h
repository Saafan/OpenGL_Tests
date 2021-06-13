#pragma once
#include "Test.h"


namespace test
{

	class TestTemplate : public Test
	{
	public:
		TestTemplate() = default;
		TestTemplate(GLFWwindow * window);
		~TestTemplate();

		void OnLoad() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
