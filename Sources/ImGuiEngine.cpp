
#include "ImGuiEngine.h"

#include "vulkan/vulkan_core.h"
#include <iostream>
#include "GLFW/glfw3.h"
#include "../MathPhysicEngine/Particle.h"
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleGravity.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ConstantForce.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleSpring.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleAnchoredSpring.h"
#include <random>
#include "../MathPhysicEngine/Forces/ParticuleForces/ParticleElasticBungee.h"
#include "../MathPhysicEngine/Forces/ParticuleForces/InputForce.h"
#include "../MathPhysicEngine/Forces/RigidBodyForces/RigidBodyGravity.h"
#include "../MathPhysicEngine/Forces/RigidBodyForces/RigidBodySpring.h"

ImGuiEngine::ImGuiEngine(GLFWwindow *_window, std::vector<GameObject *> *_gameObjects) : window(_window), gameObjects(_gameObjects)
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
    if (inputForce)
        ImGuiInput.BlobControls(window, *inputForce);

    if (visibilityTrigger)
        SetVisible(!visible);

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (visible)
    {

        // Show Demo ImGUI
        ShowEngineImGui();
    }
}

CollisionPrimitive* ImGuiEngine::ChooseShape(CollisionShape shape)
{
    CollisionPrimitive* primitive = new Sphere(1.0f);

    switch (shape)
    {
    case CollisionShape::SPHERE :
		modelePath = "Models/sphere.obj";
		modeleInertiaTensor = "sphere";
        primitive = new Sphere(1.0f);

		break;
    case CollisionShape::BOX :
        modelePath = "Models/colored_cube.obj";
        modeleInertiaTensor = "cuboid";
        primitive = new Box({ 1.0f,1.0f,1.0f });
        break;
    case CollisionShape::PLANE :
        modelePath = "Models/quad model.obj";
		modeleInertiaTensor = "cuboid";
        primitive = new Plane({0.0f,1.0f,0.0f },-8);
		break;
    default:
        break;
    }

    return primitive;
}

