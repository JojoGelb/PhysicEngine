
#include "ImGuiEngine.h"

#include "vulkan/vulkan_core.h"
#include <iostream>
#include"GLFW/glfw3.h"
#include "../MathPhysicEngine/Particle.h"
#include "../MathPhysicEngine/Vecteur3D.h"
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../MathPhysicEngine/Forces/ParticleGravity.h"
#include "../MathPhysicEngine/Forces/ConstantForce.h"
ImGuiEngine::ImGuiEngine(GLFWwindow* _window, std::vector<GameObject*>* _gameObjects): window(_window), gameObjects(_gameObjects)
{
    this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    this->show_demo_window = true;
    this->show_another_window = false;

    SetVisible(visible);
}

ImGuiEngine::~ImGuiEngine()
{
}

void ImGuiEngine::Update(float frameTime)
{
    bool visibilityTrigger;

    framerate = frameTime;

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
        GameObject* go = new GameObject(name);
        Particle* particleBuffer = new Particle(particle);
        go->AddComponent(particleBuffer);

        ParticleGravity* particleGravity = new ParticleGravity({ 0.0f,-10.0f,0.0f });
        MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particleBuffer, particleGravity);

        VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
        go->AddComponent(v);

        gameObjects->push_back(go);
    }

    /*
    if (ImGui::TreeNode("Projectile")) 
    {
        static GameObject * projectile = nullptr;

        if (projectile == nullptr) {
            if (ImGui::Button("Add projectile"))
            {
                GameObject* go = new GameObject(name);
                Particle* particle = new Particle();
                particle->AddProjectile();
                go->AddComponent(particle);
                VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);

                gameObjects->push_back(go);

                projectile = go;
            }
        }
        else {

            if (ImGui::Button("Add projectile"))
            {
                GameObject* go = new GameObject(name);
                Particle* particle = new Particle();
                particle->AddProjectile();
                go->AddComponent(particle);
                VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);

                gameObjects->push_back(go);

                projectile = go;
            }

            auto p = projectile->GetComponentOfType<Particle>();

            ImGui::Text(" position: %.2f, %.2f, %.2f", p->position.x, p->position.y, p->position.z);
            ImGui::Text(" velocity: %.2f, %.2f, %.2f", p->velocity.x, p->velocity.y, p->velocity.z);
            ImGui::Text(" acceleration: %.2f, %.2f, %.2f", p->acceleration.x, p->acceleration.y, p->acceleration.z);
            ImGui::Text(" force: %.2f, %.2f, %.2f", p->force.x, p->force.y, p->force.z);
            ImGui::Text(" gravity force: %.2f, %.2f, %.2f", p->gravityForce.x, p->gravityForce.y, p->gravityForce.z);

            if (ImGui::Button("launch projectile")) {
                p->force = Vector3D(500.0f, 500.0f, 0.0f);
                p->gravity = 1;
                p->impulse = true;
            }
        }

        ImGui::TreePop();
        
    }*/

    


    ImGui::End();

    ImGui::Begin("Objects List");

    ImGui::Text("famerate : %.5f", framerate);

    static std::vector<Vector3D> force(200);

    for (int i = 0; i < gameObjects->size(); i++)
    {
        auto particle = gameObjects->at(i)->GetComponentOfType<Particle>();
        if (ImGui::TreeNode( ( std::to_string(i) + ". " + gameObjects->at(i)->GetName()).c_str())) {

            if (ImGui::TreeNode("Add force")){

                ImGui::InputFloat("force x", &force[i].x);
                ImGui::InputFloat("force y", &force[i].y);
                ImGui::InputFloat("force z", &force[i].z);

                /*
                ImGui::Spacing();
                if (ImGui::Checkbox(" Impulse", &particle->impulse))*/

                ImGui::Spacing();

                if (ImGui::Button("Apply force")) {
                    //particle->force = force[i];
                    ConstantForce* constantForce = new ConstantForce(force[i]);

                    MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, constantForce);
                }

                ImGui::TreePop();
               
            }
            if (ImGui::TreeNode("Show particule data")) {

                ImGui::Spacing();
                if (ImGui::Checkbox(" print particle data on terminal ", &particle->printParticleOnTerminal))
                ImGui::Spacing();

                ImGui::Text(" position: %.2f, %.2f, %.2f", particle->position.x, particle->position.y, particle->position.z);
                ImGui::Text(" velocity: %.2f, %.2f, %.2f", particle->velocity.x, particle->velocity.y, particle->velocity.z);
                ImGui::Text(" acceleration: %.2f, %.2f, %.2f", particle->acceleration.x, particle->acceleration.y, particle->acceleration.z);
                //ImGui::Text(" force: %.2f, %.2f, %.2f", particle->force.x, particle->force.y, particle->force.z);

                ImGui::Text(" gravity force: %.2f, %.2f, %.2f", particle->gravityForce.x, particle->gravityForce.y, particle->gravityForce.z);

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::InputFloat(" inversed mass: ", &particle->inversedMass);
                ImGui::InputFloat(" gravity scale: ", &particle->gravity);
                ImGui::InputFloat(" damping: ", &particle->damping);

                ImGui::TreePop();
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