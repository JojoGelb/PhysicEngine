#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../Sources/ImGuiEngine.h"
#include "GameObject.h"

int main() {

    ObjectData* objectData = new ObjectData();
    std::vector<GameObject*> gameObjects;

    GraphicsMotor graphicsMotor = GraphicsMotor();

    MathPhysicsEngine mathPhysics = MathPhysicsEngine();
    ImGuiEngine imGuiEngine = ImGuiEngine(graphicsMotor.GetGLFWWindow(),objectData);
    mathPhysics.Init();

    GameObject* go = new GameObject(mathPhysics, graphicsMotor.GetVulkanHandler(), "Models/colored_cube.obj");
    gameObjects.push_back(go);

    double t = 0.0f;

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!graphicsMotor.ExitCondition()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        imGuiEngine.Update();
        mathPhysics.Update(t, frameTime);
        graphicsMotor.Update(frameTime);

       for ( auto gameObj : gameObjects) {
            gameObj->UpdateVisual();
       }

        graphicsMotor.Render();

        t += frameTime;
    }

    graphicsMotor.Shutdown();

    for (auto gameObj : gameObjects) {
        delete gameObj;
    }

    return EXIT_SUCCESS;
}