void ImGuiEngine::ShowDemoImGui()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (this->show_demo_window)
        ImGui::ShowDemoWindow(&this->show_demo_window);

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // std::cout << "ou";

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static int counter = 0;
        static float f = 0.0f;

        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");                // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &this->show_demo_window); // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &this->show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);                   // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float *)&this->clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (this->show_another_window)
    {
        ImGui::Begin("Another Window", &this->show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
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
    ImGui::Text("Initial rotation speed");
    ImGui::InputFloat("rot x", &rotationX);
    ImGui::InputFloat("rot y", &rotationY);
    ImGui::InputFloat("rot z", &rotationZ);

    ImGui::Text("Initial orientation");
    ImGui::InputFloat("ori w", &orientationW);
    ImGui::InputFloat("ori i", &orientationI);
    ImGui::InputFloat("ori j", &orientationJ);
    ImGui::InputFloat("ori k", &orientationK);

    if (ImGui::Button("Add Particle Game Object"))
    {
        Particle *particle = new Particle(
            Vector3D(positionX, positionY, positionZ),
            Vector3D(velocityX, velocityY, velocityZ),
            Vector3D(0.0f, 0.0f, 0.0f),
            inversedMass,
            damping,
            gravity);
        GameObject *go = new GameObject(name);

        go->AddComponent(particle);

        ParticleGravity *particleGravity = new ParticleGravity({0.0f, -10.0f, 0.0f});
        MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleGravity);

        VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);

        gameObjects->push_back(go);
    }
    if (ImGui::Button("Add RigidBody Game Object"))
    {
        RigidBody *rigidBody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(velocityX, velocityY, velocityZ),               // velocity
            Vector3D(0.0f, 0.0f, 0.0f),                              // Acceleration
            {orientationW, orientationI, orientationJ, orientationK} // Orientation
            // inversedMass,//
            // damping,
            // gravity
        );
        rigidBody->SetGravityScale(gravity);
        GameObject *go = new GameObject(name);
        go->transform.position = Vector3D(positionX, positionY, positionZ);
        go->transform.rotation = Vector3D(rotationX, rotationY, rotationZ);

        go->AddComponent(rigidBody);

        RigidBodyGravity *rigidBodyGravity = new RigidBodyGravity({0.0f, -10.0f, 0.0f});
        MathPhysicsEngine::GetInstance()->GetRigidBodyForceRegistry()->AddForce(rigidBody, rigidBodyGravity);

        VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);

        gameObjects->push_back(go);
    }

    if (ImGui::Button("Colored Cube"))
    {
        modelePath = "Models/colored_cube.obj";
        modeleInertiaTensor = "cuboid";
    }
    ImGui::SameLine();
    if (ImGui::Button("Sphere"))
    {
        modelePath = "Models/sphere.obj";
        modeleInertiaTensor = "sphere";
    }

    if (ImGui::Button("Smooth vase"))
    {
        modelePath = "Models/smooth_vase.obj";
        modeleInertiaTensor = "cuboid";
    }

    ImGui::End();

    ImGui::Begin("Objects List");

    if (ImGui::Button("Pause/Restart Physics motor")) {
        MathPhysicsEngine::GetInstance()->pause = !MathPhysicsEngine::GetInstance()->pause;
    }
    ImGui::Checkbox("pause on collision", &MathPhysicsEngine::GetInstance()->pauseOnCollision);

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

        if (ImGui::TreeNode((std::to_string(i) + ". " + gameObjects->at(i)->GetName()).c_str()))
        {
            if (particle)
            {
                if (ImGui::TreeNode("Add Unique constant force"))
                {

                    ImGui::InputDouble("force x (N)", &force[i].x);
                    ImGui::InputDouble("force y (N)", &force[i].y);
                    ImGui::InputDouble("force z (N)", &force[i].z);

                    ImGui::Spacing();

                    if (ImGui::Button("Apply Input force"))
                    {
                        InputForce *newInputForce = new InputForce();
                        particle->inputValues = force[i];

                        MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, newInputForce);
                    }

                    if (ImGui::Button("Apply constant force"))
                    {
                        ConstantForce *newConstantForce = new ConstantForce(force[i]);
                        MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, newConstantForce);
                    }

                    ImGui::TreePop();
                }
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
            if (particle)
            {
                if (ImGui::TreeNode("Show PARTICULE data"))
                {

                    ImGui::Spacing();
                    // if (ImGui::Checkbox(" print particle data on terminal ", &particle->printParticleOnTerminal))
                    ImGui::Spacing();

                    ImGui::Text(" position: %.2f, %.2f, %.2f (m)", particle->position.x, particle->position.y, particle->position.z);
                    ImGui::Text(" velocity: %.2f, %.2f, %.2f (m/s)", particle->velocity.x, particle->velocity.y, particle->velocity.z);
                    ImGui::Text(" acceleration: %.2f, %.2f, %.2f (m/s^2)", particle->acceleration.x, particle->acceleration.y, particle->acceleration.z);

                    // ImGui::Text(" force: %.2f, %.2f, %.2f", particle->force.x, particle->force.y, particle->force.z);
                    // ImGui::Text(" gravity force: %.2f, %.2f, %.2f", particle->gravityForce.x, particle->gravityForce.y, particle->gravityForce.z);

                    ImGui::Spacing();
                    ImGui::Spacing();

                    ImGui::InputFloat(" inversed mass: (Kg^-1)", &particle->inversedMass);
                    ImGui::InputFloat(" gravity scale: ", &particle->gravity);
                    ImGui::InputFloat(" damping: (Kg/s)", &particle->damping);

                    ImGui::TreePop();
                }
            }
            if (rigidBody)
            {
                if (ImGui::TreeNode("Show RIGIDBODY data"))
                {

                    ImGui::Spacing();
                    // if (ImGui::Checkbox(" print particle data on terminal ", &particle->printParticleOnTerminal))
                    ImGui::Spacing();

                    ImGui::Spacing();
                    ImGui::Text("LINEAR DATA");
                    ImGui::Spacing();

                    ImGui::Text(" position: %.2f, %.2f, %.2f (m)", rigidBody->transform->position.x, rigidBody->transform->position.y, rigidBody->transform->position.z);
                    ImGui::Text(" velocity: %.2f, %.2f, %.2f (m/s)", rigidBody->GetVelocity().x, rigidBody->GetVelocity().y, rigidBody->GetVelocity().z);
                    ImGui::Text(" linear acceleration: %.2f, %.2f, %.2f (m/s^2) ", rigidBody->GetLinearAcceleration().x, rigidBody->GetLinearAcceleration().y, rigidBody->GetLinearAcceleration().z);
                    ImGui::Text(" linear damping: %.2f ", rigidBody->GetLinearDamping());
                    ImGui::Text(" mass: %.2f (Kg)", rigidBody->GetMass());

                    ImGui::Spacing();
                    ImGui::Text("ANGULAR DATA");
                    ImGui::Spacing();

                    ImGui::Text(" angular damping: %.2f ", rigidBody->GetAngularDamping());
                    ImGui::Text("rotation: %.2f, %.2f, %.2f (m)", rigidBody->transform->rotation.x, rigidBody->transform->rotation.y, rigidBody->transform->rotation.z);
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
                    // ImGui::Text(" force: %.2f, %.2f, %.2f", particle->force.x, particle->force.y, particle->force.z);
                    // ImGui::Text(" gravity force: %.2f, %.2f, %.2f", particle->gravityForce.x, particle->gravityForce.y, particle->gravityForce.z);

                    ImGui::Spacing();
                    ImGui::Spacing();

                    // ImGui::InputFloat(" inversed mass: (Kg^-1)", &particle->inversedMass);
                    // ImGui::InputFloat(" gravity scale: ", &particle->gravity);
                    // ImGui::InputFloat(" damping: (Kg/s)", &particle->damping);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("ORIENTATION"))
                {

                    ImGui::InputFloat("ori w", &orientationVector[0][i]);
                    ImGui::InputFloat("ori i", &orientationVector[1][i]);
                    ImGui::InputFloat("ori j", &orientationVector[2][i]);
                    ImGui::InputFloat("ori k", &orientationVector[3][i]);

                    ImGui::Spacing();

                    if (ImGui::Button("Apply"))
                    {

                        gameObjects->at(index[i])->GetComponentOfType<RigidBody>()->SetOrientation(orientationVector[0][i], orientationVector[1][i], orientationVector[2][i], orientationVector[3][i]);
                        // Particle* other = new Particle();
                        // Vector3D anchoredPos = gameObjects->at(index[i])->GetComponentOfType<Particle>()->position;
                        // ParticleAnchoredSpring* particleAnchoredSpringApply = new ParticleAnchoredSpring(anchoredPos, restLength[i], springConstant[i]);

                        // MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleAnchoredSpringApply);
                    }

                    ImGui::TreePop();
                }

                if(ImGui::TreeNode("Transform Matrix"))
                {
                    ImGui::Spacing();
                    ImGui::Text("transform Matrix: ");
                    ImGui::Text("%.2f, %.2f, %.2f, %.2f", rigidBody->transformMatrix.values[0], rigidBody->transformMatrix.values[1], rigidBody->transformMatrix.values[2], rigidBody->transformMatrix.values[3]);
                    ImGui::Text("%.2f, %.2f, %.2f, %.2f", rigidBody->transformMatrix.values[4], rigidBody->transformMatrix.values[5], rigidBody->transformMatrix.values[6], rigidBody->transformMatrix.values[7]);
                    ImGui::Text("%.2f, %.2f, %.2f, %.2f", rigidBody->transformMatrix.values[8], rigidBody->transformMatrix.values[9], rigidBody->transformMatrix.values[10], rigidBody->transformMatrix.values[11]);


                    ImGui::TreePop();

                }
            }

            ImGui::TreePop();
            ImGui::Spacing();
        }
    }
    ImGui::End();

    //TestIteration2();
    //TestIteration3();
    TestIteration4();
    ContactsList();
    //TesGraphicMotor();
}


