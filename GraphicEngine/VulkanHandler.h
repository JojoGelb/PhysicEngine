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
#include "../Sources/ObjectData.h"

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

	GraphicDevice & GetGraphicDevice();

private:
	void InitialLoadGameObjects();
	void LoadGameObject(Particle particle);
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