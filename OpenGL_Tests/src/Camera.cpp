#include "Camera.h"
#include <iostream>
#include "Model.h"

glm::vec3 Camera::cameraPos = glm::vec3(1.0f);
glm::vec3 Camera::cameraFront = glm::vec3(0.0f);
float Camera::speed = 2.5f;
glm::dvec2 Camera::currentMousePos = glm::vec2(0.0f);
glm::vec2 Camera::mouseDelta = glm::vec2(0.0f);

const glm::vec3 Camera::RayCast(float t)
{
	// Get Mouse Position Device Normalized
	const glm::vec3 currentScreenMousePos((2.0f * currentMousePos.x) / WIDTH - 1.0f,
		1.0f - (2.0f * currentMousePos.y / HEIGHT),
		1.0f);

	// Get Mouse Position Clip Coordinates
	const glm::vec3& screenPos = currentScreenMousePos;
	glm::vec4 rayClip = glm::vec4(screenPos.x, screenPos.y, -1.0f, 1.0f);

	//Get Mouse Position Eye Coordinates
	if (!projMatrix)
	{
		std::cout << "Projection Matrix isn't defined!" << std::endl;
		return glm::vec3(-1.0f);
	}
	glm::vec4 eyePos = glm::transpose(glm::inverse(*projMatrix)) * rayClip;
	eyePos.z = -1.0f;
	eyePos.w = 0.0f;

	//Get Mouse Position World Position
	glm::vec4 rayWorld3 = glm::transpose(glm::inverse(*viewMatrix)) * eyePos;
	glm::vec3 rayWorld = glm::vec3(rayWorld3.x, rayWorld3.y, rayWorld3.z);
	rayWorld = glm::normalize(rayWorld);

	return cameraPos + rayWorld * t;
}

float GetDeltaTime()
{
	static float currentTime = 0.0f;
	static float oldTime = 0.0f;
	float deltaTime = 0.0f;
	currentTime = glfwGetTime();
	deltaTime = currentTime - oldTime;
	oldTime = glfwGetTime();
	return deltaTime;
}

void HandleKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{

		if (key == GLFW_KEY_GRAVE_ACCENT)
			glfwSetInputMode(window, GLFW_CURSOR, glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
		if (key == GLFW_KEY_W)
			Camera::cameraPos += Camera::cameraFront;
		if (key == GLFW_KEY_S)
			Camera::cameraPos -= Camera::cameraFront;

		glm::vec3 right = glm::cross(Camera::cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

		if (key == GLFW_KEY_A)
			Camera::cameraPos -= right;
		if (key == GLFW_KEY_D)
			Camera::cameraPos += right;
	}
}

void HandleScrollInput(GLFWwindow* window, double xOffset, double yOffset)
{
	glm::vec3 cameraDirection = glm::normalize(Camera::cameraFront - Camera::cameraPos);

	Camera::cameraPos += Camera::cameraFront * (float)yOffset;
}

void HandleMouseInput(GLFWwindow* window, double xPos, double yPos)
{
	static glm::vec2 oldMosuePos;
	static glm::vec2 currentMosuePos;

	static bool firstTime = true;

	static glm::vec2 oldMousePos;

	Camera::currentMousePos.x = (float)xPos;
	Camera::currentMousePos.y = (float)yPos;

	if (!firstTime)
	{
		Camera::mouseDelta.x = -(oldMousePos.x - Camera::currentMousePos.x);
		Camera::mouseDelta.y = (oldMousePos.y - Camera::currentMousePos.y);
	}
	else
		firstTime = false;

	oldMousePos.x = (float)xPos;
	oldMousePos.y = (float)yPos;
}




Camera::Camera(GLFWwindow* window, Shader* shader, float speed, unsigned int viewUniformLocation, unsigned int projUniformLocation)
{
	this->window = window;
	this->speed = speed;
	this->cameraShader = shader;
	this->viewUniformLocation = viewUniformLocation;
	this->projUniformLocation = projUniformLocation;
	glfwSetKeyCallback(this->window, HandleKeyboardInput);
	glfwSetCursorPosCallback(this->window, HandleMouseInput);
	glfwSetScrollCallback(this->window, HandleScrollInput);
	SetCursorLock(true);

	if (projUniformLocation != -1)
		SetProjectionMatrix(projUniformLocation, WIDTH, HEIGHT, this->fovAngle);

	CalculateViewMatrix();
}


void Camera::SetCameraPositionUniform(std::string uniformName)
{
	if (uniformName.empty())
		return;
	cameraPositionUniform = uniformName;
}

std::string& Camera::GetCameraPositionUniform()
{
	return cameraPositionUniform;
}

glm::mat4& Camera::GetCameraViewMatrix()
{
	return *viewMatrix;
}

const glm::vec2& Camera::GetCameraPitchYaw()
{
	return glm::vec2(pitch, yaw);
}

void Camera::SetSpeed(float speed)
{
	this->speed = speed;
}

void Camera::SetFOVAngle(float fov)
{
	this->fovAngle = fov;
	if (projUniformLocation != -1)
		SetProjectionMatrix(projUniformLocation, WIDTH, HEIGHT, this->fovAngle);
}

void Camera::SetCursorLock(bool locked)
{
	glfwSetInputMode(window, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Camera::SetWindow(GLFWwindow* window)
{
	this->window = window;
}

glm::mat4 Camera::SetProjectionMatrix(unsigned int projUniformLocation, float width, float height, float fovAngle)
{
	if (cameraShader)
		cameraShader->Bind();
	glm::mat4* projMat = new glm::mat4(1.0f);
	*projMat = glm::perspective(glm::radians(fovAngle), width / height, 0.1f, 1000.0f);
	this->projUniformLocation = projUniformLocation;

	if (this->projUniformLocation != -1)
		cameraShader->SetProjMatrix(*projMat);
	this->projMatrix = projMat;
	return *projMat;
}

void Camera::SetViewUniformLocation(unsigned int viewUniformLocation)
{
	this->viewUniformLocation = viewUniformLocation;
}

glm::mat4& Camera::CalculateViewMatrix()
{
	if (cameraShader)
		cameraShader->Bind();

	float deltaTime = GetDeltaTime();
	float totalSpeed = speed * deltaTime;

	mouseDelta.x *= totalSpeed;
	mouseDelta.y *= totalSpeed;

	yaw += mouseDelta.x;
	pitch += mouseDelta.y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	cameraFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	cameraFront.y = std::sin(glm::radians(pitch));
	cameraFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

	static bool firstTime = true;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		{
			static glm::vec3 center;
			if (firstTime)
			{
				center = cameraFront + cameraPos;
				firstTime = false;
			}

			cameraPos.x = -std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
			cameraPos.y = -std::sin(glm::radians(pitch));
			cameraPos.z = -std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

			cameraPos += center;
		}
		else
		{
			glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), cameraFront));
			glm::vec3 up = glm::normalize(glm::cross(right, cameraFront));

			cameraPos -= (up * mouseDelta.y);
			cameraFront -= up * mouseDelta.y * deltaTime;

			cameraPos -= right * mouseDelta.x;
			cameraFront -= right * mouseDelta.x * deltaTime;
		}
	}
	else
		firstTime = true;

	cameraFront = glm::normalize(cameraFront);

	//#OPTIMIZE 
	glm::mat4* viewMat = new glm::mat4(1.0f);
	*viewMat = glm::lookAt(cameraPos, cameraFront + cameraPos, glm::vec3(0.0f, 1.0f, 0.0f));
	if (viewUniformLocation != -1)
		cameraShader->SetViewMatrix(*viewMat);

	viewMatrix = viewMat;

	return *viewMat;
}

void Camera::ReportMouseDelta()
{
	std::cout << "(x: " << mouseDelta.x << ", y:" << mouseDelta.y << ")" << std::endl;
}

void Camera::ReportMousePosition()
{
	std::cout << "(x: " << currentMousePos.y << ", y:" << currentMousePos.x << ")" << std::endl;
}

void Camera::ReportYawPitch()
{
	std::cout << "(yaw: " << yaw << ", pitch:" << pitch << ")" << std::endl;
}

