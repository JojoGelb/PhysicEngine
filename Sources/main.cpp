#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../Sources/ImGuiEngine.h"
#include "GameObject.h"

int main() {

    std::vector<GameObject*> gameObjects;

    GraphicsMotor* graphicsMotor = GraphicsMotor::GetInstance();

    MathPhysicsEngine * mathPhysics = MathPhysicsEngine::GetInstance();
    ImGuiEngine imGuiEngine = ImGuiEngine(graphicsMotor->GetGLFWWindow(), &gameObjects);

    /*-------------------------TEST ZONE-----------------------------*/

    GameObject* go = new GameObject();
    go->AddComponent(new Particle(Vector3D(-10,0,20),Vector3D(0,0,0),Vector3D(0,0,0),0.0001f,0.999f,0));
    VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
    go->AddComponent(v);

    GameObject* go2 = new GameObject();
    go2->AddComponent(new Particle(Vector3D(10, 0, 20), Vector3D(-50, 0, 0), Vector3D(0, 0, 0), 1.0f, 0.97f, 0));
    VisualGameObject* v2 = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
    go2->AddComponent(v2);

    //add graphics
    gameObjects.push_back(go);
    gameObjects.push_back(go2);

    /*------------------------TEST ZONE--------------------------*/
    
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
    delete GraphicsMotor::GetInstance();

    delete MathPhysicsEngine::GetInstance();


    return EXIT_SUCCESS;
}