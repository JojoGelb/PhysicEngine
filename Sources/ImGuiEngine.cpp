
#include "ImGuiEngine.h"

#include "vulkan/vulkan_core.h"
#include <iostream>
#include"GLFW/glfw3.h"
#include "../MathPhysicEngine/Particle.h"
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleGravity.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ConstantForce.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleSpring.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleAnchoredSpring.h"
#include <random>
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleElasticBungee.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/InputForce.h"


ImGuiEngine::ImGuiEngine(GLFWwindow* _window, std::vector<GameObject*>* _gameObjects): window(_window), gameObjects(_gameObjects)
{
    this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    this->show_demo_window = true;
    this->show_another_window = false;

    SetVisible(!visible);
}

ImGuiEngine::~ImGuiEngine()
{
}

void ImGuiEngine::Update(float frameTime)
{
    bool visibilityTrigger;

    framerate = frameTime;

    ImGuiInput.ImGuiControls(window, visibilityTrigger);
    if(inputForce)
        ImGuiInput.BlobControls(window, *inputForce);

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

    static float rotationX = 0.0f;
    static float rotationY = 0.0f;
    static float rotationZ = 0.0f;

    static float angularVelocityX = 0.0f;
    static float angularVelocityY = 0.0f;
    static float angularVelocityZ = 0.0f;

    static float orientationW = 1.0f;
    static float orientationI = 0.0f;
    static float orientationJ = 0.0f;
    static float orientationK = 0.0f;
    /*
    ImGui::Text("3D Model");
    ImGui::BeginListBox("3D Model list box");

    ImGui::EndListBox()*/

    ImGui::InputText("Object name", name, IM_ARRAYSIZE(name));

    ImGui::Text("Initial Position  (m)");
    ImGui::InputFloat("pos x", &positionX);
    ImGui::InputFloat("pos y", &positionY);
    ImGui::InputFloat("pos z", &positionZ);
    ImGui::Text("Initial Velocity  (m/s)");
    ImGui::InputFloat("Velocity x", &velocityX);
    ImGui::InputFloat("Velocity y", &velocityY);
    ImGui::InputFloat("Velocity z", &velocityZ);
    ImGui::Text("Float parameters");
    ImGui::InputFloat("inversed mass  (Kg^-1)", &inversedMass);
    ImGui::InputFloat("damping  (Kg/s)", &damping);
    ImGui::InputFloat("gravity scale", &gravity);

    ImGui::Spacing();
    ImGui::Text("Angular parameters");
    ImGui::Text("Initial rotation");
    ImGui::InputFloat("rot x", &rotationX);
    ImGui::InputFloat("rot y", &rotationY);
    ImGui::InputFloat("rot z", &rotationZ);

    ImGui::Text("Initial orientation");
    ImGui::InputFloat("ori w", &orientationW);
    ImGui::InputFloat("ori i", &orientationI);
    ImGui::InputFloat("ori j", &orientationJ);
    ImGui::InputFloat("ori k", &orientationK);


    if (ImGui::Button("Add Particle Game Object")) {
        Particle* particle = new Particle
        (
            Vector3D(positionX, positionY, positionZ),
            Vector3D(velocityX, velocityY, velocityZ),
            Vector3D(0.0f, 0.0f, 0.0f),
            inversedMass,
            damping,
            gravity
        );
        GameObject* go = new GameObject(name);

        go->AddComponent(particle);

        ParticleGravity* particleGravity = new ParticleGravity({ 0.0f,-10.0f,0.0f });
        MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleGravity);


        VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
        go->AddComponent(v);

        gameObjects->push_back(go);
    }
    if (ImGui::Button("Add RigidBody Game Object")) {
        RigidBody* rigidBody = new RigidBody
        (
            Vector3D(positionX, positionY, positionZ),//position
            Vector3D(velocityX, velocityY, velocityZ),//velocity
            Vector3D(0.0f, 0.0f, 0.0f),//Acceleration
            Vector3D(rotationX, rotationY, rotationZ),//Rotation
            { orientationW,orientationI,orientationJ,orientationK } //Orientation
           // inversedMass,//
            //damping,
            //gravity
        );
        GameObject* go = new GameObject(name);

        go->AddComponent(rigidBody);

        //ParticleGravity* particleGravity = new ParticleGravity({ 0.0f,-10.0f,0.0f });
        //MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleGravity);


        VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
        go->AddComponent(v);

        gameObjects->push_back(go);
    }

    ImGui::End();

    ImGui::Begin("Objects List");

    ImGui::Text("famerate : %.5f", framerate);

    static std::vector<Vector3D> force(200);
    static std::vector<float> restLength(200);
    static std::vector<float[4]> orientationVector(200);

    static std::vector<float> springConstant(200);
    static std::vector<int> index(200);



    for (int i = 0; i < gameObjects->size(); i++)
    {
        auto particle = gameObjects->at(i)->GetComponentOfType<Particle>();
        auto rigidBody = gameObjects->at(i)->GetComponentOfType<RigidBody>();

        if (ImGui::TreeNode( ( std::to_string(i) + ". " + gameObjects->at(i)->GetName()).c_str())) {

            if (ImGui::TreeNode("Add Unique constant force")){

                ImGui::InputFloat("force x (N)", &force[i].x);
                ImGui::InputFloat("force y (N)", &force[i].y);
                ImGui::InputFloat("force z (N)", &force[i].z);

                ImGui::Spacing();

                if (ImGui::Button("Apply Input force")) {
                    InputForce* newInputForce = new InputForce();
                    particle->inputValues = force[i];

                    MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, newInputForce);
                }

                if (ImGui::Button("Apply constant force")) {
                    ConstantForce* newConstantForce = new ConstantForce(force[i]);
                    MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, newConstantForce);
                }

                

                ImGui::TreePop();
            }

            /*
            if (ImGui::TreeNode("Add spring force")) {

                //ImGui::PushID(i+10);

              
                ImGui::InputInt("Index", &index[i]);
                ImGui::InputFloat("rest Length", &restLength[i]);
                ImGui::InputFloat("spring Constant", &springConstant[i]);

                //ImGui::PopID();

                ImGui::Spacing();

                if (ImGui::Button("Apply spring force")) {

                    
                    //Particle* other = new Particle();
                    ParticleSpring* particleSpring = new ParticleSpring(gameObjects->at(index[i])->GetComponentOfType<Particle>(), restLength[i], springConstant[i]);
                    ParticleSpring* otherSpring = new ParticleSpring(particle, restLength[i], springConstant[i]);

                    MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleSpring);
                    MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(gameObjects->at(index[i])->GetComponentOfType<Particle>(), otherSpring);

                }


                ImGui::TreePop();



            }
            if (ImGui::TreeNode("Add anchored spring force")) {

                ImGui::InputInt("Index", &index[i]);
                ImGui::InputFloat("rest Length", &restLength[i]);
                ImGui::InputFloat("spring Constant", &springConstant[i]);


                ImGui::Spacing();

                if (ImGui::Button("Apply anchored spring force")) {


                    //Particle* other = new Particle();
                    Vector3D anchoredPos = gameObjects->at(index[i])->GetComponentOfType<Particle>()->position;
                    ParticleAnchoredSpring* particleAnchoredSpringApply = new ParticleAnchoredSpring(anchoredPos, restLength[i], springConstant[i]);

                    MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleAnchoredSpringApply);
                    
                
                }

                ImGui::TreePop();

            }*/
            if (particle) {
                if (ImGui::TreeNode("Show PARTICULE data")) {

                ImGui::Spacing();
                //if (ImGui::Checkbox(" print particle data on terminal ", &particle->printParticleOnTerminal))
                ImGui::Spacing();

                ImGui::Text(" position: %.2f, %.2f, %.2f (m)", particle->position.x, particle->position.y, particle->position.z);
                ImGui::Text(" velocity: %.2f, %.2f, %.2f (m/s)", particle->velocity.x, particle->velocity.y, particle->velocity.z);
                ImGui::Text(" acceleration: %.2f, %.2f, %.2f (m/s^2)", particle->acceleration.x, particle->acceleration.y, particle->acceleration.z);
                
                //ImGui::Text(" force: %.2f, %.2f, %.2f", particle->force.x, particle->force.y, particle->force.z);
                //ImGui::Text(" gravity force: %.2f, %.2f, %.2f", particle->gravityForce.x, particle->gravityForce.y, particle->gravityForce.z);

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::InputFloat(" inversed mass: (Kg^-1)", &particle->inversedMass);
                ImGui::InputFloat(" gravity scale: ", &particle->gravity);
                ImGui::InputFloat(" damping: (Kg/s)", &particle->damping);

                ImGui::TreePop();
                }
            }
            if (rigidBody) {
                if (ImGui::TreeNode("Show RIGIDBODY data")) {

                    ImGui::Spacing();
                    //if (ImGui::Checkbox(" print particle data on terminal ", &particle->printParticleOnTerminal))
                    ImGui::Spacing();

                    ImGui::Spacing();
                    ImGui::Text("LINEAR DATA");
                    ImGui::Spacing();

                    ImGui::Text(" position: %.2f, %.2f, %.2f (m)", rigidBody->GetPosition().x, rigidBody->GetPosition().y, rigidBody->GetPosition().z);
                    ImGui::Text(" velocity: %.2f, %.2f, %.2f (m/s)", rigidBody->GetVelocity().x, rigidBody->GetVelocity().y, rigidBody->GetVelocity().z);
                    ImGui::Text(" linear acceleration: %.2f, %.2f, %.2f (m/s^2) ", rigidBody->GetLinearAcceleration().x, rigidBody->GetLinearAcceleration().y, rigidBody->GetLinearAcceleration().z);
                    ImGui::Text(" linear damping: %.2f ", rigidBody->GetLinearDamping());
                    ImGui::Text(" mass: %.2f (Kg)", rigidBody->GetMass());

                    ImGui::Spacing();
                    ImGui::Text("ANGULAR DATA");
                    ImGui::Spacing();

                    ImGui::Text(" angular damping: %.2f ", rigidBody->GetAngularDamping());
                    ImGui::Text("rotation: %.2f, %.2f, %.2f (m)", rigidBody->GetRotation().x, rigidBody->GetRotation().y, rigidBody->GetRotation().z);
                    ImGui::Text("orientation: w: %.2f,i: %.2f,j: %.2f,k: %.2f", rigidBody->GetOrientation().w(), rigidBody->GetOrientation().i(),
                        rigidBody->GetOrientation().j(), rigidBody->GetOrientation().k());
                    ImGui::Text("angular acceleration: %.2f, %.2f, %.2f (m/s^2) ", rigidBody->GetAngularAcceleration().x, rigidBody->GetAngularAcceleration().y, rigidBody->GetAngularAcceleration().z);

                    ImGui::Text("inverse Inertia Tensor: ");
                    ImGui::Text("%.2f, %.2f, %.2f", rigidBody->GetInverseInertiaTensor().A(), rigidBody->GetInverseInertiaTensor().B(), rigidBody->GetInverseInertiaTensor().C());
                    ImGui::Text("%.2f, %.2f, %.2f", rigidBody->GetInverseInertiaTensor().D(), rigidBody->GetInverseInertiaTensor().E(), rigidBody->GetInverseInertiaTensor().F());
                    ImGui::Text("%.2f, %.2f, %.2f", rigidBody->GetInverseInertiaTensor().G(), rigidBody->GetInverseInertiaTensor().H(), rigidBody->GetInverseInertiaTensor().I());

                    ImGui::Spacing();
                    ImGui::Text("inverse Inertia Tensor WORLD: ");
                    ImGui::Text("%.2f, %.2f, %.2f", rigidBody->GetInverseInertiaTensorWorld().A(), rigidBody->GetInverseInertiaTensorWorld().B(), rigidBody->GetInverseInertiaTensorWorld().C());
                    ImGui::Text("%.2f, %.2f, %.2f", rigidBody->GetInverseInertiaTensorWorld().D(), rigidBody->GetInverseInertiaTensorWorld().E(), rigidBody->GetInverseInertiaTensorWorld().F());
                    ImGui::Text("%.2f, %.2f, %.2f", rigidBody->GetInverseInertiaTensorWorld().G(), rigidBody->GetInverseInertiaTensorWorld().H(), rigidBody->GetInverseInertiaTensorWorld().I());



                    // ImGui::InputFloat4(" inversed mass: (Kg^-1)", &rig->inversedMass);
                    //ImGui::Text(" force: %.2f, %.2f, %.2f", particle->force.x, particle->force.y, particle->force.z);
                    //ImGui::Text(" gravity force: %.2f, %.2f, %.2f", particle->gravityForce.x, particle->gravityForce.y, particle->gravityForce.z);

                    ImGui::Spacing();
                    ImGui::Spacing();

                    // ImGui::InputFloat(" inversed mass: (Kg^-1)", &particle->inversedMass);
                    // ImGui::InputFloat(" gravity scale: ", &particle->gravity);
                    // ImGui::InputFloat(" damping: (Kg/s)", &particle->damping);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("ORIENTATION")) {

                    ImGui::InputFloat("ori w", &orientationVector[0][i]);
                    ImGui::InputFloat("ori i", &orientationVector[1][i]);
                    ImGui::InputFloat("ori j", &orientationVector[2][i]);
                    ImGui::InputFloat("ori k", &orientationVector[3][i]);



                    ImGui::Spacing();

                    if (ImGui::Button("Apply")) {

                        gameObjects->at(index[i])->GetComponentOfType<RigidBody>()->SetOrientation(orientationVector[0][i], orientationVector[1][i], orientationVector[2][i], orientationVector[3][i]);
                        //Particle* other = new Particle();
                       // Vector3D anchoredPos = gameObjects->at(index[i])->GetComponentOfType<Particle>()->position;
                       // ParticleAnchoredSpring* particleAnchoredSpringApply = new ParticleAnchoredSpring(anchoredPos, restLength[i], springConstant[i]);

                       // MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleAnchoredSpringApply);


                    }

                    ImGui::TreePop();

                }
            }

          

           

            ImGui::TreePop();
            ImGui::Spacing();
         
        }
           
    }
    ImGui::End();

    TestIteration2();
    //TestIteration3();
}

