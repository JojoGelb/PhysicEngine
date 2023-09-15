#pragma once

#include <vulkan/vulkan_core.h>
#include "imgui_impl_vulkan.h"
#include <GLFW/glfw3.h>

class WindowContext
{
	public:
		
		// Data
		VkAllocationCallbacks* g_Allocator = nullptr;
		VkInstance               g_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice                 g_Device = VK_NULL_HANDLE;
		uint32_t                 g_QueueFamily = (uint32_t)-1;
		VkQueue                  g_Queue = VK_NULL_HANDLE;
		VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
		VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
		VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

		ImGui_ImplVulkanH_Window g_MainWindowData;
		int                      g_MinImageCount = 2;
		bool                     g_SwapChainRebuild = false;


		GLFWwindow* window;

		ImGui_ImplVulkanH_Window* wd;


		WindowContext* GetWindowContext();

		VkInstance GetG_Instance() {
			return g_Instance;
		}


};

