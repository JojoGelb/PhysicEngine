#include "IMGUIHandler.h"
#include "imgui.h"
#include <vulkan/vulkan_core.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <vector>
#include <VulkanHandler.h>
#include <io.h>

IMGUIHandler::IMGUIHandler(GLFWwindow * _window, VulkanHandler & vkHandler):window(_window),vulkanHandler(vkHandler)
{
		//1: create descriptor pool for IMGUI
		// the size of the pool is very oversize, but it's copied from imgui demo itself.
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		VkDescriptorPool imguiPool;
		GraphicDevice& dev = vkHandler.GetGraphicDevice();
		vkCreateDescriptorPool(dev.device(), &pool_info, nullptr, &imguiPool);


		// 2: initialize imgui library

		//this initializes the core structures of imgui
		ImGui::CreateContext();

		//this initializes imgui for SDL
		ImGui_ImplGlfw_InitForVulkan(window, true);

		//this initializes imgui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = dev.getInstance();
		init_info.PhysicalDevice = dev.getPhysicalDevice();
		init_info.Device = dev.device();
		init_info.Queue = dev.graphicsQueue();
		init_info.DescriptorPool = imguiPool;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&init_info, vkHandler.GetSwapChain().getRenderPass());

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(vulkanHandler.GetCommandBuffers().at(1), &begin_info);
		//execute a gpu command to upload imgui font textures
		ImGui_ImplVulkan_CreateFontsTexture(vulkanHandler.GetCommandBuffers().at(1));
		
		//clear font textures from cpu data
		ImGui_ImplVulkan_DestroyFontUploadObjects();

		/*
		//add the destroy the imgui created structures
		_mainDeletionQueue.push_function([=]() {

			vkDestroyDescriptorPool(_device, imguiPool, nullptr);
			ImGui_ImplVulkan_Shutdown();
			});*/

		wd.Width = 800;
		wd.Height = 600;
		wd.Swapchain = vulkanHandler.GetSwapChain().GetSwapChainKHR();
		wd.Surface = vulkanHandler.GetGraphicDevice().surface();
		wd.SurfaceFormat = vulkanHandler.GetGraphicDevice().getSwapChainSupport().formats[0];
		wd.PresentMode = vulkanHandler.GetGraphicDevice().getSwapChainSupport().presentModes[0];
		wd.RenderPass = vulkanHandler.GetSwapChain().getRenderPass();
		wd.Pipeline = vulkanHandler.GetPipeline()->GetVkPipeline();
		wd.UseDynamicRendering = false;
		wd.ClearEnable = true;
		wd.FrameIndex = 0;
		wd.ImageCount = 2;
		wd.SemaphoreIndex = 0;
}

void IMGUIHandler::FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
{
	VkResult err;

	VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	err = vkAcquireNextImageKHR(vulkanHandler.GetGraphicDevice().device(), wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		//g_SwapChainRebuild = true;
		return;
	}
	//check_vk_result(err);

	ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
	{
		err = vkWaitForFences(vulkanHandler.GetGraphicDevice().device(), 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
		//check_vk_result(err);

		err = vkResetFences(vulkanHandler.GetGraphicDevice().device(), 1, &fd->Fence);
		//check_vk_result(err);
	}
	{
		err = vkResetCommandPool(vulkanHandler.GetGraphicDevice().device(), fd->CommandPool, 0);
		//check_vk_result(err);
		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
		//check_vk_result(err);
	}
	{
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = wd->RenderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = wd->Width;
		info.renderArea.extent.height = wd->Height;
		info.clearValueCount = 1;
		info.pClearValues = &wd->ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &image_acquired_semaphore;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &render_complete_semaphore;

		err = vkEndCommandBuffer(fd->CommandBuffer);
		//check_vk_result(err);
		err = vkQueueSubmit(vulkanHandler.GetGraphicDevice().graphicsQueue(), 1, &info, fd->Fence);
		//check_vk_result(err);
	}
}

void IMGUIHandler::FramePresent(ImGui_ImplVulkanH_Window* wd)
{
	//if (g_SwapChainRebuild)
		//return;
	VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &render_complete_semaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &wd->Swapchain;
	info.pImageIndices = &wd->FrameIndex;
	VkResult err = vkQueuePresentKHR(vulkanHandler.GetGraphicDevice().graphicsQueue(), &info);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		//g_SwapChainRebuild = true;
		return;
	}
	//check_vk_result(err);
	wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
}

void IMGUIHandler::Render()
{

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	ImGui::Checkbox("Another Window", &show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
	if (!is_minimized)
	{

		wd.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
		wd.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
		wd.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
		wd.ClearValue.color.float32[3] = clear_color.w;
		FrameRender(&wd, draw_data);
		FramePresent(&wd);
	}
}