void ImGuiEngine::ContactsList() {
    ImGui::Begin("Contacts List");

    /*
    static std::vector<Vector3D> force(200);
    static std::vector<float> restLength(200);
    static std::vector<float[4]> orientationVector(200);

    static std::vector<float> springConstant(200);
    static std::vector<int> index(200);
    */

    if (!MathPhysicsEngine::GetInstance()->rigidbodiesContact.empty()) 
    {
        for (int i = 0; i < MathPhysicsEngine::GetInstance()->rigidbodiesContact.size(); i++)
        {
            auto rigidbodyContact = MathPhysicsEngine::GetInstance()->rigidbodiesContact.at(i);

            //if (ImGui::TreeNode((rigidbodyContact->rigidbody[0]->gameObject->GetName() + " - " + rigidbodyContact->rigidbody[1]->gameObject->GetName()).c_str()))
            if (ImGui::TreeNode((" contact number " + std::to_string(i)+" :").c_str()))
            {
                /*
                if (!rigidbodyContact->rigidbody[0]->gameObject->GetName().empty() && !rigidbodyContact->rigidbody[1]->gameObject->GetName().empty()) 
                {
                    ImGui::Text((rigidbodyContact->rigidbody[0]->gameObject->GetName() + " - " + rigidbodyContact->rigidbody[1]->gameObject->GetName()).c_str());
                }*/
                ImGui::Text(" contact normal: %.2f, %.2f, %.2f ", rigidbodyContact->contactNormal.x, rigidbodyContact->contactNormal.y, rigidbodyContact->contactNormal.z);
                ImGui::Text(" contact point: %.2f, %.2f, %.2f ", rigidbodyContact->contactPoint.x, rigidbodyContact->contactPoint.y, rigidbodyContact->contactPoint.z);
                ImGui::Text(" penetration: %.4f", rigidbodyContact->penetration);

                /*
                    if (ImGui::TreeNode("ORIENTATION"))
                    {

                        ImGui::InputFloat("ori w", &orientationVector[0][i]);
                        ImGui::InputFloat("ori i", &orientationVector[1][i]);
                        ImGui::InputFloat("ori j", &orientationVector[2][i]);
                        ImGui::InputFloat("ori k", &orientationVector[3][i]);

                        ImGui::Spacing();

                        if (ImGui::Button("Apply"))
                        {

                            gameObjects->at(index[i])->GetComponentOfType<RigidBody>()->SetOrientation(orientationVector[0][i], orientationVector[1][i], orientationVector[2][i], orientationVector[3][i]);
                            // Particle* other = new Particle();
                            // Vector3D anchoredPos = gameObjects->at(index[i])->GetComponentOfType<Particle>()->position;
                            // ParticleAnchoredSpring* particleAnchoredSpringApply = new ParticleAnchoredSpring(anchoredPos, restLength[i], springConstant[i]);

                            // MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particle, particleAnchoredSpringApply);
                        }

                        ImGui::TreePop();
                    }


                */
                ImGui::TreePop();
            }


            ImGui::Spacing();

        }
    }
    
    ImGui::End();
}
void ImGuiEngine::TesGraphicMotor()
{
    ImGui::Begin("Test GraphicMotor");
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::Button("Spawn components"))
    {
        GameObject *go = new GameObject("floor");
        VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject("Models/quad model.obj");
        v->transformVisual.translation = {0.f, .5f, 0.f};
        go->AddComponent(v);
        gameObjects->push_back(go);

        go = new GameObject("Vase");
        v = VisualGameObject::CreatePtrVisualGameObject("Models/smooth_vase.obj");
        v->transformVisual.translation = {.5f, .5f, 0.f};
        go->AddComponent(v);
        gameObjects->push_back(go);

        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f} //
        };

        for (int i = 0; i < lightColors.size(); i++)
        {
            GameObject* go2 = new GameObject("Light");
            VisualGameObject* v2 = VisualGameObject::makePointLight(0.2f);
            v2->color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                {0.f, -1.f, 0.f});
            v2->transformVisual.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            go2->AddComponent(v2);
            gameObjects->push_back(go2);
        }
    }

    ImGui::End();
}

