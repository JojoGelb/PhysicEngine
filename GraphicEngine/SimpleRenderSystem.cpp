#include "SimpleRenderSystem.h"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <imgui.h>
#include <imgui_impl_vulkan.h>

struct SimplePushConstantData {
	glm::mat4 modelMatrix{1.f};
	glm::mat4 normalMatrix{ 1.f };
	float alpha = 1.f;
};

SimpleRenderSystem::SimpleRenderSystem(GraphicDevice& device, VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout): graphicDevice(device)
{
	CreatePipelineLayout(globalSetLayout);
	CreatePipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(graphicDevice.Device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout)
{

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
 	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	
	//help push efficiently data to shader
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(graphicDevice.Device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass)
{
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig{};
	VKPipeline::DefaultPipelineConfigInfo(pipelineConfig);
	VKPipeline::enableAlphaBlending(pipelineConfig);
	/*pipelineConfig.attributeDescriptions.clear();
  	pipelineConfig.bindingDescriptions.clear();*/
	pipelineConfig.renderPass = renderPass;

	pipelineConfig.pipelineLayout = pipelineLayout;
	vKPipeline = std::make_unique<VKPipeline>(
		graphicDevice,
		"Shaders/simple_shader.vert.spv",
		"Shaders/simple_shader.frag.spv",
		pipelineConfig);
}

void SimpleRenderSystem::RenderGameObjects(FrameInfo& frameInfo)
{
	vKPipeline->Bind(frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(
      frameInfo.commandBuffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      pipelineLayout,
      0,
      1,
      &frameInfo.globalDescriptorSet,
      0,
      nullptr);

	for (auto& keyValue : frameInfo.gameObjects) {
		auto& obj = keyValue.second;
		if (obj->model == nullptr) continue;
		SimplePushConstantData push{};
		push.modelMatrix = obj->transformVisual.Mat4();
		push.normalMatrix = obj->transformVisual.NormalMatrix();
		push.alpha = obj->alpha;

		vkCmdPushConstants(
			frameInfo.commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);
		obj->model->Bind(frameInfo.commandBuffer);
    	obj->model->Draw(frameInfo.commandBuffer);

	}
}

void SimpleRenderSystem::RenderGameObjectsV2(FrameInfo &frameInfo)
{
	vKPipeline->Bind(frameInfo.commandBuffer);

	vkCmdBindDescriptorSets(
      frameInfo.commandBuffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      pipelineLayout,
      0,
      1,
      &frameInfo.globalDescriptorSet,
      0,
      nullptr);

	for (auto& keyValue : frameInfo.gameObjects) {
		auto& obj = keyValue.second;
		if (obj->model == nullptr) continue;
		SimplePushConstantData push{};
		push.modelMatrix = obj->transformVisual.Mat4();
		push.normalMatrix = obj->transformVisual.NormalMatrix();
		push.alpha = obj->alpha;

		vkCmdPushConstants(
			frameInfo.commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);
		obj->model->Bind(frameInfo.commandBuffer);
    	obj->model->Draw(frameInfo.commandBuffer);
	}
}

void SimpleRenderSystem::RenderImGui(VkCommandBuffer commandBuffer)
{
	// Rendering ImGui
	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
}

