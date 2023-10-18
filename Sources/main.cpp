#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine/MathPhysicEngine.h"
#include "../Sources/ImGuiEngine.h"
#include <iostream>

int main() {

    ObjectData* objectData = new ObjectData();

    GraphicsMotor graphicsMotor = GraphicsMotor(objectData);
    MathPhysicsEngine mathPhysics = MathPhysicsEngine(objectData);
    ImGuiEngine imGuiEngine = ImGuiEngine(graphicsMotor.GetGLFWWindow(),objectData);
    mathPhysics.Init();

    double t = 0.0f;
    double dt = 0.01;

    auto currentTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;

    while (!graphicsMotor.ExitCondition()) {

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

            mathPhysics.Update(t, dt);
            t += dt;
            accumulator -= dt;

        }
        const double alpha = accumulator / dt;

        mathPhysics.SetFinalStates(alpha);
        //std::cout << "time: " << t + frameTime << "\n";
        graphicsMotor.Update(frameTime);
        graphicsMotor.Render();

        t += frameTime;
    }

    graphicsMotor.Shutdown();

    return EXIT_SUCCESS;
}