void ImGuiEngine::TestIteration3() {
    ImGui::Begin("Phase 3 Test panel");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    /*
        - Test rotation brut
        - Test v�locit� angulaire : rotate sur un axe par une quantit� = impulsion: pas de force
        - Test de force: sur un point du carr�
        - Ressort ?
    */

    if (ImGui::TreeNode("Forces")) {
        
    }

    ImGui::End();
}

void ImGuiEngine::TestIteration2() {

    ImGui::Begin("Phase 2 Test panel");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("Forces"))
    {
        if (ImGui::Button("Test Gravity")) {

            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;
            //gameObjects->clear();

            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
            GameObject* go = new GameObject("Left particle");
            Particle* particle = new Particle
            (
                Vector3D(-10, 0, 20),
                Vector3D(10, 10, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.999f,
                1.0f
            );
            ParticleGravity* particleGravity = new ParticleGravity({ 0.0f,-10.0f,0.0f });
            math->GetParticleForceRegistry()->AddForce(particle, particleGravity);
            go->AddComponent(particle);

            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);

        }

        if (ImGui::Button("Test Friction")) {

            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
            GameObject* go = new GameObject("Friction particle");
            Particle* particle = new Particle
            (
                Vector3D(-10, 0, 20),
                Vector3D(100, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.95f,
                1.0f
            );

            go->AddComponent(particle);

            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);
        }


        ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        if (ImGui::TreeNode("Springs"))
        {
            static int anchoredSpringConstant = 8;
            ImGui::InputInt("Constant factor (N/m)", &anchoredSpringConstant);
            static int anchoredSpringRestLength = 6;
            ImGui::InputInt("Rest length (m)", &anchoredSpringRestLength);

            if (ImGui::Button("Test anchored spring")) {

                for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

                GameObject* go = new GameObject("anchord block");
                MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

                Particle* particleAnchord = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
                go->AddComponent(particleAnchord);
                VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);
                gameObjects->push_back(go);

                go = new GameObject("spring block");

                Particle* particleSpring = new Particle(Vector3D(0, -5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                go->AddComponent(particleSpring);

                math->GetParticleForceRegistry()->AddForce(particleSpring, math->particleGravity);
                ParticleAnchoredSpring* particleAnchoredSpring = new ParticleAnchoredSpring(particleAnchord->position, anchoredSpringConstant, anchoredSpringRestLength);

                math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);
                gameObjects->push_back(go);

            }

            if (ImGui::Button("Test spring between two objects")) {

                for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

                GameObject* go = new GameObject("spring block 1");
                MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

                Particle* particleSpring1 = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                go->AddComponent(particleSpring1);
                VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);
                gameObjects->push_back(go);

                go = new GameObject("spring block 2");

                Particle* particleSpring2 = new Particle(Vector3D(0, -5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                go->AddComponent(particleSpring2);

                //math->GetParticleForceRegistry()->AddForce(particleSpring, math->particleGravity);


                ParticleSpring* particleSpringForce = new ParticleSpring(particleSpring1, 8, 6);
                math->GetParticleForceRegistry()->AddForce(particleSpring2, particleSpringForce);

                ParticleSpring* particleSpringForce2 = new ParticleSpring(particleSpring2, 8, 6);
                math->GetParticleForceRegistry()->AddForce(particleSpring1, particleSpringForce2);


                v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);
                gameObjects->push_back(go);

            }

            if (ImGui::Button("Test elastic bungee between two objects")) {

                for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

                GameObject* go = new GameObject("elastic bungee block 1");
                MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

                Particle* particleSpring1 = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.8f, 1.0f);
                go->AddComponent(particleSpring1);
                VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);
                gameObjects->push_back(go);
                go = new GameObject("elastic bungee block 2");

                Particle* particleSpring2 = new Particle(Vector3D(0, -5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.6f, 1.0f);
                go->AddComponent(particleSpring2);

                ParticleElasticBungee* particleElasticBungeeForce1 = new ParticleElasticBungee(particleSpring1, 7, 8);
                math->GetParticleForceRegistry()->AddForce(particleSpring2, particleElasticBungeeForce1);

                ParticleElasticBungee* particleElasticBungeeForce2 = new ParticleElasticBungee(particleSpring2, 7, 8);
                math->GetParticleForceRegistry()->AddForce(particleSpring1, particleElasticBungeeForce2);


                v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);
                gameObjects->push_back(go);

            }

            static int distance = 7;
            ImGui::InputInt("Distance with Main Blob (m)", &distance);
            if (ImGui::Button("Test blob")) {

                for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

                GameObject* go = new GameObject("main block");
                MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

                Vector3D mainPosition = Vector3D(0, 0, 20);

                Particle* particleAnchord = new Particle(mainPosition, Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.9f, 1, 1.0f);
                go->AddComponent(particleAnchord);
                inputForce = &particleAnchord->inputValues;
                InputForce* newInputForce = new InputForce();

                MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particleAnchord, newInputForce);
                VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go->AddComponent(v);
                gameObjects->push_back(go);

                const int maxBlob = 8;

                std::vector<Particle*> blobs;

                std::vector<Vector3D> positions = { { -1,1,0 }, { 0,1,0 },{ 1,1,0 },{ 1,0,0 }, { 1,-1,0 },{ 0,-1,0 },{ -1,-1,0 },{ -1,0,0 } };
                Vector3D standardVec = Vector3D(distance, distance, 0);
                for (int i = 1; i <= maxBlob; i++)
                {
                    positions[i - 1].x *= standardVec.x;
                    positions[i - 1].y *= standardVec.y;

                    go = new GameObject("Blob block " + std::to_string(i));
                    Particle* particleSpring = new Particle(mainPosition + positions[i - 1], Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                    go->AddComponent(particleSpring);

                    v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                    go->AddComponent(v);
                    gameObjects->push_back(go);

                    ParticleSpring* particleAnchoredSpring = new ParticleSpring(particleAnchord, anchoredSpringConstant, anchoredSpringRestLength);
                    math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                    if (i != 1) {
                        particleAnchoredSpring = new ParticleSpring(blobs.back(), anchoredSpringConstant, anchoredSpringRestLength);
                        math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                        particleAnchoredSpring = new ParticleSpring(particleSpring, anchoredSpringConstant, anchoredSpringRestLength);
                        math->GetParticleForceRegistry()->AddForce(blobs.back(), particleAnchoredSpring);
                    }

                    if (i == maxBlob) {

                        particleAnchoredSpring = new ParticleSpring(blobs.front(), anchoredSpringConstant, anchoredSpringRestLength);
                        math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                        particleAnchoredSpring = new ParticleSpring(particleSpring, anchoredSpringConstant, anchoredSpringRestLength);
                        math->GetParticleForceRegistry()->AddForce(blobs.front(), particleAnchoredSpring);
                    }

                    blobs.push_back(particleSpring);




                }

            }

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("Collisions"))
    {
        if (ImGui::Button("Test collision")) {

            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
            GameObject* go = new GameObject("Left particle");
            Particle* particle = new Particle
            (
                Vector3D(-10, 0, 20),
                Vector3D(10, 5, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.999f,
                1.0f
            );
            ParticleGravity* particleGravity = new ParticleGravity({ 0.0f,-10.0f,0.0f });
            math->GetParticleForceRegistry()->AddForce(particle, particleGravity);
            go->AddComponent(particle);

            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);

            GameObject* go2 = new GameObject("Right particle");
            Particle* particle2 = new Particle
            (
                Vector3D(10, 0, 20),
                Vector3D(-10, 5, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.999f,
                1.0f
            );

            ParticleGravity* particleGravity2 = new ParticleGravity({ 0.0f,-10.0f,0.0f });
            math->GetParticleForceRegistry()->AddForce(particle2, particleGravity2);
            go2->AddComponent(particle2);

            v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go2->AddComponent(v);
            gameObjects->push_back(go2);
        }

        if (ImGui::Button("Test penetration and resting contact")) {

            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

            GameObject* go = new GameObject("Heavy non gravity block");
            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

            Particle* particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
            go->AddComponent(particle);
            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);

            go = new GameObject("Falling block");

            particle = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.95f, 1.0f);
            go->AddComponent(particle);
            math->GetParticleForceRegistry()->AddForce(particle, math->particleGravity);
            v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);

            /*go = new GameObject();

            particle = new Particle(Vector3D(0, 50, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.95f, 1.0f);
            go->AddComponent(particle);
            math->GetParticleForceRegistry()->AddForce(particle, math->particleGravity);
            v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);*/

        }

        static int rod = 2;
        ImGui::InputInt("rod Numbers", &rod);
        if (ImGui::Button("Test ROD")) {


            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;

            GameObject* go = new GameObject("Heavy non gravity block");
            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

            Particle* particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
            go->AddComponent(particle);
            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);

            for (int i = 0; i < rod; i++) {

                GameObject* go2 = new GameObject("Rod Extremity " + std::to_string(i));


                Particle* p2 = new Particle(Vector3D(0.1, 5 * (i + 1), 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.999f, 1);
                go2->AddComponent(p2);
                math->GetParticleForceRegistry()->AddForce(p2, math->particleGravity);
                VisualGameObject* v2 = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go2->AddComponent(v2);

                gameObjects->push_back(go2);

                math->TestRodCollisionSetup(p2, particle, 5);

                particle = p2;
            }

        }

        static int cable = 2;
        ImGui::InputInt("Cable Numbers", &cable);
        if (ImGui::Button("Test Cable")) {
            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->shouldDelete = true;
            GameObject* go;
            Particle* particle;
            VisualGameObject* v;
            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

            go = new GameObject("Heavy non gravity block");

            particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
            go->AddComponent(particle);
            v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);



            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(-5.0f, 5.0f);

            for (int i = 0; i < cable; i++) {

                GameObject* go2 = new GameObject("Cable extremity " + std::to_string(i));

                float posx = dis(gen);
                float posy = dis(gen);

                Particle* p2 = new Particle(Vector3D(posx, posy, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 1, 1);
                go2->AddComponent(p2);
                math->GetParticleForceRegistry()->AddForce(p2, math->particleGravity);
                VisualGameObject* v2 = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
                go2->AddComponent(v2);

                gameObjects->push_back(go2);

                math->TestCableCollisionSetup(p2, particle, 10);
            }

        }

        ImGui::TreePop();
    }



    ImGui::End();

}

void ImGuiEngine::SetVisible(bool _visible)
{
    if (visible == _visible) {
        return;
    }

    visible = _visible;

    if (visible) {
        ImGui_ImplGlfw_InstallCallbacks(window);
    }
    else {
        ImGui_ImplGlfw_RestoreCallbacks(window);

    }

}