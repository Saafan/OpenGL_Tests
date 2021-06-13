#include "Test.h"
#include "imgui/imgui.h"

namespace test
{

	TestMenu::TestMenu(Test*& currentTest) : currentTest(currentTest)
	{	

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : tests)
		{
			if (ImGui::Button(test.first.c_str()))
				currentTest = test.second();
			
		}
	}

}

