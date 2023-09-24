#pragma once

#include "Window.h"
#include "VulkanHandler.h"

class GraphicsMotor {

public:

	GraphicsMotor(ParticulesData* _particulesData);
	~GraphicsMotor();

	GraphicsMotor(const GraphicsMotor&) = delete;
	GraphicsMotor& operator=(const GraphicsMotor&) = delete;

	void Update(float frameTime);
	void Render();

	void Shutdown();

	bool ExitCondition();

	GLFWwindow* GetGLFWWindow();
private:
	Window window;
	VulkanHandler vulkanHandler;
};