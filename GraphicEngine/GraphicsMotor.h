#pragma once

#include "Window.h"
#include "VulkanHandler.h"

class GraphicsMotor {
protected:
	GraphicsMotor();
	static GraphicsMotor* singleton_;
public:

	//Can't be cloned
	GraphicsMotor(GraphicsMotor& other) = delete;
	//Can't be assignated
	void operator=(const GraphicsMotor&) = delete;

	static GraphicsMotor* GetInstance();


	~GraphicsMotor();


	void Update(float frameTime);
	void Render();

	void Shutdown();

	bool ExitCondition();

	GLFWwindow* GetGLFWWindow();
	VulkanHandler& GetVulkanHandler() { return vulkanHandler; }
private:
	Window window;
	VulkanHandler vulkanHandler;
};