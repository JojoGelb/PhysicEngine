#pragma once

#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {

public:
	Window(int width, int height, std::string name);
	~Window();

	//Handle inputs
	void Update();

	void Shutdown();

	bool ShouldClose();
	VkExtent2D GetExtent();
	bool WasWindowResized() { return framebufferResized; }
	void ResetWindowResizedFlag() { framebufferResized = false; }

	void CreateWindowSurface(VkInstance& instance, VkSurfaceKHR* surface);

	GLFWwindow* GetWindow() { return window; }

private:
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	std::string name;

	int width;
	int height;
	bool framebufferResized = false;

	GLFWwindow* window;
};