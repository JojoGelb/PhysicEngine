#include "Renderer.h"

#include <stdexcept>
#include <array>

Renderer::Renderer(Window& _window, GraphicDevice& _device) :
	window(_window), graphicDevice(_device)
{
	RecreateSwapChain();
	CreateCommandBuffers();
}

Renderer::~Renderer()
{
	FreeCommandBuffers();
}

VkCommandBuffer Renderer::BeginFrame()
{
	assert(!isFrameStarted && "Can't call beginFrame while already in progress");

	auto result = lveSwapChain->AcquireNextImage(&currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapChain();
		return nullptr;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	isFrameStarted = true;

	auto commandBuffer = GetCurrentCommandBuffer();
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	return commandBuffer;
}

void Renderer::EndFrame()
{
	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
	auto commandBuffer = GetCurrentCommandBuffer();
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	auto result = lveSwapChain->SubmitCommandBuffers(&commandBuffer, &currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
		window.WasWindowResized()) {
		window.ResetWindowResizedFlag();
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	isFrameStarted = false;
	currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
	assert(
		commandBuffer == GetCurrentCommandBuffer() &&
		"Can't begin render pass on command buffer from a different frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = lveSwapChain->GetRenderPass();
	renderPassInfo.framebuffer = lveSwapChain->GetFrameBuffer(currentImageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = lveSwapChain->GetSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(lveSwapChain->GetSwapChainExtent().width);
	viewport.height = static_cast<float>(lveSwapChain->GetSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, lveSwapChain->GetSwapChainExtent() };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
	assert(
		commandBuffer == GetCurrentCommandBuffer() &&
		"Can't end render pass on command buffer from a different frame");
	vkCmdEndRenderPass(commandBuffer);
}

void Renderer::CreateCommandBuffers()
{
	commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = graphicDevice.GetCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(graphicDevice.Device(), &allocInfo, commandBuffers.data()) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}


}

void Renderer::FreeCommandBuffers()
{
	vkFreeCommandBuffers(
		graphicDevice.Device(),
		graphicDevice.GetCommandPool(),
		static_cast<uint32_t>(commandBuffers.size()),
		commandBuffers.data());
	commandBuffers.clear();
}

void Renderer::RecreateSwapChain()
{
	auto extent = window.GetExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = window.GetExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(graphicDevice.Device());

	if (lveSwapChain == nullptr) {
		lveSwapChain = std::make_unique<SwapChain>(graphicDevice, extent);
	}
	else {
		std::shared_ptr<SwapChain> oldSwapChain = std::move(lveSwapChain);

		lveSwapChain = std::make_unique<SwapChain>(graphicDevice, extent, oldSwapChain);
		
		if (!oldSwapChain->CompareSwapFormats(*lveSwapChain.get())) {
			throw std::runtime_error("Swap chain image(or depth) format has changed!");
		}
	}

	//-----------------
}