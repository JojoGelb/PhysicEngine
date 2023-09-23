#pragma once

#include "imgui_impl_vulkan.h"

struct GLFWwindow;
class VulkanHandler;
struct ImDrawData;

class IMGUIHandler {

public:
	IMGUIHandler(GLFWwindow* window, VulkanHandler& vkHandler);
	void Render();

private:

	void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
	void FramePresent(ImGui_ImplVulkanH_Window* wd);

	GLFWwindow* window;
	VulkanHandler & vulkanHandler;

	ImGui_ImplVulkanH_Window wd;
};