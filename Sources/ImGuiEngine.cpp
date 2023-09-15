
#include "GraphicEngine.h"



#include "ImGuiEngine.h"



ImGuiEngine::ImGuiEngine()
{ 

}

ImGuiEngine::~ImGuiEngine()
{
}



void ImGuiEngine::CleanupVulkanWindow()
{
    ImGui_ImplVulkanH_DestroyWindow(this->windowContext->g_Instance, this->windowContext->g_Device, &this->windowContext->g_MainWindowData, this->windowContext->g_Allocator);
}

void ImGuiEngine::FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
{
    VkResult err;

    VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    err = vkAcquireNextImageKHR(this->windowContext->g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        this->windowContext->g_SwapChainRebuild = true;
        return;
    }
    GraphicEngine::check_vk_result(err);

    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
    {
        err = vkWaitForFences(this->windowContext->g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        GraphicEngine::check_vk_result(err);

        err = vkResetFences(this->windowContext->g_Device, 1, &fd->Fence);
        GraphicEngine::check_vk_result(err);
    }
    {
        err = vkResetCommandPool(this->windowContext->g_Device, fd->CommandPool, 0);
        GraphicEngine::check_vk_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        GraphicEngine::check_vk_result(err);
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
        GraphicEngine::check_vk_result(err);
        err = vkQueueSubmit(this->windowContext->g_Queue, 1, &info, fd->Fence);
        GraphicEngine::check_vk_result(err);
    }
}

void ImGuiEngine::FramePresent(ImGui_ImplVulkanH_Window* wd)
{
    if (this->windowContext->g_SwapChainRebuild)
        return;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &wd->Swapchain;
    info.pImageIndices = &wd->FrameIndex;
    VkResult err = vkQueuePresentKHR(this->windowContext->g_Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        this->windowContext->g_SwapChainRebuild = true;
        return;
    }
    GraphicEngine::check_vk_result(err);
    wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
}



bool ImGuiEngine::Init(WindowContext* wc)
{
  
    this->windowContext = wc;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

     // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(this->windowContext->window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = this->windowContext->g_Instance;
    init_info.PhysicalDevice = this->windowContext->g_PhysicalDevice;
    init_info.Device = this->windowContext->g_Device;
    init_info.QueueFamily = this->windowContext->g_QueueFamily;
    init_info.Queue = this->windowContext->g_Queue;
    init_info.PipelineCache = this->windowContext->g_PipelineCache;
    init_info.DescriptorPool = this->windowContext->g_DescriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = this->windowContext->g_MinImageCount;
    init_info.ImageCount = this->windowContext->wd->ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = this->windowContext->g_Allocator;
    init_info.CheckVkResultFn = GraphicEngine::check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, this->windowContext->wd->RenderPass);


    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Upload Fonts
    {
        // Use any command queue
        VkCommandPool command_pool = this->windowContext->wd->Frames[this->windowContext->wd->FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = this->windowContext->wd->Frames[this->windowContext->wd->FrameIndex].CommandBuffer;

        VkResult err = vkResetCommandPool(this->windowContext->g_Device, command_pool, 0);
        GraphicEngine::check_vk_result(err);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);
        GraphicEngine::check_vk_result(err);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        err = vkEndCommandBuffer(command_buffer);
        GraphicEngine::check_vk_result(err);
        err = vkQueueSubmit(this->windowContext->g_Queue, 1, &end_info, VK_NULL_HANDLE);
        GraphicEngine::check_vk_result(err);

        err = vkDeviceWaitIdle(this->windowContext->g_Device);
        GraphicEngine::check_vk_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    // Our state
    this->show_demo_window = true;
    this->show_another_window = false;
    this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	return true;
}

void ImGuiEngine::Run()
{
    while (!glfwWindowShouldClose(this->windowContext->window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Resize swap chain?
        if (this->windowContext->g_SwapChainRebuild)
        {
            int width, height;

            glfwGetFramebufferSize(this->windowContext->window, &width, &height);


            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(this->windowContext->g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(this->windowContext->g_Instance, this->windowContext->g_PhysicalDevice, this->windowContext->g_Device, &this->windowContext->g_MainWindowData, this->windowContext->g_QueueFamily, this->windowContext->g_Allocator, width, height, this->windowContext->g_MinImageCount);
                this->windowContext->g_MainWindowData.FrameIndex = 0;
                this->windowContext->g_SwapChainRebuild = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Show Demo ImGUI
        ShowDemoImGui();

        // Rendering
        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
        if (!is_minimized)
        {
            this->windowContext->wd->ClearValue.color.float32[0] = this->clear_color.x * this->clear_color.w;
            this->windowContext->wd->ClearValue.color.float32[1] = this->clear_color.y * this->clear_color.w;
            this->windowContext->wd->ClearValue.color.float32[2] = this->clear_color.z * this->clear_color.w;
            this->windowContext->wd->ClearValue.color.float32[3] = this->clear_color.w;
            FrameRender(this->windowContext->wd, draw_data);
            FramePresent(this->windowContext->wd);
        }

    }
}


void ImGuiEngine::Shutdown()
{
    
    // Cleanup
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    CleanupVulkanWindow();
}

void ImGuiEngine::ShowDemoImGui()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (this->show_demo_window)
        ImGui::ShowDemoWindow(&this->show_demo_window);


    ImGuiIO& io = ImGui::GetIO(); (void)io;


    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &this->show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &this->show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&this->clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (this->show_another_window)
    {
        ImGui::Begin("Another Window", &this->show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            this->show_another_window = false;
        ImGui::End();
    }
}
