#pragma once
#include <Window.h>
#include <GraphicDevice.h>
#include <memory>
#include <SwapChain.h>
#include <cassert>

class Renderer {
public:

	Renderer(Window & window, GraphicDevice& device);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	bool IsFrameInProgress() const { return isFrameStarted; }

	VkRenderPass GetSwapChainRenderPass() const { return lveSwapChain->GetRenderPass(); }
	float GetAspectRatio() const { return lveSwapChain->ExtentAspectRatio(); }

	VkCommandBuffer GetCurrentCommandBuffer() const {
		assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
		return commandBuffers[currentFrameIndex];
	}

	int GetFrameIndex() const {
		assert(isFrameStarted && "Cannot get frame index when frame not in progress");
		return currentFrameIndex;
	}

	VkCommandBuffer BeginFrame();
	void EndFrame();
	void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
	
	void CreateCommandBuffers();
	void FreeCommandBuffers();
	void RecreateSwapChain();

	Window& window;
	GraphicDevice& graphicDevice;
	std::unique_ptr<SwapChain> lveSwapChain;
	std::vector<VkCommandBuffer> commandBuffers;

	uint32_t currentImageIndex;
	int currentFrameIndex;
	bool isFrameStarted;
};