#pragma once

#include <vulkan/vulkan_core.h>
//#include <GLFW/glfw3.h>
#include <GraphicDevice.h>
#include "lve_frame_info.h"
#include "VisualGameObject.h" 
#include "Camera.h"

#include <memory>
#include <VKPipeline.h>

class Window;

class SimpleRenderSystem
{
public:
	SimpleRenderSystem(GraphicDevice & device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
	~SimpleRenderSystem();

	void RenderGameObjects(FrameInfo &frameInfo, std::vector<VisualGameObject> &gameObjects);
	void RenderGameObjectsV2(FrameInfo &frameInfo, std::vector<VisualGameObject*> &gameObjects);
	void RenderImGui(VkCommandBuffer commandBuffer);
private:
	void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void CreatePipeline(VkRenderPass renderPass);
	

	GraphicDevice& graphicDevice;

	VkPipelineLayout pipelineLayout;
	std::unique_ptr<VKPipeline> vKPipeline;
};


