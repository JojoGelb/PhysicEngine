#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector> 

class ImGuiEngine
{
private:


	// Our state
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

	ImGui_ImplVulkanH_Window* wd;

	GLFWwindow* window;


public:
	ImGuiEngine();
	~ImGuiEngine();

	bool Init();

	void Run();

	void Shutdown();

};
