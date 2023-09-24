#include <GraphicsMotor.h>
#include <chrono>
#include "../MathPhysicEngine.h"

int main() {

    ParticulesData* particulesData = new ParticulesData();

    GraphicsMotor graphicsMotor = GraphicsMotor(particulesData);
    MathPhysicsEngine mathPhysics = MathPhysicsEngine(particulesData);

    mathPhysics.Init();

    double t = 0.0f;

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!graphicsMotor.ExitCondition()) {

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        mathPhysics.Update(t, frameTime);
        graphicsMotor.Update(frameTime);
        graphicsMotor.Render();

        t += frameTime;
    }

    graphicsMotor.Shutdown();

    return EXIT_SUCCESS;
}