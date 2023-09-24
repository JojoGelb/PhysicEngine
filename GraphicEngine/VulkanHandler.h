#pragma once

#include <vulkan/vulkan_core.h>
#include <GraphicDevice.h>
#include "Renderer.h"

#include "GameObject.h" 

#include <memory>
#include <vector>

#include "Camera.h"
#include <SimpleRenderSystem.h>
#include <KeyboardInput.h>
#include "../ObjectData.h"

class Window;

class VulkanHandler
{
public:
	VulkanHandler(Window & _window, ObjectData* _objectData);

	~VulkanHandler();

	void Update(float frameTime = 0);
	void Render();

	void Shutdown();

	GraphicDevice & GetGraphicDevice();

	ObjectData* objectData;
private:
	void InitialLoadGameObjects();
	void LoadGameObject(Particle particle);
	void InitImGui();//ImGui
	void ShutdownImGui(); //ImGui
	VkDescriptorPool imguiPool; //ImGui
	static void check_vk_result(VkResult err);//ImGui
	GraphicDevice graphicDevice;
	Renderer renderer;
	std::vector<GameObject> gameObjects;
	Window & window;

	SimpleRenderSystem renderSystem;
	Camera camera;
	KeyboardInput cameraController;
	GameObject viewerObject; 
};

