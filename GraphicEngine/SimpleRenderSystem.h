#pragma once

#include <vulkan/vulkan_core.h>
//#include <GLFW/glfw3.h>
#include <GraphicDevice.h>

#include "GameObject.h" 
#include "Camera.h"

#include <memory>
#include <VKPipeline.h>

class Window;

class SimpleRenderSystem
{
public:
	SimpleRenderSystem(GraphicDevice & device, VkRenderPass renderPass);
	~SimpleRenderSystem();

	void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);
	void RenderImGui(VkCommandBuffer commandBuffer);
private:
	void CreatePipelineLayout();
	void CreatePipeline(VkRenderPass renderPass);
	

	GraphicDevice& graphicDevice;

	VkPipelineLayout pipelineLayout;
	std::unique_ptr<VKPipeline> vKPipeline;
};