void ImGuiEngine::TestIteration4()
{
    ImGui::Begin("Phase 4 Test RIGIDBODY panel");
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    //if (ImGui::Button("Test Broad phase"))

    static CollisionShape shapeUp = CollisionShape::SPHERE;
    static CollisionShape shapeDown = CollisionShape::SPHERE;

    if (ImGui::TreeNode("Broad Phase"))
    {
        ImGui::Text("Potential collisions number:  %d", MathPhysicsEngine::GetInstance()->potentialCollision.size());

        if (ImGui::Button("Test Broad Phase 1: no touching")) {
            modelePath = "Models/colored_cube.obj";
            modeleInertiaTensor = "cuboid";

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    GameObject* go = new GameObject("rigidBody x:" + std::to_string(x) + " y:" + std::to_string(y));
                    go->transform.position = Vector3D(1.45 + 2.9 * x, 1.45 + 2.9 * y, 20);
                    RigidBody* rigidbody = new RigidBody(
                        modeleInertiaTensor,
                        Vector3D(0, 0, 0),
                        Vector3D(0.0f, 0.0f, 0.0f),
                        { 1, 0, 0, 0 },
                        Matrix33(0.0f),
                        0.0001f, //damping
                        0.f //gravity
                    );
                    go->AddComponent(rigidbody);
                    VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                    go->AddComponent(v);
                    CollisionPrimitive* collisionPrimitive = new Sphere(1.0f);
                    collisionPrimitive->rigidBody = rigidbody;
                    collisionPrimitive->UpdateTransformMatrix();

                    rigidbody->collisionPrimitive = collisionPrimitive;
                    gameObjects->push_back(go);
                }
            }
        }

        if (ImGui::Button("Test Broad Phase 2: no touching with rotation")) {
            modelePath = "Models/colored_cube.obj";
            modeleInertiaTensor = "cuboid";

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    GameObject* go = new GameObject("rigidBody x:" + std::to_string(x) + " y:" + std::to_string(y));
                    go->transform.position = Vector3D(1.45 + 2.9 * x, 1.45 + 2.9 * y, 20);
                    RigidBody* rigidbody = new RigidBody(
                        modeleInertiaTensor,
                        Vector3D(0, 0, 0),
                        Vector3D(0.0f, 0.0f, 0.0f),
                        { 1, 0, 0, 0 },
                        Matrix33(0.0f),
                        0.0001f, //damping
                        0.f //gravity
                    );
                    rigidbody->orientation = Quaternion(0.3826834, 0, 0, 0.9238795);
                    go->AddComponent(rigidbody);
                    VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                    go->AddComponent(v);
                    CollisionPrimitive* collisionPrimitive = new Sphere(1.0f);
                    collisionPrimitive->rigidBody = rigidbody;
                    collisionPrimitive->UpdateTransformMatrix();

                    rigidbody->collisionPrimitive = collisionPrimitive;
                    gameObjects->push_back(go);
                }
            }
        }

        if (ImGui::Button("Test Broad Phase 2: touching: move on x axis")) {
            modelePath = "Models/colored_cube.obj";
            modeleInertiaTensor = "cuboid";

            modelePath = "Models/colored_cube.obj";
            modeleInertiaTensor = "cuboid";

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    GameObject* go = new GameObject("rigidBody x:" + std::to_string(x) + " y:" + std::to_string(y));
                    go->transform.position = Vector3D(2.9 * x, 1.45f + 2.9 * y, 20);
                    RigidBody* rigidbody = new RigidBody(
                        modeleInertiaTensor,
                        Vector3D(0, 0, 0),
                        Vector3D(0.0f, 0.0f, 0.0f),
                        { 1, 0, 0, 0 },
                        Matrix33(0.0f),
                        0.0001f, //damping
                        0.f //gravity
                    );
                    rigidbody->orientation = Quaternion(0.3826834, 0, 0, 0.9238795);
                    go->AddComponent(rigidbody);
                    VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                    go->AddComponent(v);
                    CollisionPrimitive* collisionPrimitive = new Sphere(1.0f);
                    collisionPrimitive->rigidBody = rigidbody;
                    collisionPrimitive->UpdateTransformMatrix();

                    rigidbody->collisionPrimitive = collisionPrimitive;

                    gameObjects->push_back(go);
                }
            }
        }

        if (ImGui::Button("Test Broad Phase 2: touching: move on x & y axis")) {
            modelePath = "Models/colored_cube.obj";
            modeleInertiaTensor = "cuboid";

            modelePath = "Models/colored_cube.obj";
            modeleInertiaTensor = "cuboid";

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    GameObject* go = new GameObject("rigidBody x:" + std::to_string(x) + " y:" + std::to_string(y));
                    go->transform.position = Vector3D(2.9 * x, 2.9 * y, 20);
                    RigidBody* rigidbody = new RigidBody(
                        modeleInertiaTensor,
                        Vector3D(0, 0, 0),
                        Vector3D(0.0f, 0.0f, 0.0f),
                        { 1, 0, 0, 0 },
                        Matrix33(0.0f),
                        0.0001f, //damping
                        0.f //gravity
                    );
                    rigidbody->orientation = Quaternion(0.3826834, 0, 0, 0.9238795);
                    go->AddComponent(rigidbody);
                    VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                    go->AddComponent(v);
                    CollisionPrimitive* collisionPrimitive = new Sphere(1.0f);
                    collisionPrimitive->rigidBody = rigidbody;
                    collisionPrimitive->UpdateTransformMatrix();

                    rigidbody->collisionPrimitive = collisionPrimitive;

                    gameObjects->push_back(go);
                }
            }
        }

        ImGui::TreePop();
    }

   // bool showDropdown = false;
    //const char* items[] = { "Option 1", "Option 2", "Option 3" };
   // int selectedItem = -1;

    if (ImGui::TreeNode("Narrow Phase"))
    {

        static float positionTestCollision1[3] = { 0.0f, 0.0f, 0.0f };
        ImGui::InputFloat3("position 1", positionTestCollision1);

        static float positionTestCollision2[3] = { 3.0f, 0.0f, 0.0f };
        ImGui::InputFloat3("position 2", positionTestCollision2);

        if (ImGui::Button("Test potential collision"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
            GameObject* go = new GameObject("rigidBody1");
            go->transform.position = Vector3D(positionTestCollision1[0], positionTestCollision1[1], positionTestCollision1[2]);
            go->transform.rotation = Vector3D(0.0f, 0.0f, 0.0f);

            RigidBody* rigidbody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                { 1, 0, 0, 0 });

            rigidbody->SetGravityScale(0.0f);
            //RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
           // math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);


            Sphere* box = new Sphere(1.0f);
            box->rigidBody = rigidbody;
            box->UpdateTransformMatrix();

            rigidbody->collisionPrimitive = box;

            go->AddComponent(rigidbody);

            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/sphere.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);


            //SPHERE COLLISION SHAPE

            go = new GameObject("rigidBody2");
            go->transform.position = Vector3D(positionTestCollision2[0], positionTestCollision2[1], positionTestCollision2[2]);
            go->transform.rotation = Vector3D(0.0f, 0.0f, 0.0f);

            rigidbody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                { 1, 0, 0, 0 });

            rigidbody->SetGravityScale(0.0f);
            //RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
           // math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);

            Sphere* sphere = new Sphere(1.0f);
            sphere->rigidBody = rigidbody;
            sphere->UpdateTransformMatrix();

            rigidbody->collisionPrimitive = sphere;

            go->AddComponent(rigidbody);

            v = VisualGameObject::CreatePtrVisualGameObject("Models/sphere.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);
        }

        if (ImGui::Button("Test collision boxes"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
            GameObject* go = new GameObject("rigidBody1");
            go->transform.position = Vector3D(positionTestCollision1[0], positionTestCollision1[1], positionTestCollision1[2]);
            go->transform.rotation = Vector3D(0.0f, 0.0f, 0.0f);

            RigidBody* rigidbody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                { 1, 0, 0, 0 });

            rigidbody->SetGravityScale(0.0f);
            //RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
           // math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);


            Box* box1 = new Box({ 1.0f ,1.0f ,1.0f });
            box1->rigidBody = rigidbody;
            box1->UpdateTransformMatrix();

            rigidbody->collisionPrimitive = box1;

            go->AddComponent(rigidbody);

            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);


            //BOX 2 COLLISION SHAPE

            go = new GameObject("rigidBody2");
            go->transform.position = Vector3D(positionTestCollision2[0], positionTestCollision2[1], positionTestCollision2[2]);
            go->transform.rotation = Vector3D(0.0f, 0.0f, 0.0f);

            rigidbody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                { 1, 0, 0, 0 });

            rigidbody->SetGravityScale(0.0f);
            //RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
           // math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);

            Box* box2 = new Box({ 1.0f ,1.0f ,1.0f });
            box2->rigidBody = rigidbody;
            box2->UpdateTransformMatrix();

            rigidbody->collisionPrimitive = box2;

            go->AddComponent(rigidbody);

            v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);
        }

        if (ImGui::Button("Test collision box and sphere"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
            GameObject* go = new GameObject("rigidBody1");
            go->transform.position = Vector3D(positionTestCollision1[0], positionTestCollision1[1], positionTestCollision1[2]);
            go->transform.rotation = Vector3D(0.0f, 0.0f, 0.0f);

            RigidBody* rigidbody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                { 1, 0, 0, 0 });

            rigidbody->SetGravityScale(0.0f);
            //RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
           // math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);


            Box* box1 = new Box({ 1.0f ,1.0f ,1.0f });
            box1->rigidBody = rigidbody;
            box1->UpdateTransformMatrix();

            rigidbody->collisionPrimitive = box1;

            go->AddComponent(rigidbody);

            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);


            //BOX 2 COLLISION SHAPE

            go = new GameObject("rigidBody2");
            go->transform.position = Vector3D(positionTestCollision2[0], positionTestCollision2[1], positionTestCollision2[2]);
            go->transform.rotation = Vector3D(0.0f, 0.0f, 0.0f);

            rigidbody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                { 1, 0, 0, 0 });

            rigidbody->SetGravityScale(0.0f);
            //RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
           // math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);

            Sphere* sphere = new Sphere(1.0f);
            sphere->rigidBody = rigidbody;
            sphere->UpdateTransformMatrix();

            rigidbody->collisionPrimitive = sphere;

            go->AddComponent(rigidbody);

            v = VisualGameObject::CreatePtrVisualGameObject("Models/sphere.obj");
            go->AddComponent(v);
            gameObjects->push_back(go);
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Collision resolution"))
    {



    const char* items[] = { "Sphere", "Box", "Plane" };
    static const char* current_itemUp = items[0];
  


    ImGui::Text("Choose UP shape");
    if (ImGui::BeginCombo("##t", current_itemUp)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_itemUp == items[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(items[n], is_selected) )
            {
                current_itemUp = items[n];

                switch (n)
                {
                case 0:
                    shapeUp = CollisionShape::SPHERE;

                    break;
                case 1:
                    shapeUp = CollisionShape::BOX;
                    break;
                case 2:
                    shapeUp = CollisionShape::PLANE;
                    break;
                default:
                    break;
                }

                if (is_selected) {
                    ImGui::SetItemDefaultFocus();                     // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                }

            }
        }
        ImGui::EndCombo();
    }


    static const char* current_itemDown = items[0];

    ImGui::Spacing();
    ImGui::Text("Choose DOWN shape");
    if (ImGui::BeginCombo("##combo2", current_itemDown)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_itemDown == items[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(items[n], is_selected))
            {
                current_itemDown = items[n];

                switch (n)
                {
                case 0:
                    shapeDown = CollisionShape::SPHERE;

                    break;
                case 1:
                    shapeDown = CollisionShape::BOX;
                    break;
                case 2:
                    shapeDown = CollisionShape::PLANE;
					break;
                default:
                    break;
                }

                if (is_selected) {
                    ImGui::SetItemDefaultFocus();                     // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                }

            }
        }
        ImGui::EndCombo();
    }

    

    if (ImGui::Button("Test Regular collision"))
    {
        RigidBody::contactRestitution = 1.0f;
        for (int i = 0; i < gameObjects->size(); i++)
            gameObjects->at(i)->SetShouldDelete();

        CollisionPrimitive* collisionPrimitive = ChooseShape(shapeUp);

        MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
        GameObject* go = new GameObject("rigidBody");
        go->transform.position = Vector3D(-10, 0, 20);
        go->transform.rotation = Vector3D(0.0f, 0.0f, 2.0f);

        RigidBody* rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(10, 10, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 });
        rigidbody->orientation = Quaternion(0.3826834, 0, 0, 0.9238795);
        //rigidbody->angularVelocity = Vector3D(0, 0, -1.0f);
        RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
        math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);
        go->AddComponent(rigidbody);
        
        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();
        rigidbody->collisionPrimitive = collisionPrimitive;

        VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);

        collisionPrimitive = ChooseShape(shapeDown);

        go = new GameObject("rigidBody 2");
        go->transform.position = Vector3D(10, 0, 20);
        go->transform.rotation = Vector3D(0.0f, 0.0f, -2.0f);

        rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(-10, 10, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 });
        //rigidbody->angularVelocity = Vector3D(0, 0, 1.0f);
        math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);
        go->AddComponent(rigidbody);

        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();
        rigidbody->collisionPrimitive = collisionPrimitive;

        v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);
    }

    if (ImGui::Button("Test Regular collision with rotation"))
    {
        RigidBody::contactRestitution = 1.0f;
        for (int i = 0; i < gameObjects->size(); i++)
            gameObjects->at(i)->SetShouldDelete();

        CollisionPrimitive* collisionPrimitive = ChooseShape(shapeUp);

        MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
        GameObject* go = new GameObject("rigidBody");
        go->transform.position = Vector3D(-10, 0, 20);
        go->transform.rotation = Vector3D(0.0f, 0.0f, 2.0f);

        RigidBody* rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(10, 10, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 });
        rigidbody->orientation = Quaternion(0.3826834, 0, 0, 0.9238795);
        rigidbody->angularVelocity = Vector3D(0, 0, -1.0f);
        RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
        math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);
        go->AddComponent(rigidbody);

        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();
        rigidbody->collisionPrimitive = collisionPrimitive;

        VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);

        collisionPrimitive = ChooseShape(shapeDown);

        go = new GameObject("rigidBody 2");
        go->transform.position = Vector3D(10, 0, 20);
        go->transform.rotation = Vector3D(0.0f, 0.0f, -2.0f);

        rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(-10, 10, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 });
        rigidbody->angularVelocity = Vector3D(0, 0, 1.0f);
        math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);
        go->AddComponent(rigidbody);

        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();
        rigidbody->collisionPrimitive = collisionPrimitive;

        v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);
    }

    if(ImGui::Button("Test collision on static element")) {
        RigidBody::contactRestitution = 1.0f;
        for (int i = 0; i < gameObjects->size(); i++)
            gameObjects->at(i)->SetShouldDelete();


        CollisionPrimitive* collisionPrimitive = ChooseShape(shapeDown);

        MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
        GameObject* go = new GameObject("rigidBody");
        go->transform.position = Vector3D(0, -8, 20);

        RigidBody* rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(0, 0, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 },
            Matrix33(0.0f),
            0.0001f, //damping
            0.f, //gravity
            0.0001f, //inversed mass
            0.999f //angular damping
        );
        rigidbody->orientation = Quaternion(0.9238795, 0, -0.3826834,0);
        go->AddComponent(rigidbody);

        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();
        rigidbody->collisionPrimitive = collisionPrimitive;

        VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);

        collisionPrimitive = ChooseShape(shapeUp);

        go = new GameObject("rigidBody 2");
        go->transform.position = Vector3D(0, 10, 20);

        rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(0, 0, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 });
        //rigidbody->angularVelocity = Vector3D(0, 0, 1.0f);
        RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
        math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);
        go->AddComponent(rigidbody);

        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();

        rigidbody->collisionPrimitive = collisionPrimitive;

        v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);
    }
    
    if(ImGui::Button("Test collision on static less bouncing, side fall")) {

        RigidBody::contactRestitution = 1.0f;

        for (int i = 0; i < gameObjects->size(); i++)
            gameObjects->at(i)->SetShouldDelete();

        modelePath = "Models/colored_cube.obj";
        modeleInertiaTensor = "cuboid";

        MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
        GameObject* go = new GameObject("rigidBody");
        go->transform.position = Vector3D(0, 0, 20);

        RigidBody* rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(0, 0, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 },
            Matrix33(0.0f),
            0.0001f, //damping
            0.f, //gravity
            0.0001f, //inversed mass
            0.999f //angular damping
        );
        go->AddComponent(rigidbody);

        CollisionPrimitive* collisionPrimitive = new Box(Vector3D(1.0f, 1.0f, 1.0f));
        //CollisionPrimitive* collisionPrimitive = new Sphere(1.0f);
        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();

        rigidbody->collisionPrimitive = collisionPrimitive;

        VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);

        go = new GameObject("rigidBody 2");
        go->transform.position = Vector3D(1.2f, 10, 20);

        rigidbody = new RigidBody(
            modeleInertiaTensor,
            Vector3D(0, 0, 0),
            Vector3D(0.0f, 0.0f, 0.0f),
            { 1, 0, 0, 0 });
        RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f, -10.0f, 0.0f });
        math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);
        go->AddComponent(rigidbody);

        collisionPrimitive = new Box(Vector3D(1.0f, 1.0f, 1.0f));
        //collisionPrimitive = new Sphere(1.0f);
        collisionPrimitive->rigidBody = rigidbody;
        collisionPrimitive->UpdateTransformMatrix();

        rigidbody->collisionPrimitive = collisionPrimitive;

        v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
        go->AddComponent(v);
        gameObjects->push_back(go);
    }
    
        ImGui::TreePop();
    }

    

    ImGui::End();
}

