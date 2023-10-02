#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../Sources/ImGuiEngine.h"
#include "GameObject.h"

int main() {
    //ObjectData* objectData = new ObjectData();

    std::vector<GameObject*> gameObjects;

    GraphicsMotor* graphicsMotor = GraphicsMotor::GetInstance();

    MathPhysicsEngine * mathPhysics = MathPhysicsEngine::GetInstance();
    ImGuiEngine imGuiEngine = ImGuiEngine(graphicsMotor->GetGLFWWindow(), &gameObjects);

    //GameObject* go = new GameObject(mathPhysics, graphicsMotor.GetVulkanHandler(), "Models/colored_cube.obj");
    GameObject* go = new GameObject();
    go->AddComponent(new Particle());
    VisualGameObject* v = VisualGameObject::CreatePtrGameObject("Models/colored_cube.obj");
    go->AddComponent(v);

    //add graphics
    gameObjects.push_back(go);
    
    double t = 0.0f;

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!graphicsMotor->ExitCondition()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        imGuiEngine.Update();
        mathPhysics->Update(t, frameTime);
        graphicsMotor->Update(frameTime);

       for ( auto gameObj : gameObjects) {
            gameObj->Update();
       }

        graphicsMotor->Render();

        t += frameTime;
    }

    for (auto gameObj : gameObjects) {
        delete gameObj;
    }

    graphicsMotor->Shutdown();

    delete MathPhysicsEngine::GetInstance();
    delete GraphicsMotor::GetInstance();

    return EXIT_SUCCESS;
}