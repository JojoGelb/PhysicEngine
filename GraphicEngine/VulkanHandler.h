#pragma once

#include <vulkan/vulkan_core.h>
#include <GraphicDevice.h>
#include "Renderer.h"
#include "lve_descriptors.h"

#include "VisualGameObject.h" 

#include <memory>
#include <vector>

#include "Camera.h"
#include <SimpleRenderSystem.h>
#include "PointLightSystem.h"
#include <KeyboardInput.h>

class Window;

class VulkanHandler
{
public:
	VulkanHandler(Window & _window);

	~VulkanHandler();

	void Update(float frameTime = 0);
	void Render(float frameTime);

	void Shutdown();

	void AddGameObject2(VisualGameObject* obj) {
		objects2.emplace(obj->GetId(),obj);
		}
	void RemoveGameObject2(VisualGameObject* obj) {
		objects2.erase(obj->GetId());
	}

	GraphicDevice & GetGraphicDevice();

private:
	void InitImGui();//ImGui
	void ShutdownImGui(); //ImGui
	VkDescriptorPool imguiPool; //ImGui
	static void check_vk_result(VkResult err);//ImGui
	GraphicDevice graphicDevice;
	Renderer renderer;

	// note: order of declarations matters
  	std::unique_ptr<LveDescriptorPool> globalPool{};

	//std::vector<VisualGameObject*> objects2;
	VisualGameObject::Map objects2;


	Window & window;

	SimpleRenderSystem * renderSystem;
	PointLightSystem * pointLightRenderSystem;
	Camera camera;
	KeyboardInput cameraController;
	VisualGameObject * viewerObject;

	std::vector<std::unique_ptr<LveBuffer>> uboBuffers;
	std::vector<VkDescriptorSet> globalDescriptorSets;
	//LveBuffer globalUboBuffer;
};