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
	glm::mat4 transform{ 1.f };
	glm::mat4 normalMatrix{ 1.f };
};

SimpleRenderSystem::SimpleRenderSystem(GraphicDevice& device, VkRenderPass renderPass): graphicDevice(device)
{
	CreatePipelineLayout();
	CreatePipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem()
{
	vkDestroyPipelineLayout(graphicDevice.Device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::CreatePipelineLayout()
{

	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	//Used to pass data other than vertex data to fragment and vertex shader like texture
	// uniform buffer objects...
	pipelineLayoutInfo.pSetLayouts = nullptr;
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
	pipelineConfig.renderPass = renderPass;

	pipelineConfig.pipelineLayout = pipelineLayout;
	vKPipeline = std::make_unique<VKPipeline>(
		graphicDevice,
		"Shaders/simple_shader.vert.spv",
		"Shaders/simple_shader.frag.spv",
		pipelineConfig);
}

void SimpleRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera)
{
	vKPipeline->Bind(commandBuffer);

	auto projectionView = camera.GetProjection() * camera.GetView();

	for (auto& obj : gameObjects) {
		SimplePushConstantData push{};
		auto modelMatrix = obj.transform.Mat4();
		push.transform = projectionView * modelMatrix;
		push.normalMatrix = obj.transform.NormalMatrix();
		push.transform = projectionView * obj.transform.Mat4();

		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);
		obj.model->Bind(commandBuffer);
		obj.model->Draw(commandBuffer);

	}
}

void SimpleRenderSystem::RenderImGui(VkCommandBuffer commandBuffer)
{
	// Rendering ImGui
	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
}

