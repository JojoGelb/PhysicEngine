#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <vector> 
#include <KeyboardInput.h>
#include "ObjectData.h"



class ImGuiEngine
{
private:

	// Our state
	bool show_demo_window = true;
	bool show_another_window = true;
	bool visible = true;

	ImVec4 clear_color;

	GLFWwindow* window;

	void ShowDemoImGui();
	void ShowEngineImGui();

	void SetVisible(bool _visible);


	KeyboardInput ImGuiInput;
	ObjectData* objectData;
public:
	ImGuiEngine(GLFWwindow* _window, ObjectData* _objectData);
	~ImGuiEngine();

	void Update();

};