void ImGuiEngine::TestIteration3()
{
    ImGui::Begin("Phase 3 Test RIGIDBODY panel");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    /*
        - Test rotation brut
        - Test v�locit� angulaire : rotate sur un axe par une quantit� = impulsion: pas de force
        - Test de force: sur un point du carr�
        - Ressort ?
    */

    if (ImGui::TreeNode("Forces"))
    {

        if (ImGui::Button("Test Gravity"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();
            // gameObjects->clear();

            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();
            GameObject *go = new GameObject("rigidBody");
            go->transform.position = Vector3D(-10, 0, 20);
            go->transform.rotation = Vector3D(0.0f, 0.0f, 2.0f);

            RigidBody *rigidbody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(10, 10, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                {1, 0, 0, 0});
            RigidBodyGravity *rigidBodyGravity = new RigidBodyGravity({0.0f, -10.0f, 0.0f});
            math->GetRigidBodyForceRegistry()->AddForce(rigidbody, rigidBodyGravity);
            go->AddComponent(rigidbody);

            VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);
        }
        
        if (ImGui::Button("Test angular damping"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();
            GameObject *go = new GameObject("damping rigidBody");
            go->transform.position = Vector3D(0, 0, 5);
            go->transform.rotation = Vector3D(0, 0, 20);
            RigidBody *rigidBody = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                {1, 0, 0, 0},
                1.0f,
                0.95f,
                1.0f,
                1.0f,
                0.95f);

            go->AddComponent(rigidBody);

            VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);
        }

        /*static int rod = 2;
        ImGui::InputInt("rod Numbers", &rod);
        if (ImGui::Button("Test ROD?")) {


            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->SetShouldDelete();

            GameObject* go = new GameObject("Heavy non gravity block");
            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();
            RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f,-10.0f,0.0f });

            RigidBody* rb = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 20), //position
                Vector3D(0, 0, 0), //velocite
                Vector3D(0, 0, 0), //acceleration line
                Vector3D(0,0,0), //rotation
                Quaternion(1,0,0,0), //orientation
                Matrix33(), //invers tensor
                0, //linear damp
                0.0f, //gravity
                .000001f, //invers mass
                0); //angular damping
            go->AddComponent(rb);
            VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);

            for (int i = 0; i < rod; i++) {

                GameObject* go2 = new GameObject("Rod Extremity " + std::to_string(i));


                RigidBody* r2 = new RigidBody(modeleInertiaTensor,Vector3D(0.1, 5 * (i + 1), 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.999f, 1);
                go2->AddComponent(r2);
                math->GetRigidBodyForceRegistry()->AddForce(r2, rigidBodyGravity);
                VisualGameObject* v2 = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go2->AddComponent(v2);

                gameObjects->push_back(go2);

                //math->TestRigidbodyRodCollisionSetup(r2, rb, 5);

                rb = r2;
            }

        }

        static int cable = 2;
        ImGui::InputInt("Cable Numbers", &cable);
        if (ImGui::Button("Test Cable?")) {
            for (int i = 0; i < gameObjects->size(); i++) gameObjects->at(i)->SetShouldDelete();

            RigidBodyGravity* rigidBodyGravity = new RigidBodyGravity({ 0.0f,-10.0f,0.0f });

            GameObject* go;
            VisualGameObject* v;
            MathPhysicsEngine* math = MathPhysicsEngine::GetInstance();

            go = new GameObject("Heavy non gravity block");

            RigidBody* rb = new RigidBody(
                modeleInertiaTensor,
                Vector3D(0, 0, 20), //position
                Vector3D(0, 0, 0), //velocite
                Vector3D(0, 0, 0), //acceleration line
                Vector3D(0, 0, 0), //rotation
                Quaternion(1, 0, 0, 0), //orientation
                Matrix33(), //invers tensor
                0, //linear damp
                0.0f, //gravity
                .000001f, //invers mass
                0); //angular damping
            go->AddComponent(rb);
            v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);



            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(-5.0f, 5.0f);

            for (int i = 0; i < cable; i++) {

                GameObject* go2 = new GameObject("Cable extremity " + std::to_string(i));

                float posx = dis(gen);
                float posy = dis(gen);

                RigidBody* r2 = new RigidBody(modeleInertiaTensor, Vector3D(posx, posy, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 1, 1);
                go2->AddComponent(r2);
                math->GetRigidBodyForceRegistry()->AddForce(r2, rigidBodyGravity);
                VisualGameObject* v2 = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go2->AddComponent(v2);

                gameObjects->push_back(go2);

                math->TestRigidbodyCableCollisionSetup(r2, rb, 10);
            }

        }*/


        //ImGui::TreePop();

        if (ImGui::TreeNode("Springs"))
        {

            static int springConstant = 8;
            ImGui::InputInt("Constant factor (N/m)", &springConstant);
            static int springRestLength = 6;
            ImGui::InputInt("Rest length (m)", &springRestLength);

            static float rotationBlock1[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("rotation block 1", rotationBlock1);
            static float rotationBlock2[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("rotation block 2", rotationBlock2);

            static float connectionPointBlock1[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("connection point block 1", connectionPointBlock1);
            static float connectionPointBlock2[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("connection point block 2", connectionPointBlock2);

            if (ImGui::Button("Test spring between two objects"))
            {

                for (int i = 0; i < gameObjects->size(); i++)
                    gameObjects->at(i)->SetShouldDelete();

                GameObject *go = new GameObject("spring block 1");
                MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();
                go->transform.position = Vector3D(0, 5, 20);
                go->transform.rotation = Vector3D(rotationBlock1[0], rotationBlock1[1], rotationBlock1[2]);

                RigidBody *rbSpring1 = new RigidBody(
                    modeleInertiaTensor,
                    Vector3D(0, 0, 0),
                    Vector3D(0, 0, 0),
                    {1, 0, 0, 0});

                go->AddComponent(rbSpring1);
                VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);

                go = new GameObject("spring block 2");
                go->transform.position = Vector3D(0, -5, 20);
                go->transform.rotation = Vector3D(rotationBlock2[0], rotationBlock2[1], rotationBlock2[2]);

                RigidBody *rbSpring2 = new RigidBody(
                    modeleInertiaTensor,
                    Vector3D(0, 0, 0),
                    Vector3D(0, 0, 0),
                    {1, 0, 0, 0});
                go->AddComponent(rbSpring2);

                RigidBodySpring *rbSpringForce1 = new RigidBodySpring(rbSpring1,
                                                                      {connectionPointBlock1[0], connectionPointBlock1[1], connectionPointBlock1[2]},
                                                                      {connectionPointBlock2[0], connectionPointBlock2[1], connectionPointBlock2[2]},
                                                                      springConstant,
                                                                      springRestLength);

                math->GetRigidBodyForceRegistry()->AddForce(rbSpring2, rbSpringForce1);

                RigidBodySpring *rbSpringForce2 = new RigidBodySpring(rbSpring2,
                                                                      {connectionPointBlock2[0], connectionPointBlock2[1], connectionPointBlock2[2]},
                                                                      {connectionPointBlock1[0], connectionPointBlock1[1], connectionPointBlock1[2]},
                                                                      springConstant,
                                                                      springRestLength);

                math->GetRigidBodyForceRegistry()->AddForce(rbSpring1, rbSpringForce2);

                v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);
            }

            static int anchoredSpringConstant = 8;
            ImGui::InputInt("Constant factor anchored (N/m)", &anchoredSpringConstant);
            static int anchoredSpringRestLength = 6;
            ImGui::InputInt("Rest length anchored (m)", &anchoredSpringRestLength);

            static float anchoredRotationBlock[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("rotation anchored block", anchoredRotationBlock);
            static float springRotationBlock[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("rotation spring block", springRotationBlock);

            static float anchoredconnectionPointBlock[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("anchored point", anchoredconnectionPointBlock);
            static float springconnectionPointBlock[3] = {0.0f, 0.0f, 0.0f};
            ImGui::InputFloat3("spring point", springconnectionPointBlock);

            // static float springconnectionPointBlock[3] = { 0.0f,0.0f,0.0f};
            // ImGui::InputFloat3("spring point", springconnectionPointBlock);

            static float anchoredAngularDampling = 0.7f;
            ImGui::InputFloat("Angular Dampling", &anchoredAngularDampling);

            static float anchoredLinearDampling = 0.9f;
            ImGui::InputFloat("linear Dampling", &anchoredLinearDampling);

            static float anchoredGravityScale = 0.0f;
            ImGui::InputFloat("Gravity scale", &anchoredGravityScale);

            if (ImGui::Button("Test anchored"))
            {

                for (int i = 0; i < gameObjects->size(); i++)
                    gameObjects->at(i)->SetShouldDelete();

                GameObject *go = new GameObject("anchored");
                MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

                go->transform.position = Vector3D(0, 5, 20);
                go->transform.rotation = Vector3D(anchoredRotationBlock[0], anchoredRotationBlock[1], anchoredRotationBlock[2]);

                RigidBody *rbSpringAnchored1 = new RigidBody(
                    modeleInertiaTensor,
                    Vector3D(0, 0, 0),
                    Vector3D(0, 0, 0),
                    {1, 0, 0, 0});

                // rbSpringAnchored1->SetAngularDamping(anchoredAngularDampling);

                go->AddComponent(rbSpringAnchored1);
                VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);

                go = new GameObject("spring anchored block");

                go->transform.position = Vector3D(0, -5, 20);
                go->transform.rotation = Vector3D(springRotationBlock[0], springRotationBlock[1], springRotationBlock[2]);

                RigidBody *rbSpringAnchored2 = new RigidBody(
                    modeleInertiaTensor,
                    Vector3D(0, 0, 0),
                    Vector3D(0, 0, 0),
                    {1, 0, 0, 0});

                rbSpringAnchored2->SetGravityScale(anchoredGravityScale);
                rbSpringAnchored2->SetAngularDamping(anchoredAngularDampling);
                rbSpringAnchored2->SetLinearDamping(anchoredLinearDampling);
                go->AddComponent(rbSpringAnchored2);

                v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);

                RigidBodySpring *rbAnchoredForce = new RigidBodySpring(rbSpringAnchored1,
                                                                       {springconnectionPointBlock[0], springconnectionPointBlock[1], springconnectionPointBlock[2]},
                                                                       {anchoredconnectionPointBlock[0], anchoredconnectionPointBlock[1], anchoredconnectionPointBlock[2]},
                                                                       anchoredSpringConstant,
                                                                       anchoredSpringRestLength);
                math->GetRigidBodyForceRegistry()->AddForce(rbSpringAnchored2, rbAnchoredForce);

                RigidBodyGravity *rigidBodyGravity2 = new RigidBodyGravity({0.0f, -10.0f, 0.0f});
                MathPhysicsEngine::GetInstance()->GetRigidBodyForceRegistry()->AddForce(rbSpringAnchored2, rigidBodyGravity2);
            }

          
        }
        
        ImGui::TreePop();

    }
    
    ImGui::TreePop();

    ImGui::End();
}

void ImGuiEngine::TestIteration2()
{

    ImGui::Begin("Phase 2 Test PARTICULES panel");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("Forces"))
    {
        if (ImGui::Button("Test Gravity"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();
            // gameObjects->clear();

            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();
            GameObject *go = new GameObject("Left particle");
            Particle *particle = new Particle(
                Vector3D(-10, 0, 20),
                Vector3D(10, 10, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.999f,
                1.0f);
            ParticleGravity *particleGravity = new ParticleGravity({0.0f, -10.0f, 0.0f});
            math->GetParticleForceRegistry()->AddForce(particle, particleGravity);
            go->AddComponent(particle);

            VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);
        }

        if (ImGui::Button("Test Friction"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();
            GameObject *go = new GameObject("Friction particle");
            Particle *particle = new Particle(
                Vector3D(-10, 0, 20),
                Vector3D(100, 0, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.95f,
                1.0f);

            go->AddComponent(particle);

            VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
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

            if (ImGui::Button("Test anchored spring"))
            {

                for (int i = 0; i < gameObjects->size(); i++)
                    gameObjects->at(i)->SetShouldDelete();

                GameObject *go = new GameObject("anchord block");
                MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

                Particle *particleAnchord = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
                go->AddComponent(particleAnchord);
                VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);

                go = new GameObject("spring block");

                Particle *particleSpring = new Particle(Vector3D(0, -5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                go->AddComponent(particleSpring);

                math->GetParticleForceRegistry()->AddForce(particleSpring, math->particleGravity);
                ParticleAnchoredSpring *particleAnchoredSpring = new ParticleAnchoredSpring(particleAnchord->position, (float)anchoredSpringConstant, (float)anchoredSpringRestLength);

                math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);
            }

            if (ImGui::Button("Test spring between two objects"))
            {

                for (int i = 0; i < gameObjects->size(); i++)
                    gameObjects->at(i)->SetShouldDelete();

                GameObject *go = new GameObject("spring block 1");
                MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

                Particle *particleSpring1 = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                go->AddComponent(particleSpring1);
                VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);

                go = new GameObject("spring block 2");

                Particle *particleSpring2 = new Particle(Vector3D(0, -5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                go->AddComponent(particleSpring2);

                ParticleSpring *particleSpringForce = new ParticleSpring(particleSpring1, 8, 6);
                math->GetParticleForceRegistry()->AddForce(particleSpring2, particleSpringForce);

                ParticleSpring *particleSpringForce2 = new ParticleSpring(particleSpring2, 8, 6);
                math->GetParticleForceRegistry()->AddForce(particleSpring1, particleSpringForce2);

                v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);
            }

            if (ImGui::Button("Test elastic bungee between two objects"))
            {

                for (int i = 0; i < gameObjects->size(); i++)
                    gameObjects->at(i)->SetShouldDelete();

                GameObject *go = new GameObject("elastic bungee block 1");
                MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

                Particle *particleSpring1 = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.8f, 1.0f);
                go->AddComponent(particleSpring1);
                VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);
                go = new GameObject("elastic bungee block 2");

                Particle *particleSpring2 = new Particle(Vector3D(0, -5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.6f, 1.0f);
                go->AddComponent(particleSpring2);

                ParticleElasticBungee *particleElasticBungeeForce1 = new ParticleElasticBungee(particleSpring1, 7, 8);
                math->GetParticleForceRegistry()->AddForce(particleSpring2, particleElasticBungeeForce1);

                ParticleElasticBungee *particleElasticBungeeForce2 = new ParticleElasticBungee(particleSpring2, 7, 8);
                math->GetParticleForceRegistry()->AddForce(particleSpring1, particleElasticBungeeForce2);

                v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);
            }

            static int distance = 7;
            ImGui::InputInt("Distance with Main Blob (m)", &distance);
            if (ImGui::Button("Test blob"))
            {

                for (int i = 0; i < gameObjects->size(); i++)
                    gameObjects->at(i)->SetShouldDelete();

                GameObject *go = new GameObject("main block");
                MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

                Vector3D mainPosition = Vector3D(0, 0, 20);

                Particle *particleAnchord = new Particle(mainPosition, Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.9f, 1, 1.0f);
                go->AddComponent(particleAnchord);
                inputForce = &particleAnchord->inputValues;
                InputForce *newInputForce = new InputForce();

                MathPhysicsEngine::GetInstance()->GetParticleForceRegistry()->AddForce(particleAnchord, newInputForce);
                VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go->AddComponent(v);
                gameObjects->push_back(go);

                const int maxBlob = 8;

                std::vector<Particle *> blobs;

                std::vector<Vector3D> positions = {{-1, 1, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, {1, -1, 0}, {0, -1, 0}, {-1, -1, 0}, {-1, 0, 0}};
                Vector3D standardVec = Vector3D(distance, distance, 0);
                for (int i = 1; i <= maxBlob; i++)
                {
                    positions[i - 1].x *= standardVec.x;
                    positions[i - 1].y *= standardVec.y;

                    go = new GameObject("Blob block " + std::to_string(i));
                    Particle *particleSpring = new Particle(mainPosition + positions[i - 1], Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.99f, 1.0f);
                    go->AddComponent(particleSpring);

                    v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                    go->AddComponent(v);
                    gameObjects->push_back(go);

                    ParticleSpring *particleAnchoredSpring = new ParticleSpring(particleAnchord, (float)anchoredSpringConstant, (float)anchoredSpringRestLength);
                    math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                    if (i != 1)
                    {
                        particleAnchoredSpring = new ParticleSpring(blobs.back(), (float)anchoredSpringConstant, (float)anchoredSpringRestLength);
                        math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                        particleAnchoredSpring = new ParticleSpring(particleSpring, (float)anchoredSpringConstant, (float)anchoredSpringRestLength);
                        math->GetParticleForceRegistry()->AddForce(blobs.back(), particleAnchoredSpring);
                    }

                    if (i == maxBlob)
                    {

                        particleAnchoredSpring = new ParticleSpring(blobs.front(), (float)anchoredSpringConstant, (float)anchoredSpringRestLength);
                        math->GetParticleForceRegistry()->AddForce(particleSpring, particleAnchoredSpring);

                        particleAnchoredSpring = new ParticleSpring(particleSpring, (float)anchoredSpringConstant, (float)anchoredSpringRestLength);
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

        if (ImGui::Button("Test collision"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();
            GameObject *go = new GameObject("Left particle");
            Particle *particle = new Particle(
                Vector3D(-10, 0, 20),
                Vector3D(10, 5, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.999f,
                1.0f);
            ParticleGravity *particleGravity = new ParticleGravity({0.0f, -10.0f, 0.0f});
            math->GetParticleForceRegistry()->AddForce(particle, particleGravity);
            go->AddComponent(particle);

            VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);

            GameObject *go2 = new GameObject("Right particle");
            Particle *particle2 = new Particle(
                Vector3D(10, 0, 20),
                Vector3D(-10, 5, 0),
                Vector3D(0.0f, 0.0f, 0.0f),
                1.0f,
                0.999f,
                1.0f);

            ParticleGravity *particleGravity2 = new ParticleGravity({0.0f, -10.0f, 0.0f});
            math->GetParticleForceRegistry()->AddForce(particle2, particleGravity2);
            go2->AddComponent(particle2);

            v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go2->AddComponent(v);
            gameObjects->push_back(go2);
        }

        if (ImGui::Button("Test penetration and resting contact"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            GameObject *go = new GameObject("Heavy non gravity block");
            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

            Particle *particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
            go->AddComponent(particle);
            VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);

            go = new GameObject("Falling block");

            particle = new Particle(Vector3D(0, 5, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.95f, 1.0f);
            go->AddComponent(particle);
            math->GetParticleForceRegistry()->AddForce(particle, math->particleGravity);
            v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
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
        if (ImGui::Button("Test ROD"))
        {

            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();

            GameObject *go = new GameObject("Heavy non gravity block");
            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

            Particle *particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
            go->AddComponent(particle);
            VisualGameObject *v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);

            for (int i = 0; i < rod; i++)
            {

                GameObject *go2 = new GameObject("Rod Extremity " + std::to_string(i));

                Particle *p2 = new Particle(Vector3D(0.1, 5 * (i + 1), 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 0.999f, 1);
                go2->AddComponent(p2);
                math->GetParticleForceRegistry()->AddForce(p2, math->particleGravity);
                VisualGameObject *v2 = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go2->AddComponent(v2);

                gameObjects->push_back(go2);

                math->TestRodCollisionSetup(p2, particle, 5);

                particle = p2;
            }
        }

        static int cable = 2;
        ImGui::InputInt("Cable Numbers", &cable);
        if (ImGui::Button("Test Cable"))
        {
            for (int i = 0; i < gameObjects->size(); i++)
                gameObjects->at(i)->SetShouldDelete();
            GameObject *go;
            Particle *particle;
            VisualGameObject *v;
            MathPhysicsEngine *math = MathPhysicsEngine::GetInstance();

            go = new GameObject("Heavy non gravity block");

            particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
            go->AddComponent(particle);
            v = VisualGameObject::CreatePtrVisualGameObject(modelePath);
            go->AddComponent(v);
            gameObjects->push_back(go);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(-5.0f, 5.0f);

            for (int i = 0; i < cable; i++)
            {

                GameObject *go2 = new GameObject("Cable extremity " + std::to_string(i));

                float posx = dis(gen);
                float posy = dis(gen);

                Particle *p2 = new Particle(Vector3D(posx, posy, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 1, 1);
                go2->AddComponent(p2);
                math->GetParticleForceRegistry()->AddForce(p2, math->particleGravity);
                VisualGameObject *v2 = VisualGameObject::CreatePtrVisualGameObject(modelePath);
                go2->AddComponent(v2);

                gameObjects->push_back(go2);

                math->TestCableCollisionSetup(p2, particle, 10);
            }
        }
    }
    ImGui::TreePop();

    ImGui::End();
}

void ImGuiEngine::SetVisible(bool _visible)
{
    if (visible == _visible)
    {
        return;
    }

    visible = _visible;

    if (visible)
    {
        ImGui_ImplGlfw_InstallCallbacks(window);
    }
    else
    {
        ImGui_ImplGlfw_RestoreCallbacks(window);
    }
}