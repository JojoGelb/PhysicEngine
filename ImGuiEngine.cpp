
#include "ImGuiEngine.h"

#include "vulkan/vulkan_core.h"
#include <iostream>
#include"GLFW/glfw3.h"

ImGuiEngine::ImGuiEngine(GLFWwindow* _window): window(_window)
{
    this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    this->show_demo_window = true;
    this->show_another_window = false;

    SetVisible(visible);
}

ImGuiEngine::~ImGuiEngine()
{
}

void ImGuiEngine::Update()
{
    bool visibilityTrigger;

    ImGuiInput.ImGuiControls(window, visibilityTrigger);

    if(visibilityTrigger)
        SetVisible(!visible);



    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (visible) {

        //Show Demo ImGUI
        ShowDemoImGui();
    }

}



void ImGuiEngine::ShowDemoImGui()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (this->show_demo_window)
        ImGui::ShowDemoWindow(&this->show_demo_window);

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //std::cout << "ou";

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Test");                          // Create a window called "Hello, world!" and append into it.

        // Variable needed to make a particle
        static float px = 0.0f;
        static float py = 5.0f;
        static float pz = 0.0f;
        static float sx = 0.0f;
        static float sy = 0.0f;
        static float sz = 0.0f;
        static float invMass = 1.0f;
        static float gravityFactor = 1.0f;

        // Associated buttons
        ImGui::Text("Initial Position");
        ImGui::InputFloat("pos x", &px);
        ImGui::InputFloat("pos y", &py);
        ImGui::InputFloat("pos z", &pz);
        ImGui::Text("Initial Velocity");
        ImGui::InputFloat("Velocity x", &sx);
        ImGui::InputFloat("Velocity y", &sy);
        ImGui::InputFloat("Velocity z", &sz);
        ImGui::InputFloat("inv mass", &invMass);
        ImGui::InputFloat("gravity factor", &gravityFactor);
        ImGui::End();

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

void ImGuiEngine::SetVisible(bool _visible)
{
    if (visible == _visible) {
        return;
    }

    std::cout << visible;
    visible = _visible;

    if (visible) {
        ImGui_ImplGlfw_InstallCallbacks(window);
    }
    else {
        ImGui_ImplGlfw_RestoreCallbacks(window);

    }

}