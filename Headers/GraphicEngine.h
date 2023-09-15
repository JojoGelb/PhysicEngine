#pragma once

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "imgui_impl_vulkan.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include "WindowContext.h"



class GraphicEngine
{

public:
	GraphicEngine();
	~GraphicEngine();

	bool Init(WindowContext* wc);
	void Run();
	void Shutdown();

	VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();

	static void check_vk_result(VkResult err);


	


private:
	
	WindowContext* windowContext;

	void CleanupVulkan();

	void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

	void SetupVulkan(std::vector<const char*> instance_extensions);

	//void glfw_error_callback(int error, const char* description);


};