
#include "ImGuiEngine.h"

#include "vulkan/vulkan_core.h"
#include <iostream>
#include"GLFW/glfw3.h"

ImGuiEngine::ImGuiEngine(GLFWwindow* _window, ObjectData* _objectData): window(_window), objectData(_objectData)
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
        ShowEngineImGui();
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
        static int counter = 0;
        static float f = 0.0f;


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

void ImGuiEngine::ShowEngineImGui()
{

    ImGui::Begin("Add object");                          

    static float positionX = 0.0f;
    static float positionY = 0.0f;
    static float positionZ = 0.0f;
    static float velocityX = 0.0f;
    static float velocityY = 0.0f;
    static float velocityZ = 0.0f;
    static float inversedMass = 1.0f;
    static float damping = 0.999f;
    static float gravity = 1;
    static char name[128] = "cube";


    /*
    ImGui::Text("3D Model");
    ImGui::BeginListBox("3D Model list box");
    
    ImGui::EndListBox()*/
   
    ImGui::InputText("Object name", name, IM_ARRAYSIZE(name));

    ImGui::Text("Initial Position");
    ImGui::InputFloat("pos x", &positionX);
    ImGui::InputFloat("pos y", &positionY);
    ImGui::InputFloat("pos z", &positionZ);
    ImGui::Text("Initial Velocity");
    ImGui::InputFloat("Velocity x", &velocityX);
    ImGui::InputFloat("Velocity y", &velocityY);
    ImGui::InputFloat("Velocity z", &velocityZ);
    ImGui::Text("Float parameters");
    ImGui::InputFloat("inversed mass", &inversedMass);
    ImGui::InputFloat("damping", &damping);
    ImGui::InputFloat("gravity", &gravity);
    if (ImGui::Button("Add Game Object")) {
        Particle particle = Particle
        (
            Vector3D(positionX, positionY, positionZ),
            Vector3D(velocityX, velocityY, velocityZ),
            Vector3D(0.0f, 0.0f, 0.0f),
            inversedMass,
            damping,
            gravity
        );
            objectData->particles.push_back(particle);
            objectData->names.push_back(name);
    }
    ImGui::End();

    ImGui::Begin("Objects List");

    static std::vector<Vector3D> force(200);

    for (int i = 0; i < objectData->names.size(); i++)
    {

        //std::string s = (std::string)i;
        if (ImGui::TreeNode( ( std::to_string(i) + ". " + objectData->names[i]).c_str())) {
            if (ImGui::TreeNode("Add force")){

                ImGui::InputFloat("force x", &force[i].x);
                ImGui::InputFloat("force y", &force[i].y);
                ImGui::InputFloat("force z", &force[i].z);

                ImGui::Spacing();
                if (ImGui::Checkbox(" Impulse", &objectData->particles[i].impulse))
                ImGui::Spacing();

                if (ImGui::Button("Apply force")) {
                    objectData->particles[i].force = force[i];
                }

                ImGui::TreePop();
               
            }
            if (ImGui::TreeNode("Show particule data")) {

                ImGui::Spacing();
                if (ImGui::Checkbox(" print particle data on terminal ", &objectData->particles[i].printParticleOnTerminal))
                ImGui::Spacing();

                ImGui::Text(" position: %.2f, %.2f, %.2f", objectData->particles[i].position.x, objectData->particles[i].position.y, objectData->particles[i].position.z);
                ImGui::Text(" velocity: %.2f, %.2f, %.2f", objectData->particles[i].velocity.x, objectData->particles[i].velocity.y, objectData->particles[i].velocity.z);
                ImGui::Text(" acceleration: %.2f, %.2f, %.2f", objectData->particles[i].acceleration.x, objectData->particles[i].acceleration.y, objectData->particles[i].acceleration.z);
                ImGui::Text(" force: %.2f, %.2f, %.2f", objectData->particles[i].force.x, objectData->particles[i].force.y, objectData->particles[i].force.z);

                ImGui::Text(" gravity force: %.2f, %.2f, %.2f", objectData->particles[i].gravityForce.x, objectData->particles[i].gravityForce.y, objectData->particles[i].gravityForce.z);

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::InputFloat(" inversed mass: ", &objectData->particles[i].inversedMass);
                ImGui::InputFloat(" gravity scale: ", &objectData->particles[i].gravity);
                ImGui::InputFloat(" damping: ", &objectData->particles[i].damping);



                ImGui::TreePop();

                /*
                if (ImGui::Button(sh")) {
                    objectData->particles[i].force = { forceX , forceY , forceZ };
                }*/
            }

            ImGui::TreePop();
            ImGui::Spacing();
         
        }
           
    }
    ImGui::End();
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