#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <KeyboardInput.h>
#include "GameObject.h"
#include "../MathPhysicEngine/Vecteur3D.h"


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

	float framerate;

	KeyboardInput ImGuiInput;

	std::vector<GameObject*>* gameObjects;
public:
	ImGuiEngine(GLFWwindow* _window, std::vector<GameObject*> * _gameObjects);
	~ImGuiEngine();

	void Update(float frameTime);
	Vector3D testT;

};