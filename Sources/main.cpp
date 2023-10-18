#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../Sources/ImGuiEngine.h"
#include <iostream>
#include "GameObject.h"
#include "../MathPhysicEngine/Forces/ParticleGravity.h"
#include <random>

int main() {

    std::vector<GameObject*> gameObjects;

    GraphicsMotor* graphicsMotor = GraphicsMotor::GetInstance();

    MathPhysicsEngine * mathPhysics = MathPhysicsEngine::GetInstance();
    ImGuiEngine imGuiEngine = ImGuiEngine(graphicsMotor->GetGLFWWindow(), &gameObjects);

    /*-------------------------TEST ZONE-----------------------------*/
    GameObject* go;
    Particle* particle;
    VisualGameObject* v;

    ParticleGravity* particleGravity = new ParticleGravity({ 0.0f,-10.0f,0.0f });

    // Test cable
    /*go = new GameObject();

     particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
    go->AddComponent(particle);
    v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
    go->AddComponent(v);
    gameObjects.push_back(go);

    

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-5.0f, 5.0f);

    for (int i = 0; i < 10; i++) {
    
        GameObject* go2 = new GameObject();

        float posx = dis(gen);
        float posy = dis(gen);

        Particle* p2 = new Particle(Vector3D(posx, posy, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 1, 1);
        go2->AddComponent(p2);
        mathPhysics->GetParticleForceRegistry()->AddForce(p2, particleGravity);
        VisualGameObject* v2 = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
        go2->AddComponent(v2);

        gameObjects.push_back(go2);

        mathPhysics->TestCableCollisionSetup(p2, particle, 10);
    }*/

    //Test ROD
    go = new GameObject();

    particle = new Particle(Vector3D(0, 0, 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 0.00000001f, 1, 1.0f);
    go->AddComponent(particle);
    v = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
    go->AddComponent(v);
    gameObjects.push_back(go);

    for (int i = 0; i < 3; i++) {

        GameObject* go2 = new GameObject();


        Particle* p2 = new Particle(Vector3D(0.1, 5* (i+1), 20), Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1, 1, 1);
        go2->AddComponent(p2);
        mathPhysics->GetParticleForceRegistry()->AddForce(p2, particleGravity);
        VisualGameObject* v2 = VisualGameObject::CreatePtrVisualGameObject("Models/colored_cube.obj");
        go2->AddComponent(v2);

        gameObjects.push_back(go2);

        mathPhysics->TestRodCollisionSetup(p2, particle, 5);

        particle = p2;
    }

    //add graphics
    


    

    /*------------------------TEST ZONE--------------------------*/
    
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
            //std::cout << dt << "\n";

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