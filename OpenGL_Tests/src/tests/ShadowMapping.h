#pragma once
#include "Test.h"


namespace test
{

	class ShadowMapping : public Test
	{
	public:
		ShadowMapping() = default;
		ShadowMapping(GLFWwindow * window);
		~ShadowMapping();

		void OnLoad() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
