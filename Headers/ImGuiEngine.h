#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <WindowContext.h>
#include <vector> 

class ImGuiEngine
{
private:

	// Our state
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

	WindowContext* windowContext;

	void ShowDemoImGui();

	void CleanupVulkanWindow();

	void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);

	void FramePresent(ImGui_ImplVulkanH_Window* wd);
public:
	ImGuiEngine();
	~ImGuiEngine();

	
	bool Init(WindowContext* wc);

	void Run();

	void Shutdown();

};
