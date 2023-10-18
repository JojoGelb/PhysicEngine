#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../Sources/ImGuiEngine.h"
#include <iostream>
#include "GameObject.h"
#include "../MathPhysicEngine/Forces/ParticleGravity.h"
int main() {

    std::vector<GameObject*> gameObjects;

    GraphicsMotor* graphicsMotor = GraphicsMotor::GetInstance();

    MathPhysicsEngine * mathPhysics = MathPhysicsEngine::GetInstance();
    ImGuiEngine imGuiEngine = ImGuiEngine(graphicsMotor->GetGLFWWindow(), &gameObjects);

    GameObject* go = new GameObject();
    Particle* particle = new Particle();
    ParticleGravity* particleGravity = new ParticleGravity({0.0f,-10.0f,0.0f});
    go->AddComponent(particle);
    mathPhysics->GetParticleForceRegistry()->AddForce(particle, particleGravity);
    VisualGameObject* v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
    go->AddComponent(v);

    //add graphics
    gameObjects.push_back(go);
    
    double t = 0.0f;
    double dt = 0.01;

    auto currentTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;

    while (!graphicsMotor->ExitCondition()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        if (frameTime > 0.25)
            frameTime = 0.25;

        currentTime = newTime;

        accumulator += frameTime;

        imGuiEngine.Update(frameTime);


        while (accumulator >= dt)
        {

            mathPhysics->Update(t, dt);
            t += dt;
            accumulator -= dt;

        }
        const double alpha = accumulator / dt;

        mathPhysics->SetFinalStates(alpha);
        //std::cout << "time: " << t + frameTime << "\n";

        graphicsMotor->Update(frameTime);

        for (auto gameObj : gameObjects) {
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