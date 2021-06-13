#pragma once

#include <functional>
#include <vector>
#include <string>
#include <iostream>

#include <GLFW/glfw3.h>

namespace test
{
	class Test
	{
	public:
		Test() {}
		Test(GLFWwindow* window) : window(window) {}
		virtual ~Test() {}

		virtual void OnLoad() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		GLFWwindow* window = nullptr;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTest);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test " << name << std::endl;

			tests.push_back(std::make_pair(name, []() { return new T(nullptr); }));
		}

		template<typename T>
		void RegisterTest(const std::string& name, GLFWwindow* window)
		{
			std::cout << "Registering Test " << name << std::endl;

			tests.push_back(std::make_pair(name, [=]() { return new T(window); }));
		}

		Test*& currentTest;
	private:
		std::vector<std::pair<std::string, std::function<Test* ()>>> tests;
	};

}