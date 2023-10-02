#pragma once

#include <vulkan/vulkan_core.h>
#include <GraphicDevice.h>
#include "Renderer.h"

#include "VisualGameObject.h" 

#include <memory>
#include <vector>

#include "Camera.h"
#include <SimpleRenderSystem.h>
#include <KeyboardInput.h>

class Window;

class VulkanHandler
{
public:
	VulkanHandler(Window & _window);

	~VulkanHandler();

	void Update(float frameTime = 0);
	void Render();

	void Shutdown();

	void AddGameObject2(VisualGameObject* obj) { objects2.push_back(obj); }
	void RemoveGameObject2(VisualGameObject* obj) {
		objects2.erase(std::remove(objects2.begin(), objects2.end(), obj), objects2.end());
	}

	GraphicDevice & GetGraphicDevice();

private:
	void InitImGui();//ImGui
	void ShutdownImGui(); //ImGui
	VkDescriptorPool imguiPool; //ImGui
	static void check_vk_result(VkResult err);//ImGui
	GraphicDevice graphicDevice;
	Renderer renderer;

	std::vector<VisualGameObject*> objects2;


	Window & window;

	SimpleRenderSystem renderSystem;
	Camera camera;
	KeyboardInput cameraController;
	VisualGameObject viewerObject;
};